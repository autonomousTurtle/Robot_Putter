#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define rfStatusLED 4


RF24 radio(7, 8); // using pin 7 for the CE pin, and pin 8 for the CSN pin
uint8_t address[][6] = {"1Node", "2Node"};
bool radioNumber = 0; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
float payload = 0.0;

struct Data_Package {
  int j1PotX;
  int j1PotY;
  int j2PotX;
  int j2PotY;
  //byte pot1;
  //byte pot2;
  //byte tSwitch1;
  //byte tSwitch2;
  byte button1;

};
Data_Package data; //Create a variable with the above structure
int button_state = 0;
int j1X = A0;
int j1Y = A1;
int j2X = A2;
int j2Y = A3;

const int button = 3;
const int deadBand = 200;

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(Data_Package)); // float datatype occupies 4 bytes
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1
  radio.stopListening();  // put radio in TX mode

  data.j1PotX = 512;
  data.j1PotY = 512;
  data.j2PotX = 512;
  data.j2PotY = 512;
  //data.pot1 = 1;
  //data.pot2 = 1;
  //data.tSwitch1 = 1;
  //data.tSwitch2 = 1;
  data.button1 = 1;

  pinMode(3, INPUT_PULLUP);

} // setup

void loop() {
  // read the analog and digital inputs
  //byte j1PotX
  data.j1PotX = analogRead(A0);
  data.j1PotY = analogRead(A1);
  data.j2PotX = analogRead(A2);
  data.j2PotY = analogRead(A3);
  button_state = digitalRead(button);



  if (data.j1PotX > (512 - deadBand) && data.j1PotX < (512 + deadBand)) {
    data.j1PotX = 512;
  }
  if (data.j1PotY > 512 - deadBand && data.j1PotY < 512 + deadBand) {
    data.j1PotY = 512;
  }
  if (data.j2PotX > (512 - deadBand) && data.j2PotX < (512 + deadBand)) {
    data.j2PotX = 512;
  }
  if (data.j2PotY > 512 - deadBand && data.j2PotY < 512 + deadBand) {
    data.j2PotY = 512;
  }

  if (button_state == LOW)
  {
    Serial.println("button");
    data.button1 = 2;
  }
  else {
    data.button1 = 1;
  }


  Serial.print("j1PotX: ");
  Serial.print(data.j1PotX);
  Serial.print("j1PotY: ");
  Serial.print(data.j1PotY);
  Serial.print("j2PotX: ");
  Serial.print(data.j2PotX);
  Serial.print("j2PotY: ");
  Serial.print(data.j2PotY);
  Serial.print("button: ");
  Serial.println(data.button1);

  // This device is a TX node
  unsigned long start_timer = micros();                    // start the timer
  //bool report = radio.write(&payload, sizeof(Data_Package));      // transmit & save the report
  bool report = radio.write(&data, sizeof(Data_Package));      // transmit & save the report
  unsigned long end_timer = micros();                      // end the timer

  if (report) {
    digitalWrite(rfStatusLED, HIGH);
    Serial.print(F("Transmission successful! "));          // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer);                 // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.println(payload);                               // print payload sent
    payload += 0.01;                                       // increment float payload
  } else {
    Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    digitalWrite(rfStatusLED, LOW);
  }

  // to make this example readable in the serial monitor
  delay(50);  // slow transmissions down by 1 second

} // loop
