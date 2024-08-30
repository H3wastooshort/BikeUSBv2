void setup() {
  Serial.begin(115200);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);


  noInterrupts();
  // Clear registers
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  // 10000 Hz (16000000/((24+1)*64))
  OCR2A = 24;
  // CTC
  TCCR2A |= (1 << WGM21);
  // Prescaler 64
  TCCR2B |= (1 << CS22);
  // Output Compare Match A Interrupt Enable
  TIMSK2 |= (1 << OCIE2A);
  interrupts();
}

uint16_t divider = 9999;
uint16_t counter = 0;
bool state = 0;
ISR(TIMER2_COMPA_vect) {
  counter++;
  if (counter > divider) {
    digitalWrite(7, state);
    digitalWrite(6, !state);
    state ^= 1;
    counter = 0;
  }
}

union {
  uint16_t i;
  uint8_t b[sizeof(i)];
} val;

void loop() {
  val.i = analogRead(A0);
  Serial.write(val.b, sizeof(val.b));

  if (Serial.available()) {
    switch (Serial.read()) {
      case 'F':
        delay(100);
        divider = Serial.parseInt();
        break;
      case 'i': analogReference(INTERNAL);
      case 'd': analogReference(DEFAULT);
    }
  }
}
