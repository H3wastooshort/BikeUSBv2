void setup() {
  Serial.begin(115200);
  pinMode(7, OUTPUT);
}

union {
  uint16_t i;
  uint8_t b[sizeof(i)];
} val;

uint16_t t = 100;

void loop() {
  val.i = analogRead(A0);
  Serial.write(val.b, sizeof(val.b));

  digitalWrite(7, (millis() % t) > (t / 2));

  if (Serial.available()) {
    switch (Serial.read()) {
      case 't':
        delay(100);
        t = Serial.parseInt();
        break;
      case 'i': analogReference(INTERNAL);
      case 'd': analogReference(DEFAULT);
    }
  }
}
