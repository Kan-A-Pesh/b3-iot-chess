#include <Arduino.h>

#define ALIVE_PIN_LED 5

// Multiplexer pins
#define MLPLX_S0_PIN 10
#define MLPLX_S1_PIN 11
#define MLPLX_S2_PIN 12
#define MLPLX_S3_PIN 13

#define MLPLX0_ENABLE_PIN 18
#define MLPLX_LENGTH 4
// MLPLX1, ..., MLPLXn will be defined as
// MLPLX0_ENABLE_PIN + 1, ..., MLPLX0_ENABLE_PIN + MLPLX_LENGTH - 1

#define MLPLX_SIGNAL_PIN 26

void setup() {
  Serial.begin(9600);

  pinMode(ALIVE_PIN_LED, OUTPUT);

  pinMode(MLPLX_S0_PIN, OUTPUT);
  pinMode(MLPLX_S1_PIN, OUTPUT);
  pinMode(MLPLX_S2_PIN, OUTPUT);
  pinMode(MLPLX_S3_PIN, OUTPUT);

  pinMode(MLPLX0_ENABLE_PIN, OUTPUT);

  for (int i = 0; i < MLPLX_LENGTH; i++) {
    pinMode(MLPLX0_ENABLE_PIN + i, OUTPUT);
  }

  pinMode(MLPLX_SIGNAL_PIN, INPUT);
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

//#region Multiplexer
int mlplx_read_channel(int channel) {
  // Get the multiplexer enable pin
  short enable_pin_offset = channel / 16;
  digitalWrite(MLPLX0_ENABLE_PIN + enable_pin_offset, HIGH);

  // shift the channel number to get the pins matrix (from 0 to 15)
  // Get the actual channel number (0-15) within the current multiplexer
  int actual_channel = channel % 16;
  digitalWrite(MLPLX_S0_PIN, bitRead(actual_channel, 0));
  digitalWrite(MLPLX_S1_PIN, bitRead(actual_channel, 1));
  digitalWrite(MLPLX_S2_PIN, bitRead(actual_channel, 2));
  digitalWrite(MLPLX_S3_PIN, bitRead(actual_channel, 3));

  // Serial.print("Stats for ");
  // Serial.print(String(channel));
  // Serial.print(": ");

  // Serial.print(String(MLPLX0_ENABLE_PIN + enable_pin_offset));
  // Serial.print(" ");
  // Serial.print(bitRead(actual_channel, 0));
  // Serial.print(bitRead(actual_channel, 1));
  // Serial.print(bitRead(actual_channel, 2));
  // Serial.print(bitRead(actual_channel, 3));
  // Serial.print(" ");
  // Serial.println(digitalRead(MLPLX_SIGNAL_PIN));

  // Read the value from the signal pin
  float value = digitalRead(MLPLX_SIGNAL_PIN);

  digitalWrite(MLPLX0_ENABLE_PIN + enable_pin_offset, LOW);
  digitalWrite(MLPLX_S0_PIN, LOW);
  digitalWrite(MLPLX_S1_PIN, LOW);
  digitalWrite(MLPLX_S2_PIN, LOW);
  digitalWrite(MLPLX_S3_PIN, LOW);

  // Parse value to X.XXX
  return value;
}
//#endregion

void loop() {
  // Update clock
  clock_counter++;
  delay(10);

  // Toggle LED
  check_alive_led();

  // Read all 64 channels
  if (is_clock_time(1000)) {
  Serial.print("Clock: ");
  for (int i = 0; i < 64; i++) {
    Serial.print(String(mlplx_read_channel(i)));
  }

    Serial.println();
  }
}