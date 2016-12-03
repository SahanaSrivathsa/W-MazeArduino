//Pins
const int solPin1 = 7; // output to solenoid 1
const int solPin2 = 5;// output to solenoid 2
const int solPin3 = 3; //output to solenoid 3
const int ir1Pin = 6; // input from ir1
const int ir2Pin = 4; //input from ir2
const int ir3Pin = 2; //input from ir3
const int ir1MidPin = 8; //input from ir1M
const int ir2MidPin = 9; //input from ir2M
const int ir3MidPin = 10; //input from ir3M

//Solenoid
const int solDur = 300; //duration solenoid is open

//IR Sensors
boolean checking1 = false; //reset to listen to ir1
boolean checking2 = true; //reset to listen to ir2
boolean checking3 = false; //reset to listen to ir3
int trigRew = 0; //Which solenoid to trigger a reward at
int ir1Val = 0; //reads the value from ir1 pin
int ir2Val = 0; //reads the value from ir2 pin
int ir3Val = 0; //reads the value from ir3 pin
int ir1MidVal = 0; //reads the value from ir1M pin
int ir2MidVal = 0; //reads the value from ir2M pin
int ir3MidVal = 0; //reads the value from ir3M pin
int phase = 0; //Have we determined the correct pattern yet?
int next_sensor = 2; //The next sensor the rat must visit. Use to index the pattern array.
int correct_pattern[] = {0, 0, 0, 0}; //The current correct pattern

//Tracking Errors
int errCnt = 0; //Number of errors for the session
boolean departedir1 = false; //Has the rat left ir1?
boolean departedir2 = false; //Has the rat left ir2?
boolean departedir3 = false; //Has the rat left ir3?

//Feeder Visits
int totfeed = 0; //Total Correct Feeder Visits
int TotalLaps = totfeed + errCnt; //Total Feeder Visits (correct and incorrect)

void setup() {
  Serial.begin(9600);
  //Set pin direction
  pinMode(solPin1, OUTPUT);
  pinMode(solPin2, OUTPUT);
  pinMode(solPin3, OUTPUT);
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);
  pinMode(ir1MidPin, INPUT);
  pinMode(ir2MidPin, INPUT);
  pinMode(ir3MidPin, INPUT);

  //Set initial pin state
  digitalWrite(solPin1, LOW); //solenoid 1 pin starts low
  digitalWrite(solPin2, LOW); //solenoid 2 pin starts low
  digitalWrite(solPin3, LOW); //solenoid 3 pin starts low
}

