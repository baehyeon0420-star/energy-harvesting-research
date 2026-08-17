#define SOIL_PIN 34

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(SOIL_PIN);
  Serial.print("토양수분 raw 값: ");
  Serial.println(raw);
  delay(1000);
}
