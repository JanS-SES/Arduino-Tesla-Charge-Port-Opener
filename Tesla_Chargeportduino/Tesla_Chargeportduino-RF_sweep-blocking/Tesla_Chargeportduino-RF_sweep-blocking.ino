/*
  Arduino Tesla Charge Port Opener - blocking sweep version

  Sends the captured OOK/ASK bitstream several times while sweeping the pulse
  length from 390 us to 410 us. The sweep avoids hand-calibrating a single
  timing value for every Arduino board and cheap 433 MHz transmitter module.

  The transmitter data pin is active high.
*/

const unsigned int PULSE_LENGTH_MIN_US = 390;
const unsigned int PULSE_LENGTH_MAX_US = 410;
const unsigned int PULSE_LENGTH_STEP_US = 5;
const unsigned int TRANSMISSION_DELAY_MS = 100;

const byte TX_PIN = 2;
const byte PUSH_BUTTON_PIN = 0;
const byte BUTTON_GND_PIN = 4;

bool startTransmission = false;
bool transmitOnStartup = true;

const uint8_t DATA[] = {
  0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1
};

const size_t DATA_LENGTH = sizeof(DATA) / sizeof(DATA[0]);

void setup() {
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW);

  pinMode(BUTTON_GND_PIN, OUTPUT);
  digitalWrite(BUTTON_GND_PIN, LOW);

  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if (transmitOnStartup) {
    startTransmission = true;
    transmitOnStartup = false;
  }

  if (digitalRead(PUSH_BUTTON_PIN) == LOW) {
    startTransmission = true;
  }

  if (startTransmission) {
    sendSweep();
    startTransmission = false;
  }
}

void sendSweep() {
  for (unsigned int pulseLength = PULSE_LENGTH_MIN_US;
       pulseLength <= PULSE_LENGTH_MAX_US;
       pulseLength += PULSE_LENGTH_STEP_US) {
    sendBitstream(pulseLength);
    delay(TRANSMISSION_DELAY_MS);
  }
}

void sendBitstream(unsigned int pulseLengthUs) {
  for (size_t dataIndex = 0; dataIndex < DATA_LENGTH; dataIndex++) {
    digitalWrite(TX_PIN, DATA[dataIndex] ? HIGH : LOW);
    delayMicroseconds(pulseLengthUs);
  }

  digitalWrite(TX_PIN, LOW);
}
