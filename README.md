# Energy Harvesting Research Project

자연광 에너지 하베스팅 기반 슈퍼커패시터 저전력 IoT 시스템의 실증적 에너지 효율 분석

## 개요

실내 창가 자연광을 이용한 태양광 에너지 하베스팅 시스템에서, **슈퍼커패시터 기반 저장·구동 시스템의 에너지 효율 병목 지점을 실측 기반으로 분석**하는 연구. 태양광 발전 특성, 저장소자(슈퍼커패시터) 충·방전 특성, 센서/MCU 소비전력을 각각 실측하여, 시스템 전체에서 에너지가 실제로 어디서 소모·손실되는지 규명하고 저전력 설계 개선 방향을 제시하는 것을 목표로 한다.

📊 **[전체 데이터 시각화 리포트 보기](https://claude.ai/code/artifact/f93994a1-ef0f-4203-b03a-22405840afa6)** — 태양광/슈퍼커패시터/소비전력/에너지 수지 모델을 그래프로 정리하고, 트러블슈팅 기록까지 포함한 포트폴리오 페이지

## 하드웨어 구성

| 구분 | 부품 |
|---|---|
| 제어부 | ESP32 DevKitC (ESP32-WROOM-32E) |
| 발전부 | Small Solar Panel 80x100mm 1W |
| 저장부 | 슈퍼커패시터 SC5R5105ZC (5.5V/1.0F) |
| 센서부 | DHT22(AM2302) 온습도센서, 정전용량식 센서(상시 구동형) |
| 측정부 | INA219 전류/전압 센서 |
| 기타 | DC-DC 스텝업/다운 모듈, SR540 다이오드, 브레드보드 |
| (참고, 범위 외) | LIR2032 (3.6V/40mAh) — 초기전압만 측정, 정식 실험은 후속 과제 |

## 개발 환경

- Arduino IDE 2.3.10
- ESP32 board package (espressif/arduino-esp32)
- 라이브러리: Adafruit INA219, Adafruit DHT sensor library, Adafruit Unified Sensor

## 폴더 구조

```
energy-harvesting-research/
├── README.md
├── LICENSE
├── docs/                  # 회로도 등 참고 자료
├── experiment-logs/       # 실험 기록 (측정 데이터, 결과 해석)
└── arduino-code/          # 각 단계별 테스트/측정 코드
```

## 회로도

전체 시스템 개념도 + 슈퍼커패시터 충전/방전 측정 회로:

![circuit diagram](./docs/circuit-diagram.png)

## 실험 기록

자세한 측정 데이터와 해석은 [`experiment-logs/`](./experiment-logs) 폴더 참고.

## 아두이노 코드

| 파일 | 설명 |
|---|---|
| `01_connection_test.ino` | ESP32-PC 연결 확인 (시리얼 출력) |
| `02_dht22_test.ino` | DHT22 온습도센서 테스트 |
| `03_capacitive_sensor_test.ino` | 정전용량식 센서(상시 구동형) 테스트 |
| `04_ina219_test.ino` | INA219 전압/전류 측정 (태양전지·커패시터 실험 공용) |
| `05_i2c_scanner_debug.ino` | I2C 장치 인식 문제 진단용 스캐너 |
| `06_supercap_discharge_test.ino` | 슈퍼커패시터 방전 특성 측정 (시간별 전압/전류 CSV 출력) |
| `07_dht22_power_measurement.ino` | DHT22 소비전류 측정 (대기 vs 측정 순간) |
| `08_capacitive_sensor_power_measurement.ino` | 정전용량식 센서 소비전류 측정 (대기 vs 측정 순간) |
| `09_deepsleep_timing.ino` | ESP32 Deep Sleep 사이클 단계별 타이밍 측정 |
