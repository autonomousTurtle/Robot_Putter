/***************************************************

  Online Libraries:
  Timer.h - JChristhensen's Timer Library https://github.com/JChristensen/Timer
  Event.h - Arcturial arduino event https://github.com/arcturial/arduino-event
  Link to the Adafruit PWM Servo Driver  http://www.adafruit.com/products/815

  Link to Raspberry Pi to Arduino Serial Comm:
  https://www.aranacorp.com/en/serial-communication-between-raspberry-pi-and-arduino/


 ****************************************************/

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Timer.h>
#include <Event.h>


// ---------- L298N Motor Driver ---------
#define enA 9
#define in1 12
#define in2 11
#define in3 8
#define in4 13
#define enB 10


// ---------- LED Pins ---------
#define LED1 3  // status LED, green is online and HB is good
#define LED2 2
#define LED3 4
#define LED4 5


// ---------- SERVO DRIVER -----------
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // called this way, it uses the default address 0x40

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// Servo home positions (in microsecond length)
const int servo0_home = 300;
const int servo1_home = 280;


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
  digitalWrite(LED1, LOW);
}


void ALLSTOP() {
  analogWrite(enA, 0); // Send PWM signal to L298N Enable pin
  analogWrite(enB, 0); // Send PWM signal to L298N Enable pin
  Serial.write("All Stop\n");
}


void setup() {
  // Start serial communication with the PC
  Serial.begin(115200);
  hti = 1000;
  timerEvent = timer.after(hti, clearHBB);
  
  // ---------- L298N Pin Settings ---------
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  // servo driver
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates


  delay(100); 

  // set servos to 0 position
  pwm.setPWM(0, 0, servo0_home); // putter servo
  pwm.setPWM(1, 0, servo1_home); // camera servo

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
      digitalWrite(LED1, LOW); //status LED on 
    }

    // heartbeat - check this first
    if (messageFromPC[0] == 'H') {
      digitalWrite(LED1, HIGH); //status LED on 
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
      digitalWrite(LED1, HIGH); //status LED on 
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
        RMotorInput = -RMotorInput;
      }

      if (LMotorInput > 0) {
        digitalWrite(in3, 0);
        digitalWrite(in4, 1);
      }
      else if (LMotorInput < 0) {
        digitalWrite(in3, 1);
        digitalWrite(in4, 0);
        LMotorInput = -LMotorInput;
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
      digitalWrite(LED2, HIGH);
      digitalWrite(LED1, HIGH); //status LED on 
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

      int servonum = 0; // set the servo to the putter servo

      // move the putter back
      for (uint16_t pulselen = servo0_home; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(servonum, 0, pulselen);
      }
      delay(500);

      // move the putter forward
      for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(servonum, 0, pulselen);
      }

      delay(500);

      pwm.setPWM(servonum, 0, servo0_home); // move the servo back home
      delay(500);
      digitalWrite(LED2, LOW);
    }
  }

  newData = false;
} // end while loop
