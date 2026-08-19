// 슈퍼커패시터(SC5R5105ZC, 5.5V/1.0F) 방전 특성 측정
// 회로: 충전된 슈퍼커패시터(+) → INA219 Vin+ → [INA219] → Vin- → 저항(부하) → 슈퍼커패시터(-)
// 사용법: 슈퍼커패시터를 충분히 충전한 뒤(07_supercap_charge 실험 참고),
//         전원부를 분리하고 부하 저항을 연결한 상태로 업로드 직후 방전을 시작한다.
//         시리얼 모니터(115200bps) 출력을 CSV로 복사해 시간-전압-전류 그래프 작성에 사용.

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
bool found = false;

const unsigned long SAMPLE_INTERVAL_MS = 1000;  // 측정 주기
unsigned long startTime = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  found = ina219.begin();

  if (!found) {
    Serial.println("INA219를 찾을 수 없습니다.");
    return;
  }

  Serial.println("방전 측정 시작");
  startTime = millis();
}

void loop() {
  if (!found) {
    delay(1000);
    return;
  }

  float busVoltage = ina219.getBusVoltage_V();
  float shuntVoltage_mV = ina219.getShuntVoltage_mV();
  float loadVoltage = busVoltage + (shuntVoltage_mV / 1000.0);
  float current_mA = ina219.getCurrent_mA();

  float elapsed_s = (millis() - startTime) / 1000.0;

  Serial.print("시간: "); Serial.print(elapsed_s, 1);
  Serial.print("s | 전압: "); Serial.print(loadVoltage, 3);
  Serial.print("V | 전류: "); Serial.print(current_mA, 3);
  Serial.println("mA");

  delay(SAMPLE_INTERVAL_MS);
}
