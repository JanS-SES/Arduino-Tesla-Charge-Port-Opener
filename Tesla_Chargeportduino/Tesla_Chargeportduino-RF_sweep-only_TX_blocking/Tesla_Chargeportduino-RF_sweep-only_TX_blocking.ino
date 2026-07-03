/*
  Arduino Tesla Charge Port Opener - sweep with blocking TX bursts

  The bitstream itself is sent with blocking delayMicroseconds() timing.
  The pause between bitstream repetitions is handled with millis(), so the
  surrounding sketch can keep running between RF bursts.
*/

const unsigned int PULSE_LENGTH_MIN_US = 390;
const unsigned int PULSE_LENGTH_MAX_US = 410;
const unsigned int PULSE_LENGTH_STEP_US = 5;
const unsigned long TRANSMISSION_DELAY_MS = 100;

const byte TX_PIN = 2;
const byte PUSH_BUTTON_PIN = 0;
const byte BUTTON_GND_PIN = 4;

unsigned int pulseLengthNowUs = PULSE_LENGTH_MIN_US;
unsigned long lastTransmissionMillis = 0;
bool startTransmission = false;
bool transmitOnStartup = true;
bool waitingBetweenTransmissions = false;

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

  openChargePort();
}

void openChargePort() {
  if (!startTransmission) {
    return;
  }

  if (waitingBetweenTransmissions) {
    if (millis() - lastTransmissionMillis < TRANSMISSION_DELAY_MS) {
      return;
    }

    waitingBetweenTransmissions = false;
    pulseLengthNowUs += PULSE_LENGTH_STEP_US;

    if (pulseLengthNowUs > PULSE_LENGTH_MAX_US) {
      pulseLengthNowUs = PULSE_LENGTH_MIN_US;
      startTransmission = false;
      return;
    }
  }

  sendBitstream(pulseLengthNowUs);
  lastTransmissionMillis = millis();
  waitingBetweenTransmissions = true;
}

void sendBitstream(unsigned int pulseLengthUs) {
  for (size_t dataIndex = 0; dataIndex < DATA_LENGTH; dataIndex++) {
    digitalWrite(TX_PIN, DATA[dataIndex] ? HIGH : LOW);
    delayMicroseconds(pulseLengthUs);
  }

  digitalWrite(TX_PIN, LOW);
}
