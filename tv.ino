#include <TVout.h>
#include <fontALL.h>

#define FPS 15
#define CHANGE_STATE LOW
#define PLAYER_VELOCITY 2
#define BALL_VELOCITY 2
#define OFFSET 2
#define OFFSET_SCREEN 4
#define GAME_OFFSET 40
#define GAME_STATS_OFFSET 4

void updatePlayers(int delta);
int getPlayerDirection(int number);

typedef struct {
	int x;
	int y;
	int wins;
	int size;
} PLAYER;

typedef struct {
	int x;
	int y;
	int vx;
	int vy;
	int diameter;
} BALL;

 typedef struct {
 	int width;
 	int height;
 	int startx;
	int starty;

 } DISPLAY_INFO;

typedef enum {
	RUN,
	STOP
} GAME_STATE;

 TVout TV;

PLAYER right;

PLAYER left;

DISPLAY_INFO display_info;

GAME_STATE game_state;

BALL ball;

int player_velocity = PLAYER_VELOCITY;

int ball_velocity = BALL_VELOCITY;

int playerPins[2][2] = {{4,5},{6,7}};

unsigned long lastUpdate;

void setup()  {
  TV.begin(_PAL);
  display_info.height = TV.vres() - OFFSET_SCREEN;
  display_info.width = TV.hres() - OFFSET_SCREEN - GAME_OFFSET;
  display_info.starty = 0;
  display_info.startx = OFFSET_SCREEN;

  left.wins = 0;
  right.wins = 0;
 
  initGame();
  game_state = RUN;
  Serial.begin(9600);
}

void initGame() {

  left.x = display_info.startx;
  left.y = ((display_info.height) / 2)-5;
  left.size = 10;

  right.x = display_info.width - OFFSET_SCREEN ;
  right.y = ((display_info.height) / 2)+35;

  right.size = 10;


  ball.x = (display_info.width-display_info.startx)/2;
  ball.y = (display_info.height)/2;
  ball.vx = 1;
  ball.vy = 1;
  ball.diameter = 2;
  lastUpdate = millis();
  game_state = RUN;
}

void loop() {
  TV.clear_screen();
  int delta = millis()-lastUpdate;
  delta = delta / 100;
  updateGameState(delta);
  lastUpdate = millis();
  drawGame();
  TV.delay(20);
}

void updateGameState(int delta) {
	if(game_state == RUN) {
		updateBall(delta);
		if(game_state == RUN) {
			updatePlayers(delta);
		}
    } else {
    	TV.print_str(30, 40, "LOOSER!");
    	initGame();
    }
}

void drawGame() {
	printStats();
	drawBorders();
	drawBall(ball);
	drawPlayer(left);
	drawPlayer(right);
}

int getNumber(int number, int position) {
	if(number > 9) {
		int first = number / 10;
		if(position == 0) {
			return number-(first*10);
		}
		return first;
	} else {
		if(position == 0) {
			return number;
		}
		return 0;
	}
}

char casToChar(int number) {
	return '0'+number;
}

void printStats() {
	char result[6];
	result[0] = casToChar(getNumber(left.wins, 1));
	result[1] = casToChar(getNumber(left.wins, 0));
	result[2] = '-';
	result[3] = casToChar(getNumber(right.wins, 1));
	result[4] = casToChar(getNumber(right.wins, 0));
	result[5] = 0;
	TV.print_str(display_info.width + GAME_STATS_OFFSET + 6, display_info.starty+ 4, "PONG");
	TV.print_str(display_info.width+ GAME_STATS_OFFSET + 4, display_info.starty + 20, result);
}


