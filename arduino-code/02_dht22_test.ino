#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT22 읽기 실패");
  } else {
    Serial.print("습도: "); Serial.print(h);
    Serial.print(" %  온도: "); Serial.print(t);
    Serial.println(" C");
  }
  delay(2000);
}
