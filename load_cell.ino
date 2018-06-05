#include <HX711.h>
#include <Servo.h>
//functions for weight sensor, pill dispensing (wait for touch), IR sensor(is pill available)

#define R1 2
#define G1 3
#define B1 4
#define Y 5
#define R2 6
#define G2 7
#define B2 8
#define Touch_Button_Pin_1 9
#define Speaker 15
#define Left_Button 10
#define Right_Button 11
#define IR_Sensor 14

Servo servo;
// Scale Settings
const int SCALE_DOUT_PIN = 0;
const int SCALE_SCK_PIN = 1;

int IRsensorValue = 0;        // value read from the receiver
int touchState = 0;        //Touch Sensor state
String incomingString = "";    //Serial read string
String temp = "";             //temporary String
String Dispense = "Dispense"; //Dispense Command
int count = 0;                //Dispense Amount
int leftButton;           //left button state
int lastLeftButton = LOW;       //previous left button state
int leftReading;            //left reading
int rightButton;          //right button state
int lastRightButton = LOW;       //previous right button state
int rightReading;           //right reading
unsigned long lastDebounceTime = 0; //last time button was pressed
unsigned long debounceDelay = 50;   //debounce time
int RButton = 0;        //keep track of toggle state
int LButton = 0;        //keep track of toggle state

HX711 scale(SCALE_DOUT_PIN, SCALE_SCK_PIN);

void setup() {
  servo.attach(16); //D11
  servo.write(90);
  pinMode(IR_Sensor, INPUT);
  pinMode(Speaker, OUTPUT);
//  analogWrite(Speaker, 0);
  pinMode(2, OUTPUT);
  pinMode(Touch_Button_Pin_1, INPUT);
  pinMode(Left_Button, INPUT);
  pinMode(Right_Button, INPUT);
  pinMode(R1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(B2, OUTPUT);
  Serial.begin(9600);
  scale.set_scale(1800.00/24.00);
  scale.tare();
}

void loop() {
  senseWeight();
  
  //read Serial
  while(Serial.available()){
    delay(1000);
    incomingString="";
    temp="";
    incomingString = Serial.readString();
    if(incomingString.length() > 0){
      temp = incomingString;
      temp.remove(8);
      if(temp.equals(Dispense)){
        temp = incomingString;
        temp.remove(0,9);
        count = temp.toInt();
        Serial.println(count);
        int timer = 0;
        while (pettouchSensor() != 1){  
          if (timer%30 == 0){
            Serial.println("Alarm Disp"); 
          }
//        analogWrite(Speaker, 1000);
//        delay(500);
//        analogWrite(Speaker, 0);
//        delay(500);
//        analogWrite(Speaker, 1000);
//        delay(500);
//        analogWrite(Speaker, 0);
//        delay(500);
//        analogWrite(Speaker, 1000);
//        delay(500);
//        analogWrite(Speaker, 0);
        delay(100);
        timer = timer + 1;
        }
        Serial.println("Ack Disp");
        setColor1(0,0,0);
        setColor2(0,0,0);
        delay(100);
        setColor1(0,255,0);
        delay(100);
        setColor1(0,0,0);
        delay(100);
        setColor2(0,255,0);
        delay(100);
        setColor2(0,255,0);
        delay(100);
//        analogWrite(Speaker, 300);
//        delay(500);
//        analogWrite(Speaker, 0);
        dispense(count);
        
        break;
      }
      else{
        Serial.println(incomingString);
        break;
      }
    }
  }

  if (pettouchSensor() == 1){
    Serial.println("Pet");
    setColor1(0,0,0);
    setColor2(0,0,0);
    delay(100);
    setColor1(0,0,255);
    setColor2(0,0,255);
    delay(100);
    setColor1(0,0,0);
    setColor2(0,0,0);
//    analogWrite(Speaker, 300);
//    delay(500);
//    analogWrite(Speaker, 0);
  }

  if (leftEar() == 1){
    Serial.println("Left Ear");
    setColor1(0,0,0);
    setColor2(0,0,0);
    delay(100);
    setColor1(0,0,255);
    delay(100);
//    analogWrite(Speaker, 100);
//    delay(500);
//    analogWrite(Speaker, 0);
    setColor1(0,0,0);
  }

  if (rightEar() == 1){
    Serial.println("Right Ear");
    setColor1(0,0,0);
    setColor2(0,0,0);
    delay(100);
    setColor2(0,0,255);
    delay(100);
//    analogWrite(Speaker, 500);
//    delay(500);
//    analogWrite(Speaker, 0);
    setColor2(0,0,0);
  }
}

void senseWeight(){
  float w = scale.get_units(1); //get weight
  String weight = String(w, 2);
  Serial.println(weight);
  if (w <= 20){
    setColor1(255,0,0);
    digitalWrite(Y,LOW);
    setColor2(255,0,0);
    Serial.println("Empty");
    delay(1000);
  }
  else if (w > 20 && w <= 100){
    digitalWrite(G1, LOW);
    setColor1(0,0,0);
    digitalWrite(Y, HIGH);
    setColor2(0,0,0);
    Serial.println("Low");
    delay(1000);
  }
  else if (w > 100){
    setColor1(0,255,0);
    digitalWrite(Y, LOW);
    setColor2(0,255,0);
    Serial.println("Gucci");
    delay(1000);
  }
}

void dispense(int count){
  int i = 0;
  int start = millis();
  while (i < count){
   servo.write(55);
   if (pillDispensed() == 1){              //If pill dispensed
      start = millis();
      i++;
   }
   if (millis() - start > 10000){
    Serial.println("Jam");
    break;
   }
  }
  servo.write(90);
  Serial.println("Done Disp");
}

bool pillDispensed(){
  // read the analog in value:
  IRsensorValue = analogRead(IR_Sensor);

  // print the results to the serial monitor:
  Serial.print("\nsensor = ");
  Serial.print(IRsensorValue);
  //the threshold found fron analog In Out program was when object is detected, the sensor value is below 100
  //set the threshold whihc you get
  //the threshold varies for different sets of emitter-receiver pairs
  if(IRsensorValue < 100){ //checks if object is there or not
    Serial.print("\nObject Detected");
    return true;
    }
  else{
    Serial.print("\nNo object in Front");
    return false;
    }
}

bool pettouchSensor(){
  return digitalRead(Touch_Button_Pin_1);         //check if touched
}

bool leftEar(){
  int state = digitalRead(Left_Button);
  if (LButton != state){
    LButton = state;
    return true;
  }
  return false;
}

bool rightEar(){
  int state = digitalRead(Right_Button);
  if (RButton != state){
    RButton = state;
    return true;
  }
  return false;
  }

void setColor1 (int redValue, int greenValue, int blueValue) {
  analogWrite(R1, redValue);
  analogWrite(G1, greenValue);
  analogWrite(B1, blueValue);
}

void setColor2(int redValue, int greenValue, int blueValue) {
  analogWrite(R2, redValue);
  analogWrite(G2, greenValue);
  analogWrite(B2, blueValue);
}
