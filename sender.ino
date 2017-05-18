#include <LiquidCrystal.h>

#define lowThreshold 425
#define highThreshold 597
int headlights = 10; // pin for headlights
int horn = 9; // pin for horn
int xPin = 1; // pin for X direction
int yPin = 2; // pin for Y direction
int speedPin = 3; //analog input
int tempPin = 4;

int directionX; // stores the X direction from controller
int directionY; // stores the Y direction from controller
int speedy = 100;
int temp = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);

// arrow up
byte up[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000
};

// arrow down
byte down[8] = {
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000
};

// arrow right
byte right[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};
 
// arrow left
byte left[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

byte farhen[8] = {
  B11100,
  B10100,
  B11100,
  B01111,
  B01000,
  B01110,
  B01000,
  B01000
};

void setup() {                
  Serial.begin(9600);;
  pinMode(headlights, INPUT);
  pinMode(horn, INPUT);
 // pinMode(xPin, INPUT);
 // pinMode(yPin, INPUT);

  // set up the LCD's number of columns and rows: 
  lcd.createChar(0, up);
  lcd.createChar(1, down);
  lcd.createChar(2, right);
  lcd.createChar(3, left);
  lcd.createChar(4, farhen);
  lcd.begin(16, 2);
  lcd.clear();
  //analogReference(INTERNAL);
}

void loop() {
  lcd.clear();
  directionX = analogRead(xPin);
  directionY = analogRead(yPin);

  //--------------------------Y DIRECTION--------------------------------
  // stop
  speedy = analogRead(speedPin) / 12 + 170;
  lcd.setCursor(0,0);
  int speedyPercent = 100.0 * (speedy - 170) / (255 - 170);
  lcd.print(speedyPercent);
  lcd.setCursor(4, 0);
  lcd.print('%');
  Serial.write(speedy);
  lcd.setCursor(0,1);
  int reading = analogRead(tempPin); 
  float voltage = reading / 1024 * 500;
  float tempC = reading / 9.31 - 32;
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
  lcd.print(tempF);
  lcd.setCursor(6, 1);
  lcd.print((char)4);
  Serial.write((int)tempF);
  if (directionY < highThreshold && directionY > lowThreshold)
  {
    Serial.write("M"); // stop
  } 
  // go
  else if (directionY < lowThreshold)
  {
    lcd.setCursor(14,0);
    lcd.print((char)0);
    Serial.write("g"); // go
  }
  
  // reverse
  else
  {
    lcd.setCursor(14,1);
    lcd.print((char)1);
    Serial.write("r"); // reverse
  }

  //--------------------------X DIRECTION--------------------------------
  // straight
  if (directionX < highThreshold && directionX > lowThreshold)
  {
    Serial.write("s"); // straight
  }
  // left
  else if (directionX < lowThreshold)
  {
    lcd.setCursor(13,0);
    lcd.print((char)3);
    Serial.write("l"); // left
  }
  // right
  else 
  {
    lcd.setCursor(15,0);
    lcd.print((char)2);
    Serial.write("r"); // right
  }
  
//  //---------------------------- HORN---------------------------------
  if (digitalRead(horn) == HIGH)
  {
    Serial.write("h"); // horn
  }
  else
  {
    Serial.write("n"); // silent
  }
  
//  //---------------------------HEADLIGHTS-----------------------------

  if (digitalRead(headlights) == HIGH)
  {
    Serial.write("M"); // switch state
  }
  else
  {
    Serial.write("k"); // keep state
  }
  
  //Serial.flush();
  delay(180);

}
