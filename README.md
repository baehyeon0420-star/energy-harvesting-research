# Energy Harvesting Research Project

자연광 에너지 하베스팅 기반 슈퍼커패시터 저전력 IoT 시스템의 실증적 에너지 효율 분석

## 개요

실내 창가 자연광을 이용한 태양광 에너지 하베스팅 시스템에서, **슈퍼커패시터 기반 저장·구동 시스템의 에너지 효율 병목 지점을 실측 기반으로 분석**하는 연구. 태양광 발전 특성, 저장소자(슈퍼커패시터) 충·방전 특성, 센서/MCU 소비전력을 각각 실측하여, 시스템 전체에서 에너지가 실제로 어디서 소모·손실되는지 규명하고 저전력 설계 개선 방향을 제시하는 것을 목표로 한다.

> **연구 범위 조정 안내(2026-08-21)**: 초기 계획에는 충전식 리튬코인셀(LIR2032)과의 비교도 포함되어 있었으나, 충전모듈(TP4056) 하드웨어 이슈로 정식 충·방전 실험 진행이 어려워짐에 따라 연구 범위를 **슈퍼커패시터 중심의 에너지 효율 분석**으로 조정함. LIR2032 초기전압 측정 등 확보된 데이터는 참고자료로 유지하되, 정식 배터리 비교는 후속 과제로 남겨둠. 자세한 경위는 [`experiment-logs/`](./experiment-logs) 참고.

## 하드웨어 구성

| 구분 | 부품 |
|---|---|
| 제어부 | ESP32 DevKitC (ESP32-WROOM-32E) |
| 발전부 | Small Solar Panel 80x100mm 1W |
| 저장부 | 슈퍼커패시터 SC5R5105ZC (5.5V/1.0F) |
| 센서부 | DHT22(AM2302) 온습도센서, 정전식 토양수분센서 |
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

## 진행 상황

- [x] 측정 시스템 구축 (ESP32 / DHT22 / 토양수분센서 / INA219 개별 검증)
- [x] 태양전지 발전 특성 측정 (예비실험)
- [x] 슈퍼커패시터 충전 특성 측정
- [x] 슈퍼커패시터 방전 특성 측정
- [x] DHT22 소비전류 측정 (대기 vs 측정 순간)
- [x] 토양수분센서 소비전류 측정 (대기 vs 측정 순간)
- [x] ESP32 Deep Sleep 사이클 타이밍 측정 (전류는 데이터시트 인용)
- [x] LIR2032 초기전압 확인 (참고자료, 정식 실험은 범위 외로 조정)
- [x] 슈퍼커패시터 자가방전 측정 (0~48시간, 전압유지율 97.9%)
- [x] 슈퍼커패시터 내부저항(ESR) 측정 (13회 반복, 약 3.5~4Ω)
- [x] 하루 에너지 수지 모델 및 흐린 날 생존기간 추정 (최적화 전후 비교)
- [ ] 통합 시스템 실험 및 흐린 날 장기 운용 평가 (실측 검증)

자세한 측정 데이터와 해석은 [`experiment-logs/`](./experiment-logs) 폴더 참고.

## 아두이노 코드

| 파일 | 설명 |
|---|---|
| `01_connection_test.ino` | ESP32-PC 연결 확인 (시리얼 출력) |
| `02_dht22_test.ino` | DHT22 온습도센서 테스트 |
| `03_soil_moisture_test.ino` | 정전식 토양수분센서 테스트 |
| `04_ina219_test.ino` | INA219 전압/전류 측정 (태양전지·커패시터 실험 공용) |
| `05_i2c_scanner_debug.ino` | I2C 장치 인식 문제 진단용 스캐너 |
| `06_supercap_discharge_test.ino` | 슈퍼커패시터 방전 특성 측정 (시간별 전압/전류 CSV 출력) |
| `07_dht22_power_measurement.ino` | DHT22 소비전류 측정 (대기 vs 측정 순간) |
| `08_soil_moisture_power_measurement.ino` | 토양수분센서 소비전류 측정 (대기 vs 측정 순간) |
| `09_deepsleep_timing.ino` | ESP32 Deep Sleep 사이클 단계별 타이밍 측정 |