void drawBorders() {
	//GAME 
	TV.draw_line(display_info.startx-OFFSET_SCREEN, display_info.starty, display_info.startx-OFFSET_SCREEN, display_info.height, '1');

	TV.draw_line(display_info.width, display_info.height, display_info.startx-OFFSET_SCREEN, display_info.height, '1');

	TV.draw_line(display_info.width, display_info.starty, display_info.startx-OFFSET_SCREEN, display_info.starty, '1');

	TV.draw_line(display_info.width, display_info.starty, display_info.width, display_info.height, '1');

	//STATS
	TV.draw_line(display_info.width + GAME_STATS_OFFSET, display_info.starty, display_info.width + GAME_STATS_OFFSET, 
		display_info.height, '1');

	TV.draw_line(display_info.width + GAME_OFFSET, display_info.starty, display_info.width + GAME_OFFSET, 
		display_info.height, '1');

	TV.draw_line(display_info.width + GAME_STATS_OFFSET, display_info.height, 
		display_info.width + GAME_OFFSET, display_info.height, '1');

	TV.draw_line(display_info.width + GAME_STATS_OFFSET, display_info.starty, 
		display_info.width+GAME_OFFSET, display_info.starty, '1');

}

void drawBall(BALL ball) {
	int radius = ball.diameter / 2;
	TV.draw_circle(ball.x-radius, ball.y - radius, radius, '1','0','0');
}

void drawPlayer(PLAYER player) {
	TV.draw_line(player.x, player.y - player.size / 2, player.x, player.y + player.size / 2, '1');
}

void updateBall(int delta) {
	int newX = ball.x + (ball.vx*delta*ball_velocity);
	int newY = ball.y + (ball.vy*delta*ball_velocity);

	if(newY + ball.diameter/2 >= display_info.height) {
		newY = display_info.height - ball.diameter/2;
		ball.vy = -1;
	}

	if(newY - ball.diameter/2 <= 0) {
		newY = display_info.starty + ball.diameter/2;
		ball.vy = 1;
	}
	ball.x = newX;
	ball.y = newY;

	int ballDirection = newBallXdirection(newX, newY);
	if(ballDirection == 0) {
		if(newX < display_info.startx) {
			game_state = STOP;
			right.wins++;
		} else if(newX > display_info.width - OFFSET) {
			game_state = STOP;
			left.wins++;
		}
	} else {
		ball.vx = ballDirection;
	}
}

int isInCircle(int x, int y, int testx, int testy, int allowedOffset, int radius) {
	int dx = abs(testx-x);
    int dy = abs(testy-y);
    return (dx+dy) < (radius + allowedOffset);
}

int newBallXdirection(int x, int y) {
	int radius = ball.diameter/2;
	if(isInCircle(right.x, right.y, ball.x, ball.y, OFFSET, radius) || 
		isInCircle(right.x, right.y+right.size/2, ball.x, ball.y, OFFSET, radius) ||
		isInCircle(right.x, right.y-right.size/2, ball.x, ball.y, OFFSET, radius) ) {
			return -1;
	} else if(isInCircle(left.x, left.y, ball.x, ball.y, OFFSET, radius) ||
		isInCircle(left.x, left.y + left.size/2, ball.x, ball.y, OFFSET, radius) ||
		isInCircle(left.x, left.y - left.size/2, ball.x, ball.y, OFFSET, radius)) {
			return 1;
	}
	return 0;
}


void updatePlayers(int delta) {
	int leftDirection = getPlayerDirection(0);
	int rightDirection = getPlayerDirection(1);
	updatePlayer(right, rightDirection, delta);
	updatePlayer(left, leftDirection, delta);
}

void updatePlayer(PLAYER player, int direction, int delta) {
	int newY = (player.y + (direction * player_velocity * delta));
	if(newY < display_info.starty) {
		newY = display_info.starty;
	} else if(newY > display_info.height) {
		newY = display_info.height;
	}
}

int getPlayerDirection(int number) {
	int left = playerPins[number][0];
	int right = playerPins[number][1];
	left = digitalRead(left) ==  CHANGE_STATE;
	right = (digitalRead(right) == CHANGE_STATE) * - 1;
	return left+right;
}
