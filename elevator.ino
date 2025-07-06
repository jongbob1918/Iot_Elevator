//FLOOR
const int totalfloor = 4;      // 층 개수
const int floorHeight = 3;     // 층 길이(자기 포함)
int wantFloor = 0;             // 목적층		
int nowFloor = 0;              // 현재층
bool floorList[] = {false, false, false, false}; // 층 호출 상태
bool upMode = false;           // 오르는 이동
//TIME
unsigned long updateTime = 0;  // 현재시간
unsigned long beforeTime = 0;  // 과거시간
const int moveTime = 500;
//LED
const int floorleds = 10;      // led개수
const int leds[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int ledGreens[] ={12, 13, A0, A1};
//BUTTON
const int buttons[]  = {A2, A3, A4, A5}; 
int prevButtons[] = {LOW, LOW, LOW, LOW}; // 버튼들의 과거 상태
bool wantButton = false;       // 호출한 버튼이 있는지

//핀 설정
void setup()
{
  for (int i = 0; i < floorleds; i++)
  {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  
  for (int i = 0; i < totalfloor; i++)
  {
    pinMode(ledGreens[i], OUTPUT);
    digitalWrite(ledGreens[i], LOW);
  }

  for (int i = 0; i < totalfloor; i++)
  {
   	pinMode(buttons[i], INPUT);
  }
  Serial.begin(9600);
}

// 아래 호출층 스캔 
int scanBottom() 
{
  for (int floor =(nowFloor / floorHeight) ; floor >= 0 ; floor --)
  {
    if (floorList[floor] == true)
    {
      wantFloor = floor * floorHeight ;
      return wantFloor;
    }
  }
  return -1;
}

// 위 호출층 스캔
int scanTop() 
{
  for (int floor = (nowFloor+2) / floorHeight ; floor < totalfloor ; floor++)
  {
    if (floorList[floor] == true)
    {
      wantFloor =  floor * floorHeight;
      return wantFloor;
    }
  }
  return -1;
}
// 호출 층 찾기
int getWantFloor(int mode) 
{
  int result;
  switch(mode) 
  {
    case 0:  
      result = scanBottom();
      if (result < 0) result = scanTop(); // 스캔시 없을때 반대 방향 스캔
      break;
    case 1:  
      result = scanTop();
      if (result < 0) result = scanBottom(); // 스캔시 없을때 반대 방향 스캔
      break;
    default: // 호출 없을 때 가장 가까운 층으로
      wantFloor = ((nowFloor + 1) / floorHeight)* floorHeight;  
      return wantFloor;;
  }
}

void moveElevator() 
{
  // 원하는층이 현재층 같은데 호출이 있으면 꺼주고 아니면 가만히 있기
  if (wantFloor == nowFloor) 
  {
    int callFloor = nowFloor / floorHeight;
    if (floorList[callFloor] == true) 
    {
      floorSensing(callFloor); 
    }
    digitalWrite(leds[nowFloor], HIGH);  
  }
  // 원하는 층이 현재층보다 낮으면 한칸 내려가기
  else if (wantFloor > nowFloor) 
  {
    nowFloor++;
    upMode = true;
    digitalWrite(leds[nowFloor], HIGH);
    digitalWrite(leds[nowFloor - 1], LOW);
  }
  // 원하는 층이 현재층보다 낮으면 한칸 내려가기
  else if (wantFloor < nowFloor) 
  {
    nowFloor--;
    upMode = false;
    digitalWrite(leds[nowFloor], HIGH);
    digitalWrite(leds[nowFloor + 1], LOW);
  }
}


//버튼 눌렀을때 해당 층 호출 및 점등 
bool floorSensing(int button)
{
  floorList[button]= !floorList[button];
  digitalWrite(ledGreens[button], floorList[button] ? HIGH : LOW);
  Serial.println("Press button");
  return floorList[button];
}


// 버튼 눌렸는지 
void buttonPress()
{
 for (int button = 0; button < totalfloor; button++) 
 {
  int current = digitalRead(buttons[button]);
  if (prevButtons[button] == LOW && current == HIGH)
  {
	floorSensing(button);
  }
  prevButtons[button] = current;
  if (floorList[button]) 
   {
     wantButton = true;
   }
 }
}

/// 메인 함수
void loop()
{
  // 버튼 누른지 확인
  wantButton = false;  
  buttonPress();
  
  // 호출한 버튼이 없으면 근처 층으로 가기
  if(!wantButton)
  {
  	wantFloor = getWantFloor(3);
  }
  //호출층찾기
  else{
  wantFloor = getWantFloor(upMode);  
  }
  // 시간 확인
  updateTime = millis();
  
  if((updateTime- beforeTime) > moveTime) // 0.5초 대기
  {
	moveElevator();

    // 시간 초기화
    beforeTime = updateTime;
    Serial.print(" 목표층 :");
    Serial.print(wantFloor);
    Serial.print(" 현재층 :");
    Serial.println(nowFloor);
  }
  delay(10);
}