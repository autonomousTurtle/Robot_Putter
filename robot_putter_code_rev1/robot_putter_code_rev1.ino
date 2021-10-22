#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <Adafruit_MotorShield.h>
#define led 4

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
unsigned long lastPuttTime = 0;

int motorSpeed = 255;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *putterMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);


// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int X1;
  int X2;
  int Y1;
  int Y2;
  byte button1;
  int puttSpeed;
};
Data_Package data; //Create a variable with the above structurej



RF24 radio(7, 8); // using pin 7 for the CE pin, and pin 8 for the CSN pin
uint8_t address[][6] = {"1Node", "2Node"};
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
float payload = 0.0;

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

  Serial.println(F("Starting"));
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(Data_Package)); // float datatype occupies 4 bytes
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1
  radio.startListening();

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  //-------MOTOR SETUP PARAMETERS-------
  leftMotor->setSpeed(0); // max speed is 255
  rightMotor->setSpeed(0); // max speed is 255
  putterMotor->setSpeed(0); // max speed is 255
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  putterMotor->run(RELEASE);

} //setup

void loop() {
  currentTime = millis();

  uint8_t pipe;
  if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
    radio.read(&data, sizeof(Data_Package));            // fetch payload from FIFO
    lastReceiveTime = millis();
    /*
      Serial.print(F("Received "));
      Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
      Serial.print(data.X1);                // print the payload's value
      Serial.print(", ");                // print the payload's value
      Serial.print(data.X2);                // print the payload's value
      Serial.print(", ");
      Serial.println(data.button1);                // print the payload's value
    */
    digitalWrite(led, HIGH);
  }

  else if (currentTime - lastReceiveTime > 3000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone jas a throttle up, if we lose connection it can keep flying away if we dont reset the function
    digitalWrite(led, LOW);
  }

  //process the data to see what the robot should do
  if (data.button1 > 1) {
    currentTime = millis();
    if (currentTime - lastPuttTime > 3000) {
      putt();
      lastPuttTime = millis();
    }
  }

  leftMotor->setSpeed(motorSpeed-2);
  rightMotor->setSpeed(motorSpeed);

  if (data.X1 == 1 && data.X2 == 1 && data.Y1 == 0 ) {
    rotateLeft();
  }
  else if (data.X1 == 1 && data.X2 == 1 && data.Y2 == 0 ) {
    rotateRight();
  }
  else if (data.X1 == 0 && data.Y1 == 1 && data.Y2 == 1) {
    moveForward();
  }
  else if (data.X2 == 0 && data.Y1 == 1 && data.Y2 == 1) {
    moveBackward();
  }
  else if (data.X1 == 0 && data.Y1 == 0) {
    moveLeftForward();
  }
  else if (data.X1 == 0 && data.Y2 == 0) {
    moveRightForward();
  }
  else if (data.X2 == 0 && data.Y2 == 0) {
    moveRightBackward();
  }
  else if (data.X2 == 0 && data.Y1 == 0) {
    moveLeftBackward();
  }
  else {
    stopMoving();
  }


} //loop

void putt()
{
  uint8_t i;
  Serial.print("Putt speed");
  Serial.println(data.puttSpeed);

  int backwardStroke;
  int forwardStroke;
  int returnStroke;
  int puttMotorSpeed;

  if (data.puttSpeed > 700) { //fast putt

    backwardStroke = 120;
    forwardStroke = 50;
    returnStroke = 67;
    puttMotorSpeed = 255;
  }
  else if (data.puttSpeed < 300) { //slow putt
    backwardStroke = 70;
    forwardStroke = 40;
    returnStroke = 62;
    puttMotorSpeed = 150;
  }
  else { //medium putt
    backwardStroke = 80;
    forwardStroke = 30;
    returnStroke = 64;
    puttMotorSpeed = 255;
  }

  //run motor backwards to swing putter back
  putterMotor->run(BACKWARD);
  for (i = 0; i < backwardStroke; i++) {
    putterMotor->setSpeed(i);
    delay(10);
  }
  for (i = backwardStroke; i != 0; i--) {
    putterMotor->setSpeed(i);
    delay(10);
  }

  delay(500);
  //run motor forwards to hit ball
  putterMotor->run(FORWARD);
  for (i = 0; i < forwardStroke; i++) {
    putterMotor->setSpeed(puttMotorSpeed);
    delay(10);
  }
  for (i = (forwardStroke - 20); i != 0; i--) {
    putterMotor->setSpeed(i);
    delay(10);
  }

  //return to home


  putterMotor->run(BACKWARD);
  for (i = 0; i < returnStroke; i++) {
    putterMotor->setSpeed(i);
    delay(10);
  }
  for (i = returnStroke; i != 0; i--) {
    putterMotor->setSpeed(i);
    delay(10);
  }
  putterMotor->run(RELEASE);
}


void moveForward() {
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}
void moveBackward() {
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}
void moveSidewaysRight() {
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
}
void moveSidewaysLeft() {
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
}
void rotateLeft() {
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
}
void rotateRight() {
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
}
void moveRightForward() {
  rightMotor->setSpeed(motorSpeed / 2);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}
void moveRightBackward() {
  rightMotor->setSpeed(motorSpeed / 2);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}
void moveLeftForward() {
  leftMotor->setSpeed(motorSpeed / 2);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}
void moveLeftBackward() {
  leftMotor->setSpeed(motorSpeed / 2);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}
void stopMoving() {
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void resetData() {
  Serial.println("reset data");
  // Reset the values when there is no radio connection - Set initial default values
  data.X1 = 1;
  data.X2 = 1;
  data.Y1 = 1;
  data.Y2 = 1;
  data.button1 = 0;
}
