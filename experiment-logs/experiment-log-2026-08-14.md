# 자연광 에너지 하베스팅 기반 저전력 환경 모니터링 시스템 — 실험 기록

**작성일**: 2026-08-17
**대상 연구계획서**: 자연광 에너지 하베스팅 기반 슈퍼커패시터 및 충전식 배터리의 저장 특성 비교 및 저전력 환경 모니터링 시스템 적용 가능성 평가

---

## 0. 전체 진행 상황 요약

| 단계 | 항목 | 상태 |
|---|---|---|
| 0 | 측정 시스템 구축 (ESP32/DHT22/토양수분센서/INA219 개별 검증) | ✅ 완료 |
| 1 | 태양전지 발전 특성 측정 (3.3절) | ✅ 예비실험 완료 (흐린 날) |
| 2 | 슈퍼커패시터 충전 특성 측정 (3.4절) | ✅ 완료 |
| 3 | 슈퍼커패시터 방전 특성 측정 | ⬜ 예정 |
| 4 | LIR2032 충·방전 특성 측정 (4.5~4.6절) | ⬜ 미착수 |
| 5 | 내부저항/ESR, 자가방전 측정 | ⬜ 미착수 |
| 6 | ESP32 소비전력 측정 (4.9절) | ⬜ 미착수 |
| 7 | 통합 시스템 실험 (4.10절) | ⬜ 미착수 |

---

## 1. 개발 환경 구축

- Arduino IDE 2.3.10 설치
- ESP32 보드 패키지 추가 (Boards Manager URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`)
- 보드: ESP32 Dev Module (HG-ESP32-DEVKIT V4, ESP32-WROOM-32E)
- 연결: USB-C(C to C) 케이블

---

## 2. ESP32-컴퓨터 연결 확인

Blink 예제에서 `LED_BUILTIN` 미정의 컴파일 에러 발생 → `#define LED_BUILTIN 2`로 해결. 이후 보드에 사용자 LED가 없는 것으로 판단되어 시리얼 출력 방식으로 검증 전환.

```cpp
void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("연결 확인 중...");
  delay(1000);
}
```

**결과**: 1초 간격 정상 출력 확인.

---

## 3. DHT22 (온습도센서) 테스트

- 라이브러리: Adafruit DHT sensor library v1.4.7
- 배선: DATA → GPIO4, VCC → 3.3V, GND → GND

```cpp
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
```

**결과**: 습도 32.5~32.6%, 온도 29.1℃ 안정적 측정.

---

## 4. 정전식 토양수분센서 테스트

- 배선: OUT → GPIO34(ADC), VCC → 3.3V, GND → GND

```cpp
#define SOIL_PIN 34
void setup() { Serial.begin(115200); }
void loop() {
  int raw = analogRead(SOIL_PIN);
  Serial.print("토양수분 raw 값: ");
  Serial.println(raw);
  delay(1000);
}
```

**결과**: 건조 상태 raw≈3300~3400, 손으로 파지 시 raw≈2500 → 자극 반응 확인.

---

## 5. INA219 (전류센서) 테스트

- 라이브러리: Adafruit INA219 v1.2.3
- 배선: SDA → GPIO21, SCL → GPIO22, VCC → 3.3V, GND → GND

```cpp
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
```

**I2C 스캐너 (진단용, 문제 발생 시 사용)**:
```cpp
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
```

**결과**: 전압/전류 정상 출력, 통신 정상 확인.

### 트러블슈팅 메모
- 최초 I2C 미검출 원인: DHT22를 INA219로 착각하고 테스트했던 것으로 판명
- 이후 전압이 8.10V/0mA처럼 "전압은 있는데 전류가 0"으로 뜨는 현상 반복 발생 → **회로가 물리적으로 열려있다는 신호**(개방회로)였음. 원인은 대부분 헐거운 점퍼선 접촉, 또는 브레드보드 A~E/F~J 블록을 혼동해서 다른 블록에 연결한 경우였음

---

## 6. 태양전지 발전 특성 측정 (연구계획서 3.3절)

- 회로: `태양전지(+) → INA219 Vin+ → [INA219] → Vin- → 저항 220Ω → 태양전지(-)`
- 측정일: 2026-08-17, 날씨: 흐림

### 결과
- **흐린 날 창가 자연광 + 220Ω 부하**: 전압 0.00V, 전류 0.00mA (검출 안 됨)
- **스마트폰 손전등 대조실험** (약 5cm 거리):

| 조사 조건 | 전압(V) | 전류(mA) |
|---|---|---|
| 근접 조사 (~5cm) | 0.00~0.02 | 0.90 (최댓값) |
| 거리 이격 | 0.00~0.02 | 0.40~0.60 |

### 해석
흐린 날 조건에서는 220Ω 부하 기준 유효 발전량이 관측되지 않았으나, 손전등 대조실험으로 광원 거리(광량)에 비례해 전류가 반응함을 확인 → **회로 및 태양전지 자체는 정상 작동**. 이는 발전량 부족 시 에너지 저장소자의 필요성을 뒷받침하는 예비 관찰로 볼 수 있음. 맑은 날 재측정 및 무부하 개방전압(Voc) 측정 추후 필요.

---

## 7. 슈퍼커패시터 충전 특성 측정 (연구계획서 3.4절)

- 부품: SC5R5105ZC (5.5V / 1.0F)
- 회로: `3.3V(ESP32) → INA219 Vin+ → [INA219] → Vin- → 저항 220Ω → 슈퍼커패시터(+) / 슈퍼커패시터(-) → GND`
- 측정일: 2026-08-17

### 결과

| 경과시간 | 충전전류 |
|---|---|
| 0분 | 14.20 mA |
| 1분 | 9.50 mA |
| 2분 | 7.30 mA |
| 3분 | 6.15 mA |
| 5분 | 4.20 mA |
| 10분 | 2.00 mA |
| 14분 | 0.95 mA |
| 15분 | 0.61 mA |
| 20분 | 0.34 mA |

### 해석
- 초기 전류(14.20mA)는 이론값(3.3V ÷ 220Ω ≈ 15mA)과 근접
- 시간에 따라 지수적으로 감소하는 전형적 RC 충전 곡선 형태
- 20분 시점 전류(0.34mA)가 무부하 노이즈 수준(~0.2~0.3mA)에 근접 → **사실상 충전 완료로 판단**
- 실측 시정수(τ)가 이론값(RC=220초≈3.7분)보다 큰 것은 점퍼선 접촉저항 등 부가 저항 성분에 기인한 것으로 추정

### 결론
슈퍼커패시터는 220Ω 부하 기준 약 20분 이내에 충전 완료. 다음 단계로 방전 특성 측정을 통해 저장된 에너지의 실사용 가능성을 확인할 예정.

---

## 8. 다음 계획

1. 슈퍼커패시터 방전 특성 측정 (저항 부하 연결, 시간별 전압 감소 기록)
2. LIR2032 충·방전 특성 측정 (보호회로 경유 충전 필수)
3. 내부저항/ESR 비교, 자가방전 측정 (장시간 소요 — 가능한 일찍 착수 권장)
4. ESP32 소비전력 측정 (Deep Sleep 상태별)
5. 전체 통합 시스템 구성 및 흐린 날 장기 운용 평가

## 9. 참고 — 최종 발표/논문용 정리 방향
- 브레드보드 배선 자체를 "제품처럼" 정리할 필요는 없음
- 논문에는 브레드보드 사진 대신 **회로도(schematic)**로 정리 예정
- 대회/외부 설명용으로는 통합 시스템이 실제로 작동하는 **짧은 데모 영상** 확보가 효과적
