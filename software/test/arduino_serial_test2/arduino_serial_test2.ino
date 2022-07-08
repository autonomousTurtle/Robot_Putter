#include <SoftwareSerial.h>
#include <Wire.h>
#include <Timer.h>
#include <Event.h>


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

// serail
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
}


void ALLSTOP() {
  Serial.write("All Stop\n");
}


void setup() {
  // start serial communicaitons with the servo controller

  // Start serial communication with the PC
  Serial.begin(115200);
  Serial.write("Power On Robot Motor Control\n");
  hti = 1000;
  timerEvent = timer.after(hti, clearHBB);
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

      Serial.print("DRV ");
      Serial.print(RMotorInput);
      Serial.println(">");
    }

  }

  newData = false;
} // end while loop
