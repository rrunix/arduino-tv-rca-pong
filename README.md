arduine-tv-rca-pong
===================

Pong game with arduino on the tv.

The game is very simple and is still in development. Tasks to do :
  
  - Test controllers for the players
  - Develop a bot (for being able to play a single player)
  
The library https://code.google.com/p/arduino-tvout/ is used to be able to draw on the screen, it works like a charm but
is a low level library and only is able to draw pixels, circles, boxes and lines in two colors {black, white}.

I am thinking in develop a game library that goes on the top of tvout to avoid such low level things and make
easy develop games in arduino.

assembly
===================

The assembly is very easy, the parts you need are the following :
  
  - Two wires
  - Two resistors (1k and 470Ω) ( I don't have a 470 resistor, so I put two 220 resistors in serie and it works as well)
  - An rca wire. ( I bought a 2$ wire and it always works )
  
In this example we use an arduino uno, so depending on the arduino the wires may go in another pins.

![alt wires](https://raw.github.com/rrunix/arduine-tv-rca-pong/master/assembly/1.jpg)
![alt connection arduino](https://raw.github.com/rrunix/arduine-tv-rca-pong/master/assembly/2.jpg)
![alt all the board ](https://raw.github.com/rrunix/arduine-tv-rca-pong/master/assembly/3.jpg)
![alt all the board 2](https://raw.github.com/rrunix/arduine-tv-rca-pong/master/assembly/4.jpg)
![alt game on screen](https://raw.github.com/rrunix/arduine-tv-rca-pong/master/assembly/screen.jpg)

Note: if you are using another board you can see the guide https://code.google.com/p/arduino-tvout/

[![Alt video](https://www.youtube.com/vi/Ct_DD2c47ZI/0.jpg)](https://www.youtube.com/watch?v=Ct_DD2c47ZI)
