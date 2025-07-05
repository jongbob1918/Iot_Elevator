

/* 변수 설정 (순서도 참고) */
const int totalfloor   = 4;
const int floorHeight  = 3;
int       wantFloor    = 0;
int       nowFloor     = 0;
bool      floorList[]  = {false, false, false, false};
bool      upMode       = false;

unsigned long updateTime = 0;
unsigned long beforeTime = 0;
const int moveTime = 500;

const int floorleds = 10;
const int leds[]      = {2,3,4,5,6,7,8,9,10,11};
const int ledGreens[] = {12,13,A0,A1};
const int buttons[]   = {A2,A3,A4,A5};
int  prevButtons[]    = {LOW, LOW, LOW, LOW};
bool wantButton       = false;

/* 호출 LED 토글 */
void floorSensing(int btn){
  floorList[btn] = !floorList[btn];
  digitalWrite(ledGreens[btn], floorList[btn] ? HIGH : LOW);
}

/* 버튼 감지 */
void buttonPress(){
  wantButton = false;
  for(int i=0;i<totalfloor;i++){
    int cur = digitalRead(buttons[i]);
    if(prevButtons[i]==LOW && cur==HIGH) floorSensing(i);
    prevButtons[i] = cur;
    if(floorList[i]) wantButton = true;
  }
}

/* 아래쪽 탐색 */
int scanBottom(){
  int fl = nowFloor / floorHeight;
  while(fl >= 0){
    if(floorList[fl]) return fl * floorHeight;
    fl--;
  }
  return -1;
}

/* 위쪽 탐색 */
int scanTop(){
  int fl = (nowFloor + floorHeight) / floorHeight;
  while(fl < totalfloor){
    if(floorList[fl]) return fl * floorHeight;
    fl++;
  }
  return -1;
}

/* 다음 목적 층 결정 */
int getWantFloor(int mode){
  int res = -1;
  if(mode == 0){              // 하강 중
    res = scanBottom();
    if(res < 0) res = scanTop();
  }
  else if(mode == 1){         // 상승 중
    res = scanTop();
    if(res < 0) res = scanBottom();
  }
  else {                      // 호출 없음
    res = ((nowFloor + floorHeight - 1)/floorHeight)*floorHeight;
  }
  return res;
}

/* 한 칸 이동 */
void moveElevator(){
  if(nowFloor == wantFloor){
    int callFloor = nowFloor / floorHeight;
    if(floorList[callFloor]) floorSensing(callFloor);
    return;
  }

  if(nowFloor < wantFloor){
    if(nowFloor-1 >= 0) digitalWrite(leds[nowFloor-1], LOW);
    nowFloor++;  upMode = true;
  }else{
    if(nowFloor+1 < floorleds) digitalWrite(leds[nowFloor+1], LOW);
    nowFloor--;  upMode = false;
  }
  digitalWrite(leds[nowFloor], HIGH);
}

/* 초기화 */
void setup(){
  for(int i=0;i<floorleds;i++) pinMode(leds[i], OUTPUT);
  for(int i=0;i<totalfloor;i++){
    pinMode(ledGreens[i], OUTPUT);
    pinMode(buttons[i],   INPUT_PULLUP);
  }
  digitalWrite(leds[nowFloor], HIGH);
}

/* 메인 루프 */
void loop(){
  buttonPress();

  if(!wantButton)  wantFloor = getWantFloor(3);
  else             wantFloor = getWantFloor(upMode ? 1 : 0);

  updateTime = millis();
  if(updateTime - beforeTime > moveTime){
    moveElevator();
    beforeTime = updateTime;
  }else{
    delay(10);
  }
}
