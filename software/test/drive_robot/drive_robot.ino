/***************************************************

  Online Libraries:
  Timer.h - JChristhensen's Timer Library https://github.com/JChristensen/Timer
  Event.h - Arcturial arduino event https://github.com/arcturial/arduino-event


  Link to the Adafruit PWM Servo Driver
  http://www.adafruit.com/products/815

  Link to Pololu Qik Motor Driver Driver
  https://github.com/pololu/qik-arduino

  Required connections between Arduino and Qik 2s9v1:

      Arduino     Qik 2s9v1
   -------------------------
             5V - VCC
            GND - GND
  Digital Pin 2 - TX
  Digital Pin 3 - RX
  Digital Pin 4 - RESET


  Link to Raspberry Pi to Arduino Serial Comm:
  https://www.aranacorp.com/en/serial-communication-between-raspberry-pi-and-arduino/


 ****************************************************/

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Timer.h>
#include <Event.h>
#include <PololuQik.h>

// ---------- L298N Motor Driver ---------
#define enA 5
#define in1 6
#define in2 7
#define in3 8
#define in4 9
#define enB 10


// ---------- POLOLU MOTOR DRIVER -----------
PololuQik2s9v1 qik(2, 3, 4);

// Left motor is M0
// Right motor is M1

// ---------- RASBPERRY PI COMMS ----------
int timerEvent;
int hti; //heartbeat timeout interfal
boolean hbb; //heartbeat fail indication
Timer timer;
const byte inDataLen = 28;
char inData[inDataLen]; // buffer input commands
char messageFromPC[inDataLen] = {0};
double HBBtime = 0;
double LMotorInput;
double RMotorInput;

boolean newData = false; // detect new data into the arduino from the pi

boolean between(int in, int lower, int upper) {
  boolean result = false;
  if ((in <= upper) && (in >= lower)) {
    result = true;
  }
  return result;
}


void clearHBB() {
  hbb = false;
  timerEvent = timer.after(hti, clearHBB);
  // stop all of the motors
  analogWrite(enA, 0); // Send PWM signal to L298N Enable pin
  analogWrite(enB, 0); // Send PWM signal to L298N Enable pin
  //qik.setM0Speed(0);
  //qik.setM1Speed(0);
}


void ALLSTOP() {
  //qik.setM0Speed(0);
  //qik.setM1Speed(0);
  analogWrite(enA, 0); // Send PWM signal to L298N Enable pin
  analogWrite(enB, 0); // Send PWM signal to L298N Enable pin
  Serial.write("All Stop\n");
}


void setup() {
 
  // Start serial communication with the PC
  Serial.begin(115200);
  hti = 1000;
  timerEvent = timer.after(hti, clearHBB);

  // initiate the motor driver
  //qik.init();
  //Serial.print("Firmware version: ");
  //Serial.write(qik.getFirmwareVersion());
  //Serial.println(">");
  
  // ---------- L298N Pin Settings ---------
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  
  Serial.println("ready >");

  delay(500);

}

void loop() {
  char EOL = '>';
  static byte ndx = 0;

  timer.update();

  if (Serial.available() > 0) {
    char inChar = Serial.read();

    if (inChar != EOL) {
      inData
      [ndx] = inChar;
      ndx++;
      if (ndx >= inDataLen) {
        ndx =  inDataLen - 1;
      }
    }
    else if (inChar == EOL) {
      inData[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }

  if (newData == true) {

    // parse the data commas are the seperator

    // split the data into its parts
    char * strtokIndx; //
    strtokIndx = strtok(inData, ","); // get the first part, the string (HBB, DRV, STP..)
    strcpy(messageFromPC, strtokIndx); // copy the string to the messageFromPC char


    if (messageFromPC[0] == 'S') {
      // stop!
      Serial.print("STOP CMD\n");
      ALLSTOP();
    }

    // heartbeat - check this first
    if (messageFromPC[0] == 'H') {
      // parse the time data
      strtokIndx = strtok(NULL, ","); // continues where we left off in the message
      //strcpy(HBBtime, strtokIndx);
      //Serial.print(strtokIndx);
      HBBtime = atof(strtokIndx);

      // just put it right back out the serial port
      Serial.print("HBB ");
      Serial.print(HBBtime);
      Serial.println(">");
      hbb = true;
      timer.stop(timerEvent); //candel the last timer
      // set a timer that goes off in 2 seconds
      timerEvent = timer.after(hti, clearHBB);
    }

    if (messageFromPC[0] == 'D') {
      hbb = true;
      timer.stop(timerEvent);
      timerEvent = timer.after(hti, clearHBB);
      // parse data

      strtokIndx = strtok(NULL, ","); // continues where we left off in the message
      RMotorInput = atof(strtokIndx);
      strtokIndx = strtok(NULL, ","); // continues where we left off in the message
      LMotorInput = atof(strtokIndx);

      // set motor directions based on incoming commands
      if (RMotorInput > 0) {
        digitalWrite(in1, 0);
        digitalWrite(in2, 1);
      }
      else if (RMotorInput < 0) {
        digitalWrite(in1, 1);
        digitalWrite(in2, 0);
        RMotorInput=-RMotorInput;
      }

      if (LMotorInput > 0) {
        digitalWrite(in3, 0);
        digitalWrite(in4, 1);
      }
      else if (LMotorInput < 0) {
        digitalWrite(in3, 1);
        digitalWrite(in4, 0);
        LMotorInput=-LMotorInput;
      }
      

      //set motors speeds
      analogWrite(enA, RMotorInput); // Send PWM signal to L298N Enable pin
      analogWrite(enB, LMotorInput); // Send PWM signal to L298N Enable pin

      Serial.print("DRV ");
      Serial.print(RMotorInput);
      Serial.println(">");
    }

    if (messageFromPC[0] == 'P') {
      hbb = true;
      Serial.print("Putting");
      Serial.print(">");

      strtokIndx = strtok(NULL, ","); // continues where we left off in the message
      double motor0input = atof(strtokIndx);
      strtokIndx = strtok(NULL, ","); // continues where we left off in the message
      double motor1input = atof(strtokIndx);
      strtokIndx = strtok(NULL, ","); // continues where we left off in the message
      double motor2input = atof(strtokIndx);
      strtokIndx = strtok(NULL, ","); // continues where we left off in the message
      double gripperInput = atof(strtokIndx);

      Serial.print("ARM:");
      Serial.print(motor0input);
      Serial.print(",");
      Serial.print(motor1input);
      Serial.print(",");
      Serial.print(motor2input);
      Serial.print(",");
      Serial.print(gripperInput);
      Serial.print(">");

    }
  }

  newData = false;
} // end while loop
