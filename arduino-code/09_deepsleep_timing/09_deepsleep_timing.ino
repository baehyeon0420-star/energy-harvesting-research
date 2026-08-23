// ESP32 Deep Sleep 사이클 타이밍 측정
// Wake -> 센서 측정(DHT22+토양수분) -> 데이터 처리 -> 전송(시리얼) -> Deep Sleep 반복
// 각 단계 소요 시간을 ms 단위로 측정하여 출력. 전류값은 데이터시트를 인용해 별도로 결합.

#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_PIN 34
#define SLEEP_SECONDS 30   // 테스트용 30초 (실제 운용 시 600초=10분으로 변경)

DHT dht(DHTPIN, DHTTYPE);
RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  delay(200);
  bootCount++;

  unsigned long t_wake_start = millis();

  dht.begin();
  unsigned long t_sensor_start = millis();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soil = analogRead(SOIL_PIN);
  unsigned long t_sensor_end = millis();

  unsigned long t_process_start = millis();
  char buf[100];
  snprintf(buf, sizeof(buf), "습도:%.1f%% 온도:%.1fC 토양:%d", h, t, soil);
  unsigned long t_process_end = millis();

  unsigned long t_tx_start = millis();
  Serial.println(buf);
  unsigned long t_tx_end = millis();

  unsigned long t_total_end = millis();

  Serial.println("---- 타이밍 결과 ----");
  Serial.printf("부팅 횟수: %d\n", bootCount);
  Serial.printf("센서 측정 시간: %lu ms\n", t_sensor_end - t_sensor_start);
  Serial.printf("데이터 처리 시간: %lu ms\n", t_process_end - t_process_start);
  Serial.printf("전송(시리얼) 시간: %lu ms\n", t_tx_end - t_tx_start);
  Serial.printf("전체 Wake 구간 시간: %lu ms\n", t_total_end - t_wake_start);
  Serial.flush();

  esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_SECONDS * 1000000ULL);
  Serial.println("Deep Sleep 진입...");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
  // Deep Sleep에서 깨면 setup()부터 다시 실행되므로 사용 안 함
}
