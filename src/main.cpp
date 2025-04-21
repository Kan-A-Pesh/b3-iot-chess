#include <Arduino.h>

#define ALIVE_PIN_LED 0

#define REED_SWITCH_TEST_PIN 16

void setup() {
  Serial.begin(9600);

  pinMode(ALIVE_PIN_LED, OUTPUT);
  pinMode(REED_SWITCH_TEST_PIN, INPUT);
}

//#region Clock
long clock_counter = 0;

bool is_clock_time(int ms_interval) {
  return clock_counter % (ms_interval / 10) == 0;
}
//#endregion

//#region Alive LED
bool led_status = false;

void check_alive_led() {
  if (is_clock_time(1000)) {
    led_status = !led_status;
    digitalWrite(ALIVE_PIN_LED, led_status);
  }
}
//#endregion

void loop() {
  // Update clock
  clock_counter++;
  delay(10);

  // Toggle LED
  check_alive_led();

  Serial.println(digitalRead(REED_SWITCH_TEST_PIN));
}