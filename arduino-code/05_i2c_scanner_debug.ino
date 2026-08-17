#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(500);
  Serial.println("I2C 스캔 시작");
}

void loop() {
  byte count = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("발견된 주소: 0x");
      Serial.println(addr, HEX);
      count++;
    }
  }
  if (count == 0) Serial.println("아무 장치도 못 찾음");
  delay(3000);
}
