#define period 10

//const int leftMotorPin = 9;
//const int rightMotorPin = 8;
//
//const int forwardMotorPin = 10;
//const int backwardMotorPin = 11;

int pwm_a = 3;  //PWM control for motor outputs 1 and 2 
int pwm_b = 9;  //PWM control for motor outputs 3 and 4 
int dir_a = 2;  //direction control for motor outputs 1 and 2 
int dir_b = 8;  //direction control for motor outputs 3 and 4 

int temp = 0;
int speedy = 0;
const int headlightsL = 6;
const int headlightsR = 7;
const int horn = 5;
int tilt = 12;

const int EEGpin = 4;
const int led = 13;

char go;
char oneDirection;
char hornChar;
char headlightsChar;
boolean headlightsOn = false;
boolean tiltFlag = true;

const char s = 'M';

void setup() {                
  Serial.begin(9600);
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  pinMode(tilt, INPUT);
  pinMode(headlightsL, OUTPUT); 
  pinMode(headlightsR, OUTPUT); 
  pinMode(horn, OUTPUT);
  
  pinMode(EEGpin, INPUT);
  pinMode(led, OUTPUT);
  
  analogWrite(pwm_a, 0);
  analogWrite(pwm_b, 0);
//  pinMode(leftMotorPin, OUTPUT);   
//  pinMode(rightMotorPin, OUTPUT);  
//  pinMode(forwardMotorPin, OUTPUT);   
//  pinMode(backwardMotorPin, OUTPUT); 
}

// the loop routine runs over and over again forever:
void loop() {
  //Serial.print("Tilt: ");
  //Serial.print(digitalRead(tilt));
  //Serial.print('\n');
  
 //EEG More 80% attention, go forward
  if(digitalRead(EEGpin)){
    digitalWrite(led, HIGH);
    while(digitalRead(EEGpin)){
        digitalWrite(dir_b, HIGH);
        analogWrite(pwm_b, 210);
        //Serial.flush();
    }
  }
 if(!(digitalRead(EEGpin))){
    digitalWrite(led, LOW);
    //Serial.flush();
    Stop();
    straight();
  }
  
  if (temp > 79) 
  {
    //Serial.println("Tilt Flag true");
    tiltFlag = false;
    Stop();
  } 
  else
  {
    tiltFlag = true; //false
    
   // Serial.println("Tilt Flag false");
  }
  
  if (Serial.available() > 0)
  {
    speedy = Serial.read(); 
    Serial.print("Speed: ");
    Serial.print(speedy);
    Serial.print('\n'); 
  }
  
  if (Serial.available() > 0)
  {
    temp = Serial.read();
    Serial.print("Temp: ");
    Serial.print(temp);  
    Serial.print('\n');
  }
  
  if (Serial.available() > 0)
  {
    go = Serial.read();
    
    // stop
    if (go == 's' && tiltFlag == true) 
    {
      Serial.print("Car is STOPPED and ");
      Stop();
    }
    
    // go
    else if (go == 'g' && tiltFlag == true)
    {
      Serial.print("Car is GOING and ");
      forward(speedy);
    }
    
    // reverse
    else if (go == 'r' && tiltFlag == true)
    {
      Serial.print("Car is REVERSING and ");
      backward(speedy);
    }
  }
  if (Serial.available() > 0)
  {
    oneDirection = Serial.read();
    // staight
    if (oneDirection == 's' && tiltFlag)
    {
      Serial.println("wheels are turned STRAIGHT");
      straight(); 
      
    }
    
    // left
    else if (oneDirection == 'l' && tiltFlag)
    {
      Serial.println("wheels are turned LEFT");
      left();
    }
    
    // right
    else if (oneDirection == 'r' && tiltFlag)
    {
      Serial.println("wheels are turned RIGHT");
      right();
    }
  }
  if (Serial.available() > 0)
  {
    hornChar = Serial.read();
    if (hornChar == 'h')
    {
      analogWrite(horn, 50);
    }
    else
    {
      analogWrite(horn, 0);
    }
  }
  
  if (Serial.available() > 0)
  {
    headlightsChar = Serial.read();
    if (headlightsChar == s)
    {
      headlightsOn = !headlightsOn;
    }
  }
  if (headlightsOn)
  {
    digitalWrite(headlightsL, HIGH);
    digitalWrite(headlightsR, HIGH);
  } 
  else 
  {
    digitalWrite(headlightsL, LOW);
    digitalWrite(headlightsR, LOW);
    
  }
//  Serial.print("go = ");
//  Serial.println(go);
//  Serial.print("left = ");
//  Serial.println(oneDirection);

  delay(180);  

    
    
 

  
}


void left(){
  digitalWrite(dir_a, LOW);
  analogWrite(pwm_a, 225);
}

void right(){
  digitalWrite(dir_a, HIGH);
  analogWrite(pwm_a, 200);
}

void forward(int speedy){
  int speedSet = speedy * 2;
  digitalWrite(dir_b, HIGH);
  analogWrite(pwm_b, speedSet);
}

void backward(int speedy){
  int speedSet = speedy * 2;
  digitalWrite(dir_b, LOW);
  analogWrite(pwm_b, speedSet);
}

void straight(){
//  digitalWrite(dir_a, LOW);
  analogWrite(pwm_a, 0);
}

void Stop(){
  analogWrite(pwm_b, 0);
}
