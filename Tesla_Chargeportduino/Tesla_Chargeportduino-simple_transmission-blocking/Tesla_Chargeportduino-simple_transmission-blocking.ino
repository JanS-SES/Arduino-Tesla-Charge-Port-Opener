/*
  Arduino Tesla Charge Port Opener - simple fixed-timing transmission

  Sends the captured OOK/ASK bitstream with one fixed pulse length.
  This is the smallest possible version and is useful once you have calibrated
  the timing for your board/transmitter combination.
*/

const unsigned int PULSE_LENGTH_US = 400;
const byte TX_PIN = 2;
const unsigned int REPEAT_DELAY_MS = 100;

const uint8_t DATA[] = {
  0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1
};

const size_t DATA_LENGTH = sizeof(DATA) / sizeof(DATA[0]);

void setup() {
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW);
}

void loop() {
  for (size_t dataIndex = 0; dataIndex < DATA_LENGTH; dataIndex++) {
    digitalWrite(TX_PIN, DATA[dataIndex] ? HIGH : LOW);
    delayMicroseconds(PULSE_LENGTH_US);
  }

  digitalWrite(TX_PIN, LOW);
  delay(REPEAT_DELAY_MS);
}
