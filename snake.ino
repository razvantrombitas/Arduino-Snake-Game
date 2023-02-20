#include <SPI.h>
#include <TFT.h>

enum GameState{Logo, Game, GameOver} gameState;
enum SnakeDirection{Up, Down, Right, Left} snake_direction = Right;

// LCD module pins
#define CS   10
#define DC   8
#define RST  9

const int buzzer = 3; //set digital pin for buzzer 
TFT tft = TFT(CS, DC, RST); //screen reference 
byte mapGrid[800]; //array of snake elements

//Snake initial position&length
byte x = 5;
byte y = 5;
byte start_Snake_length = 4;
byte snake_length = start_Snake_length;

//apple position
byte appleX = 40;
byte appleY = 40;

void setup() {
  gameState = 0;
  tft.begin();
  tft.background(10, 10, 250); //(R, G, B)
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
}

void loop() {  
  if (gameState == Logo) {
      logo();
  }
  else if (gameState == Game) {
      game();
  }
  else if (gameState = GameOver) {
    gameOver();
  }
}

void gameOver() {
  tft.stroke(0, 255, 0); //Called before drawing an object on screen, it sets the color of lines and borders around shapes
  tft.fill(0, 255, 0); //Called before drawing an object on screen, it sets the fill color of shapes and text

  tft.background(0,0,0);
  tft.setTextSize(2);
  tft.text("GAME OVER", 28, 40); //Write text to the screen at the given coordinates
  
  digitalWrite(buzzer, HIGH); //turn on the buzzer 
  delay(500);    
  digitalWrite(buzzer, LOW); //turn off the buzzer
  delay(500);        
  
  char pointChars[4];
  String pointVal = String(snake_length-4);
  pointVal.toCharArray(pointChars, 4); //myString.toCharArray(buf, len) Copies the string’s characters to the supplied buffer
  
  tft.setTextSize(2);
  tft.text("Score:", 37, 60);
  tft.text(pointChars, 107, 60);
  
  tft.setTextSize(1);
  tft.text("Down to restart", 15, 110);
  
  while(true) {
    if (analogRead(A1) > 600) {
       resetGame();
       gameState = Game;
       break;
    } 
  }
 }

void resetGame() {
   snake_length = start_Snake_length;
   x = 5;
   y = 5;
   appleX = 40;
   appleY = 40;
   snake_direction = Right;
   resetMap();
}

void resetMap() {
  for (short i = 0; i < 800; i++) {
     mapGrid[i] = 0;
  } 
}

void logo() {
  tft.background(0, 0, 0); //129, 150, 192
  
  tft.stroke(0, 255, 0);
  tft.fill(0, 255, 0);
  
  tft.setTextSize(4);
  tft.text("Snake", 20, 10);
 
  tft.setTextSize(2);
  tft.text("for Arduino", 12, 45);
  
  tft.setTextSize(1);
  tft.text("Trombitas Razvan", 25, 75);
  tft.text("MES, ACES 2022", 12, 105);
  
  delay(4000);
  tft.background(0, 0, 0);//129, 150, 192
  tft.setTextSize(2);
  tft.text("Drag Down", 20, 45);
  tft.text("to start", 30, 65);
  
  while(true) {
    if (analogRead(A1) > 600) {
      gameState = Game;
      break;
    }
  }
}

void game() {
  tft.background(129, 150, 192);
  
  if (appleX == 40 && appleY == 40) 
     generateApple(); //after eating an apple, generate a new one 

  // apple found, increase length
  if (x == appleX && y == appleY) {
    appleX = 40;
    appleY = 40;
    snake_length++;
  }
  
  Serial.println(snake_length);
  handleInput();
  movement();
  timerDecrease();
  updateMap();
  draw();
  delay(50); // a higher value, a slower gameplay 
}

void generateApple() {
  appleX = (byte)random(1, 31);
  appleY = (byte)random(1, 24);
  Serial.print("RANDOM: x=");
  Serial.print(appleX);
  Serial.print(", y=");
  Serial.print(appleY); 
}

// X axis - A0
// Y axis - A1
void handleInput() {
  
  if (analogRead(A1) > 600) { 
     snake_direction = Down; 
  }
  else if(analogRead(A1) < 400) { 
     snake_direction = Up; 
  }
  else if(analogRead(A0) > 600) { 
     snake_direction = Right; 
  }
  else if(analogRead(A0) < 400) { 
     snake_direction = Left; 
  }
  
  Serial.print("A0:");
  Serial.print(analogRead(A0));
  Serial.print("\n");
  Serial.print("A1:");
  Serial.print(analogRead(A1));
  Serial.print("\n");
}

void updateMap() {
   byte xi = 0;
   byte yi = 0;
  
   for (short i = 0; i < 800; i++) {
     
      if (x == xi && mapGrid[i] > 0 && y == yi) {
         gameState = GameOver; 
      }
      
      if (xi == x && yi == y) {
         mapGrid[i] = snake_length; 
      }
      
      xi++;
      if (xi == 32) {
        xi = 0;
        yi++; 
      }
  } 
}

void timerDecrease() {
  for (short i = 0; i < 800; i++) {
    if (mapGrid[i] > 0) {
         mapGrid[i] -= 1; 
    }
  } 
}

void movement() {
  if (snake_direction == Up) {
    if (y != 0) {
      y-=1;
    }
    else {
      y = 24; 
    }
  } 
  
  if (snake_direction == Down) {
    if (y != 24) {
      y++;
    }
    else {
      y = 0;
   } 
  }
  
  if (snake_direction == Right) {
    if (x != 31) {
      x++;
    }
    else {
      x = 0;
   } 
  }
  
  if (snake_direction == Left) {
    if (x != 0) {
      x-=1;
    }
    else {
      x = 31;
    }
  } 
  Serial.print("Movement x:");
  Serial.print(x);
  Serial.print(", y:");
  Serial.print(y);
  Serial.print("\n");
}

void wallCollision() {
  if (x == 32) {
     x = 0; 
     Serial.println("A");
     return;
  }
  else if (x == 0) {
     x = 32; 
     Serial.println("B");
     return;
  }
  else if (y == 25) {
     y = 0; 
     Serial.println("C");
     return;
  }
  else if (y == 0) {
     y = 25; 
     Serial.println("D");
     return;
  }
}

void draw() {
  drawApple(appleX, appleY);
  byte x = 0;
  byte y = 0;
  
  for (short i = 0; i < 800; i++) {
    if (mapGrid[i] > 0) {
      drawSnakePoint(x, y);
    } 
    x++;
    if (x == 32) {
      x = 0;
      y++; 
    }
  } 
}

void drawApple(byte x, byte y) {
  tft.stroke(200, 20, 20);
  tft.fill  (200, 20, 20);
  
  //screen.rect(xStart, yStart, width, height) Draws a rectangle to the TFT screen
  tft.rect((x * 5) + 0, (y * 5) + 2, 5, 2); 
  tft.rect((x * 5) + 1, (y * 5) + 4, 3, 1);
  tft.rect((x * 5) + 1, (y * 5) + 1, 3, 1);
  tft.stroke(20, 200, 20);
  tft.rect((x * 5) + 1, (y * 5) + 0, 2, 1);
}
/*
 - max value of x is 32: 32*5 = 160
 - max value of y is 25: 25*5 = 125
 - the screen resolution is 128*160, so these values were chosen in order to obtain a segment width&heigth of 5 pixels each
*/
void drawSnakePoint(byte x, byte y) {
  tft.stroke(20, 100, 20);
  tft.fill  (20, 100, 20);
  tft.rect(x * 5, y * 5, 5, 5);
}
