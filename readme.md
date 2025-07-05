# Arduino Elevator Controller  
4 층(확장 가능) LED 엘리베이터 │ **One-Day Project**

[![Arduino](https://img.shields.io/badge/Arduino-Uno-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/)


> **목표** – 버튼·LED만으로 엘리베이터의 호출, 이동, 대기 로직을 시뮬레이션하는 간단한 FSM(Finite-State Machine) 예제 프로젝트입니다.  
> 핀 매핑만 바꾸면 층수·속도를 손쉽게 확장할 수 있습니다. 

## 발표자료
[![발표자료]([https://img.shields.io/badge/Arduino-Uno-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/](https://docs.google.com/presentation/d/1m6TEW22ZXlsffNen36meO2qcAVfPnEL0svScLSEzju0/edit?usp=sharing))
</br>
[![tinkercad]([https://img.shields.io/badge/Arduino-Uno-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/](https://www.tinkercad.com/things/1Y2Mx1cmY9a-elevatorled
))

---

## 목차
1. [요구 사항](#요구-사항)  
2. [주요 기능](#주요-기능)  
3. [하드웨어 구성](#하드웨어-구성)  
4. [코드 구조](#코드-구조)  
5. [빌드 & 업로드](#빌드--업로드)  
6. [사용법](#사용법)  
7. [테스트 케이스](#테스트-케이스)  
8. [확장 가이드](#확장-가이드)  

---

## 주요 기능
### 표시
1. 시스템 시작 시 1 층 대기  
2. 현재 층 RED LED 점등  
3. 이동 중 YELLOW LED 시퀀셜 표시  
4. 호출 버튼 ON/OFF 시 GREEN LED 점멸 
### 이동
- 0.5 초 주기로 한 LED(=한 칸)씩 이동  
- **상승**·**하강**·**정지** 3 상태 FSM  
- 호출이 없으면 가까운 층에서 정지 대기  

### 호출 우선순위
- **상승/정지 중** → 위쪽 최단거리 ⟶ 위쪽 전체 ⟶ 아래쪽  
- **하강 중** → 아래쪽 최단거리 ⟶ 아래쪽 전체 ⟶ 위쪽
---

## 하드웨어 구성
| 분류 | 수량 | 연결 핀 |
|------|------|---------|
| RED LED (층 표시)      | 4  | `2, 5, 8, 11` *(예시)* |
| YELLOW LED (층간)      | 6  | `3,4,6,7,9,10` |
| GREEN LED (호출 상태)  | 4  | `12, 13, A0, A1` |
| 버튼 (호출)            | 4  | `A2, A3, A4, A5` |
| Arduino Uno / Nano     | 1  | 5 V 공급 |

> **핀 번호는 코드의 배열(`leds`, `ledGreens`, `buttons`)만 수정하면 자유롭게 변경할 수 있습니다.**

---

## 코드 구조
elevator/
├── elevator.ino # 메인 스케치 (FSM / 루프)
└── README.md

핵심 상수·변수는 **순서도와 동일한 이름**으로 선언되어 유지보수성과 가독성을 높였습니다.  
- `buttonPress()`  : 버튼 → 호출 토글  
- `floorSensing()` : GREEN LED ON/OFF  
- `scanTop() / scanBottom()` : 위·아래 호출 탐색  
- `getWantFloor()` : 우선순위 결정  
- `moveElevator()` : 한 칸 이동 및 도착 처리  

> 전체 함수 호출 흐름은 `loop()` → 버튼 감지 → 목적지 결정 → 0.5 초 타이머 → 이동 순으로 진행됩니다. 

---

## 빌드 & 업로드
1. **Arduino IDE** 또는 **arduino-cli** 설치  
2. 보드 선택 → 시리얼 포트 선택  
3. `elevator.ino` 열기 후 **업로드(▶) 클릭**  
4. LED·버튼을 회로에 연결하고 동작 확인

---

## 사용법
| 동작 | 설명 |
|------|------|
| 버튼 1회 클릭 | 해당 층 호출 (GREEN LED ON) |
| 동일 버튼 재클릭 | 호출 취소 (GREEN LED OFF) |
| 이동 중 추가 호출 | 우선순위 규칙에 따라 자동 반영 |
| 모든 호출 해소 | 가장 가까운 층에서 정지 대기 |

---

## 테스트 케이스
| ID | 시나리오 | 예상 결과 |
|----|----------|-----------|
| TC-01 | 2 층 단일 호출 | 2 층 도착 후 GREEN LED 소등 |
| TC-02 | 3 층→2 층 순 호출 | 2 층→3 층 순서 이동 |
| TC-03 | 이동 중 2 층 추가 호출 | 경로 재계산 후 2 층 경유 |
| TC-04 | 호출 후 재클릭 취소 | 즉시 GREEN LED 소등·목록 제거 |
| TC-05 | 4 층 호출→취소→2 층 호출 | 2 층만 이동 |
| TC-06 | 빠른 다중 버튼 입력 | 모든 호출 정상 등록 |
| TC-07 | 호출 없음 | 근접층 대기 |
| TC-08 | 모든 호출 처리 후 | 최종 위치에서 정지 |

---

## 확장 가이드
| 확장 항목 | 방법 |
|-----------|------|
| **층 추가** | `totalfloor`, `floorHeight`, LED/버튼 배열 길이 확대 |
| **속도 변경** | `moveTime` (ms) 값 수정 |
| **모터 제어** | `moveElevator()` 내부에 서보·모터 코드 삽입 |
| **외부 표시** | 시리얼 출력·LCD·OLED 등 추가 가능 |

---

> 자유롭게 포크·수정하여 사용하시되, 원저작자 표시를 남겨 주세요.
