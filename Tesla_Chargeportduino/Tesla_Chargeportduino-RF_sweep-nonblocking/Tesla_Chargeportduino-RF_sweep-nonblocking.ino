/*
  Arduino Tesla Charge Port Opener - non-blocking sweep version

  Sends the captured OOK/ASK bitstream while sweeping the pulse length from
  390 us to 410 us. Both the RF bit timing and the pause between transmissions
  are driven by micros()/millis(), so loop() can continue to run.
*/

const unsigned int PULSE_LENGTH_MIN_US = 390;
const unsigned int PULSE_LENGTH_MAX_US = 410;
const unsigned int PULSE_LENGTH_STEP_US = 5;
const unsigned long TRANSMISSION_DELAY_MS = 100;

const byte TX_PIN = 2;
const byte PUSH_BUTTON_PIN = 0;
const byte BUTTON_GND_PIN = 4;

unsigned int pulseLengthNowUs = PULSE_LENGTH_MIN_US;
unsigned long previousMicros = 0;
unsigned long lastTransmissionMillis = 0;
size_t dataIndex = 0;

bool startTransmission = false;
bool transmitOnStartup = true;
bool transmittingBitstream = false;
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

  if (!transmittingBitstream) {
    transmittingBitstream = true;
    dataIndex = 0;
    previousMicros = micros();
    digitalWrite(TX_PIN, DATA[dataIndex] ? HIGH : LOW);
    dataIndex++;
    return;
  }

  if (micros() - previousMicros < pulseLengthNowUs) {
    return;
  }

  previousMicros += pulseLengthNowUs;

  if (dataIndex < DATA_LENGTH) {
    digitalWrite(TX_PIN, DATA[dataIndex] ? HIGH : LOW);
    dataIndex++;
    return;
  }

  digitalWrite(TX_PIN, LOW);
  transmittingBitstream = false;
  waitingBetweenTransmissions = true;
  lastTransmissionMillis = millis();
}
