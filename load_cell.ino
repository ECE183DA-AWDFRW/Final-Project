#include <HX711.h>

#define G D2
#define Y D3
#define R D4
// Scale Settings
const int SCALE_DOUT_PIN = D0;
const int SCALE_SCK_PIN = D1;


HX711 scale(SCALE_DOUT_PIN, SCALE_SCK_PIN);

void setup() {
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
  float w = scale.get_units(1);
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