void loop() {
  ir1Val = digitalRead(ir1Pin); //reads the value from ir1 pin
  ir2Val = digitalRead(ir2Pin); //reads the value from ir2 pin
  ir3Val = digitalRead(ir3Pin); //reads the value from ir3 pin
  ir1MidVal = digitalRead(ir1MidPin); //reads the value from ir1M pin
  ir2MidVal = digitalRead(ir2MidPin); //reads the value from ir2M pin
  ir3MidVal = digitalRead(ir3MidPin); //reads the value from ir3M pin

  //Before correct pattern is established ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //First Trial//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (phase == 0) {
    if (ir2Val == LOW && checking2) {
      Serial.println("IR2 was triggered. First lap complete.");
      checking2 = false; //igore ir2
      checking1 = true; //listen to ir1
      checking3 = true; //listen to ir3
      trigRew = 2; //dispense reward to solenoid 2
      phase = 1;
    }
    //If the rat makes a mistake (turns around and goes to 1 or 3 first)
    if (ir1Val == LOW) {
      errCnt++;
      Serial.println("You have made an error. You must go to arm 2");
      Serial.print("Total Errors: "); Serial.print(errCnt); Serial.println(" ");
      departedir1 = false;
      correct_pattern[0] = 2;
      correct_pattern[1] = 3;
      correct_pattern[2] = 2;
      correct_pattern[3] = 1;
      next_sensor = 0;
      phase = 2;
      departedir3 = true;

    }
    if (ir3Val == LOW) {
      errCnt++;
      Serial.println("You have made an error. You must go to arm 2");
      Serial.print("Total Errors: "); Serial.print(errCnt); Serial.println(" ");
      departedir3 = false;
      correct_pattern[0] = 2;
      correct_pattern[1] = 1;
      correct_pattern[2] = 2;
      correct_pattern[3] = 3;
      next_sensor = 0;
      phase = 2;
      departedir1 = true;
    }
  }
  //Second Trial//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (phase == 1) {
    if (ir2MidVal == LOW) {
      departedir2 = true;
    }
    if (ir1Val == LOW && checking1) {
      Serial.println("IR1 was triggered. The animal must now complete Pattern A");
      checking1 = false;
      checking2 = true;
      checking3 = true;
      trigRew = 1;
      correct_pattern[0] = 2;
      correct_pattern[1] = 1;
      correct_pattern[2] = 2;
      correct_pattern[3] = 3;
      phase = 2;
    }
    if (ir3Val == LOW && checking3) {
      Serial.println("IR3 was triggered. The animal must now complete Pattern B");
      checking3 = false;
      checking2 = true;
      checking1 = true;
      trigRew = 3;
      correct_pattern[0] = 2;
      correct_pattern[1] = 3;
      correct_pattern[2] = 2;
      correct_pattern[3] = 1;
      phase = 2;
    }
    if (ir2Val == LOW && !checking2 && departedir2 == true) {
      errCnt++;
      Serial.println("You have made an error. You were just at Feeder 2.");
      Serial.print("Total Errors: "); Serial.print(errCnt); Serial.println(" ");
      departedir2 = false;
    }
  }
  //After Correct Pattern has been Established~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (phase == 2) {
    if (ir1MidVal == LOW) {
      departedir1 = true;
    }
    if (ir2MidVal == LOW) {
      departedir2 = true;
    }
    if (ir3MidVal == LOW) {
      departedir3 = true;
    }
    //Feeder 1///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (ir1Val == LOW) {
      if (correct_pattern[next_sensor] == 1) {
        if (checking1) {
          Serial.println("IR1 was triggered. Good. Keep it up!");
          trigRew = 1; //trigger solenoid 1
          checking1 = false;
          checking2 = true;
          checking3 = true;
          departedir1 = false;
          departedir2 = false;
          departedir3 = false;
        }
        if (next_sensor == 3) next_sensor = 0;

        else next_sensor++;
      }
      else {
        if (departedir1) {
          errCnt++;
          Serial.println("You have made an error! Feeder 1 was not the next correct stop. Go back to arm 2");
          Serial.print("Total Errors: "); Serial.print(errCnt); Serial.println(" ");
          departedir1 = false;
          correct_pattern[0] = 2;
          correct_pattern[1] = 3;
          correct_pattern[2] = 2;
          correct_pattern[3] = 1;
          next_sensor = 0;
        }
      }
    }
    //Feeder 2///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (ir2Val == LOW) {
      if (correct_pattern[next_sensor] == 2) {
        if (checking2) {
          Serial.println("IR2 was triggered. Good. Keep it up!");
          checking2 = false;
          checking1 = true;
          checking3 = true;
          trigRew = 2;
          departedir1 = false;
          departedir2 = false;
          departedir3 = false;
        }
        if (next_sensor == 3) {
          next_sensor = 0;
        }
        else next_sensor++;
      }
      else {
        if (departedir2) {
          errCnt++;
          Serial.println("You have made an error; you were just at arm 2!");
          Serial.print("Total Errors: "); Serial.print(errCnt); Serial.println(" ");
          departedir2 = false;
        }
      }
    }
    //Feeder 3/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (ir3Val == LOW) {
      if (correct_pattern[next_sensor] == 3) {
        if (checking3) {
          Serial.println("IR3 was triggered. Good. Keep it up!");
          checking3 = false;
          checking1 = true;
          checking2 = true;
          trigRew = 3;
          departedir1 = false;
          departedir2 = false;
          departedir3 = false;
        }
        if (next_sensor == 3) {
          next_sensor = 0;
        }
        else next_sensor++;
      }
      else {
        if (departedir3) {
          errCnt++;
          Serial.println("You have made an error! Feeder 3 was not the next correct stop. Go back to arm 2");
          Serial.print("Total Errors: "); Serial.print(errCnt); Serial.println(" ");
          departedir3 = false;
          correct_pattern[0] = 2;
          correct_pattern[1] = 1;
          correct_pattern[2] = 2;
          correct_pattern[3] = 3;
          next_sensor = 0;
        }
      }
    }
  }
  //Dispense Reward//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  switch (trigRew) {
    case 1:                      //Reward sequence for solenoid 1
      digitalWrite(solPin1, HIGH);
      delay(solDur);
      digitalWrite(solPin1, LOW);
      totfeed++;
      Serial.print("CORRECT FEEDER VISITS: "); Serial.print(totfeed); Serial.println(" ");
      break;
    case 2:                     //Reward sequence for solenoid 2
      digitalWrite(solPin2, HIGH);
      delay(solDur);
      digitalWrite(solPin2, LOW);
      totfeed++;
      Serial.print("CORRECT FEEDER VISITS: "); Serial.print(totfeed); Serial.println(" ");
      break;
    case 3:                    //Reward sequence for solenoid 3
      digitalWrite(solPin3, HIGH);
      delay(solDur);
      digitalWrite(solPin3, LOW);
      totfeed++;
      Serial.print("CORRECT FEEEDER VISITS: "); Serial.print(totfeed); Serial.println(" ");
      break;
  }
  trigRew = 0;
}
