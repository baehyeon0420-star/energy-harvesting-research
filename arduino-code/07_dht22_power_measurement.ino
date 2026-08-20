// DHT22 소비전류 측정 (대기 상태 vs 측정 순간)
// 회로: +레일 -> INA219 Vin+ -> [INA219] -> Vin- -> DHT22 VCC("+")
//       DHT22 GND, DATA(GPIO4)는 기존과 동일하게 연결

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(115200);
  delay(500);
  dht.begin();
  ina219.begin();
}

void loop() {
  float idle_mA = ina219.getCurrent_mA();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  float active_mA = ina219.getCurrent_mA();

  Serial.print("대기전류: "); Serial.print(idle_mA);
  Serial.print(" mA | 측정순간 전류: "); Serial.print(active_mA);
  Serial.print(" mA | 습도: "); Serial.print(h);
  Serial.print(" %  온도: "); Serial.print(t);
  Serial.println(" C");

  delay(2000);
}
