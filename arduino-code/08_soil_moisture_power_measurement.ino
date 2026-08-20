// 정전식 토양수분센서 소비전류 측정 (대기 상태 vs 측정 순간)
// 회로: +레일 -> INA219 Vin+ -> [INA219] -> Vin- -> 토양수분센서 VCC
//       토양수분센서 GND, OUT(GPIO34)는 기존과 동일하게 연결

#include <Wire.h>
#include <Adafruit_INA219.h>

#define SOIL_PIN 34
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(115200);
  delay(500);
  ina219.begin();
}

void loop() {
  float idle_mA = ina219.getCurrent_mA();
  int raw = analogRead(SOIL_PIN);
  float active_mA = ina219.getCurrent_mA();

  Serial.print("대기전류: "); Serial.print(idle_mA);
  Serial.print(" mA | 측정순간 전류: "); Serial.print(active_mA);
  Serial.print(" mA | 토양수분 raw: "); Serial.println(raw);

  delay(2000);
}
