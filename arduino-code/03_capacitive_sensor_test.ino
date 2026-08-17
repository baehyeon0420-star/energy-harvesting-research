#define CAP_SENSOR_PIN 34

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(CAP_SENSOR_PIN);
  Serial.print("센서 raw 값: ");
  Serial.println(raw);
  delay(1000);
}
