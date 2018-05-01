#include <HX711.h>
#include <Servo.h>
//functions for weight sensor, pill dispensing (wait for touch), IR sensor(is pill available)

#define G D2
#define Y D3
#define R D4
#define Touch_Button_Pin D6

Servo servo;
// Scale Settings
const int SCALE_DOUT_PIN = D0;
const int SCALE_SCK_PIN = D1;

const int analogInPin = A0;  // Analog input pin that the receiver is attached to
int IRsensorValue = 0;        // value read from the receiver
int touchState = 0;        //Touch Sensor state

HX711 scale(SCALE_DOUT_PIN, SCALE_SCK_PIN);

void setup() {
  servo.attach(D5); //D5
  servo.write(90);
  pinMode(Touch_Button_Pin, INPUT);
  pinMode(G, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(R, OUTPUT);
  digitalWrite(G, LOW);
  digitalWrite(Y, LOW);
  digitalWrite(R, LOW);
  Serial.begin(115200);
  scale.set_scale(1800.00/24.00);
  scale.tare();
}

void loop() {
  senseWeight();
  while (!Serial.available()){}
  int count = Serial.read() - '0';
  if (touchSensor() == 1){  
    dispense(count);
  }
  Serial.flush();
}

void senseWeight(){
  float w = scale.get_units(1); //get weight
  String weight = String(w, 2);
  Serial.println(weight);
  if (w <= 20){
    digitalWrite(G, LOW);
    digitalWrite(Y, LOW);
    digitalWrite(R, HIGH);
    delay(1000);
  }
  else if (w > 20 && w <= 100){
    digitalWrite(G, LOW);
    digitalWrite(Y, HIGH);
    digitalWrite(R, LOW);
    delay(1000);
  }
  else if (w > 100){
    digitalWrite(G, HIGH);
    digitalWrite(Y, LOW);
    digitalWrite(R, LOW);
    delay(1000);
  }
}

void dispense(int count){
  for (int i = 0; i < count; i++){          //dispense specified amount
      for (int i = 90; i > 0; i--){
      servo.write(i);
      delay(10);
      }
      for (int i = 0; i < 180; i++){
        servo.write(i);
        delay(10);
      }
      for (int i = 180; i > 90; i--){
        servo.write(i);
        delay(10);
      }
      if (!pillDispensed){              //If pill not sensed, dispense again
        i--;
      }
    }
}

bool pillDispensed(){
  // read the analog in value:
  IRsensorValue = analogRead(analogInPin);

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

bool touchSensor(){
  return digitalRead(Touch_Button_Pin);         //check if touched
}

