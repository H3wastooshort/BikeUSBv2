void btn1_isr() {
}

void btn2_isr() {
}

void config_setup() {
  pwr_led.setStatic(false);
  usr_led.setStatic(true);

  attachInterrupt(BTN1_PIN, btn1_isr, FALLING);
  attachInterrupt(BTN2_PIN, btn2_isr, FALLING);
}

void config_loop() {  //runs when in config mode
}