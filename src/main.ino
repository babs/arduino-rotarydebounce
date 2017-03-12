
#define BTN 2
#define ROT_A 3
#define ROT_B 4


long lastencpos = 0;
volatile long encpos = 0;
volatile short lastRotB = 0;
volatile short curRotB = 0;
volatile unsigned long lastIntRotAFallChange = 0;
volatile unsigned long lastIntDelta = 0;



void setup(void) {
  Serial.begin(115200);
  Serial.println("Boot.");

  // All 3 pins as input with pull ups
  pinMode(BTN, INPUT);
  digitalWrite(BTN, HIGH);
  pinMode(ROT_A, INPUT);
  digitalWrite(ROT_A, HIGH);
  pinMode(ROT_B, INPUT);
  digitalWrite(ROT_B, HIGH);

  attachInterrupt(digitalPinToInterrupt(BTN), btnpress, FALLING);
  attachInterrupt(digitalPinToInterrupt(ROT_A), intRotAFall, FALLING);
}

void btnpress() {
  if (digitalRead(BTN) == 1) { // Bounce
    return;
  }
  encpos=0;
}


void intRotAFall() {
  lastIntDelta = millis() - lastIntRotAFallChange;
  curRotB = digitalRead(ROT_B);
   // Debounce
  if (lastIntDelta < 10) {
    return;
  }
  // Debounce "over bounce" avoid +1 / -1 on low cost encoders
  if (curRotB != lastRotB && lastIntDelta < 100) {
    return;
  }
  if (curRotB) {
    encpos++;
  } else {
    encpos--;
  }
  //Serial.print(".");
  lastRotB = curRotB;
  lastIntRotAFallChange = millis();
}

short CUR_A, CUR_B, PREV_A, PREV_B;
void show() {
  CUR_A = digitalRead(ROT_A);
  CUR_B = digitalRead(ROT_B);

  if (CUR_A == PREV_A && CUR_B == PREV_B) {
    return;
  }

  Serial.print("BTN=");
  Serial.print(digitalRead(BTN));
  Serial.print("\tA=");
  Serial.print(digitalRead(ROT_A));
  Serial.print("\tB=");
  Serial.print(digitalRead(ROT_B));
  Serial.print("\trots=");
  Serial.print(encpos);
  Serial.print("\r\n");
  PREV_A = CUR_A;
  PREV_B = CUR_B;
}

void loop() {
  if (lastencpos != encpos) {
    lastencpos = encpos;
    Serial.println(encpos);
  }
}
