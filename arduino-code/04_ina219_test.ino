#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
bool found = false;

void setup() {
  Serial.begin(115200);
  delay(500);
  found = ina219.begin();
}

void loop() {
  if (!found) {
    Serial.println("INA219를 찾을 수 없습니다.");
    delay(1000);
    return;
  }
  float busVoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  Serial.print("전압: "); Serial.print(busVoltage);
  Serial.print(" V, 전류: "); Serial.print(current_mA);
  Serial.println(" mA");
  delay(1000);
}
