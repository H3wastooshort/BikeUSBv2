// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  analogReference(INTERNAL);
  pinMode(7, OUTPUT);
}

// the loop routine runs over and over again forever:
uint16_t t = 100;
void loop() {
  uint16_t val = analogRead(A0);
  if (val < 1000) Serial.print(0);
  if (val < 100) Serial.print(0);
  if (val < 10) Serial.print(0);
  Serial.println(val);
  Serial.flush();
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
