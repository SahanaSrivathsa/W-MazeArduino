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
    const int power = 11;

    //Solenoid
    const int solDur = 300; //duration solenoid is open on reward
    const int solDurFix = 400; //solenoid clogged
    const char fill1Message = 'X';
    const char fill2Message = 'Y';
    const char fill3Message = 'Z';
    const char close1Message = 'x';
    const char close2Message = 'y';
    const char close3Message = 'z';
    

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
    int correct_pattern[] = { 0, 0, 0, 0 }; //The current correct pattern

    //Tracking Errors
    int message;
    const String correctPre = "c";
    const String inboundPre = "i";
    const String outboundPre = "o";
    const String repeatPre = "r";
    const String initialPre = "b";
    const char loadingMessage = 'L';
    
    int lastFeeder = 0;
    boolean departedir1 = false; //Has the rat left ir1?
    boolean departedir2 = false; //Has the rat left ir2?
    boolean departedir3 = false; //Has the rat left ir3?


    void setup()
    {
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
        pinMode(power, OUTPUT);

        //Set initial pin state
        digitalWrite(solPin1, LOW); //solenoid 1 pin starts low
        digitalWrite(solPin2, LOW); //solenoid 2 pin starts low
        digitalWrite(solPin3, LOW); //solenoid 3 pin starts low
        digitalWrite(power, HIGH);
    }

    void loop()
    {
        ir1Val = digitalRead(ir1Pin); //reads the value from ir1 pin
        ir2Val = digitalRead(ir2Pin); //reads the value from ir2 pin
        ir3Val = digitalRead(ir3Pin); //reads the value from ir3 pin
        ir1MidVal = digitalRead(ir1MidPin); //reads the value from ir1M pin
        ir2MidVal = digitalRead(ir2MidPin); //reads the value from ir2M pin
        ir3MidVal = digitalRead(ir3MidPin); //reads the value from ir3M pin

        
       if(Serial.available() > 0){
          message = Serial.read();
       if (message == loadingMessage)
         {      
                checking1 = false;
                checking2 = true;
                checking3 = false;
                trigRew = 0;
                ir1Val = 0;
                ir2Val = 0;
                ir3Val = 0;
                ir1MidVal = 0;
                ir2MidVal = 0;
                ir3MidVal = 0;
                phase = 0;
                next_sensor = 2;
                correct_pattern[0] = 0;
                correct_pattern[1] = 0;
                correct_pattern[2] = 0;
                correct_pattern[3] = 0;
                lastFeeder = 0;
                departedir1 = false;
                departedir2 = false;
                departedir3 = false;
                digitalWrite(solPin1, LOW); //solenoid 1 pin starts low
                digitalWrite(solPin2, LOW); //solenoid 2 pin starts low
                digitalWrite(solPin3, LOW); //solenoid 3 pin starts low
                Serial.print("G\n");  
            }
          if (message == fill1Message){
              digitalWrite(solPin1, HIGH);
          }
           if (message == fill2Message){
              digitalWrite(solPin2, HIGH);
          }
           if (message == fill3Message){
              digitalWrite(solPin3, HIGH);
          }
           if (message == close1Message){
              digitalWrite(solPin1, LOW);
          }
           if (message == close2Message){
              digitalWrite(solPin2, LOW);
          }
           if (message == close3Message){
              digitalWrite(solPin3, LOW);
          }
          }
    else {
            //Before correct pattern is established ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            //First Trial//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (phase == 0)
            {
                if (ir2Val == LOW && checking2)
                {
                    checking2 = false; //igore ir2
                    checking1 = true; //listen to ir1
                    checking3 = true; //listen to ir3
                    trigRew = 2; //dispense reward to solenoid 2
                    phase = 1;
                    lastFeeder = 2;
                }
                //If the rat makes a mistake (turns around and goes to 1 or 3 first)
                if (ir1Val == LOW)
                {
                    Serial.print(initialPre + "\n");
                    Serial.print("1\n");
                    departedir1 = false;
                    correct_pattern[0] = 2;
                    correct_pattern[1] = 3;
                    correct_pattern[2] = 2;
                    correct_pattern[3] = 1;
                    next_sensor = 0;
                    phase = 2;
                    departedir3 = true;
                    lastFeeder = 1;

                }
                if (ir3Val == LOW)
                {
                    Serial.print(initialPre + "\n");
                    Serial.print("3\n");
                    departedir3 = false;
                    correct_pattern[0] = 2;
                    correct_pattern[1] = 1;
                    correct_pattern[2] = 2;
                    correct_pattern[3] = 3;
                    next_sensor = 0;
                    phase = 2;
                    departedir1 = true;
                    lastFeeder = 3;

                }
            }
            //Second Trial///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (phase == 1)
            {
                if (ir2MidVal == LOW)
                {
                    departedir2 = true;
                }
                if (ir1Val == LOW && checking1)
                {
                    checking1 = false;
                    checking2 = true;
                    checking3 = true;
                    trigRew = 1;
                    correct_pattern[0] = 2;
                    correct_pattern[1] = 1;
                    correct_pattern[2] = 2;
                    correct_pattern[3] = 3;
                    phase = 2;
                    lastFeeder = 1;
                }
                if (ir3Val == LOW && checking3)
                {
                    checking3 = false;
                    checking2 = true;
                    checking1 = true;
                    trigRew = 3;
                    correct_pattern[0] = 2;
                    correct_pattern[1] = 3;
                    correct_pattern[2] = 2;
                    correct_pattern[3] = 1;
                    phase = 2;
                    lastFeeder = 3;
                }
                if (ir2Val == LOW && !checking2 && departedir2 == true)
                {
                    Serial.print(repeatPre + "\n");
                    Serial.print("2\n");
                    departedir2 = false;
                    lastFeeder = 2;
                }
            }
            //After Correct Pattern has been Established~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            if (phase == 2)
            {
                if (ir1MidVal == LOW)
                {
                    departedir1 = true;
                }
                if (ir2MidVal == LOW)
                {
                    departedir2 = true;
                }
                if (ir3MidVal == LOW)
                {
                    departedir3 = true;
                }
                //Feeder 1///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (ir1Val == LOW)
                {
                    if (correct_pattern[next_sensor] == 1)
                    {
                        if (checking1)
                        {
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
                    else
                    {
                        if (departedir1)
                        {
                            departedir1 = false;
                            checking2 = true;
                            correct_pattern[0] = 2;
                            correct_pattern[1] = 3;
                            correct_pattern[2] = 2;
                            correct_pattern[3] = 1;
                            next_sensor = 0;
                            switch (lastFeeder)
                            {
                                case 1:
                                    Serial.print(repeatPre + "\n");
                                    break;
                                case 2:
                                    Serial.print(outboundPre + "\n");
                                    break;
                                case 3:
                                    Serial.print(inboundPre + "\n");
                                    break;
                            }
                            Serial.print("1\n");
                        }
                    }
                    lastFeeder = 1;
                }
                //Feeder 2///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (ir2Val == LOW)
                {
                    if (correct_pattern[next_sensor] == 2)
                    {
                        if (checking2)
                        {
                            checking2 = false;
                            checking1 = true;
                            checking3 = true;
                            trigRew = 2;
                            departedir1 = false;
                            departedir2 = false;
                            departedir3 = false;
                        }
                        if (next_sensor == 3)
                        {
                            next_sensor = 0;
                        }
                        else next_sensor++;
                    }
                    else
                    {
                        if (departedir2)
                        {
                            Serial.print(repeatPre + "\n");
                            Serial.print("2\n");
                            departedir2 = false;
                        }
                    }
                    lastFeeder = 2;
                }
                //Feeder 3/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (ir3Val == LOW)
                {
                    if (correct_pattern[next_sensor] == 3)
                    {
                        if (checking3)
                        {
                            checking3 = false;
                            checking1 = true;
                            checking2 = true;
                            trigRew = 3;
                            departedir1 = false;
                            departedir2 = false;
                            departedir3 = false;
                        }
                        if (next_sensor == 3)
                        {
                            next_sensor = 0;
                        }
                        else next_sensor++;
                    }
                    else
                    {
                        if (departedir3)
                        {
                            departedir3 = false;
                            checking2 = true;
                            correct_pattern[0] = 2;
                            correct_pattern[1] = 1;
                            correct_pattern[2] = 2;
                            correct_pattern[3] = 3;
                            next_sensor = 0;
                            switch (lastFeeder)
                            {
                                case 1:
                                    Serial.print(inboundPre + "\n");
                                    break;
                                case 2:
                                    Serial.print(outboundPre + "\n");
                                    break;
                                case 3:
                                    Serial.print(repeatPre + "\n");
                                    break;
                            }
                            Serial.print("3\n");
                        }
                    }
                    lastFeeder = 3;
                }
            }
            //Dispense Reward//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            switch (trigRew)
            {
                case 1:                      //Reward sequence for solenoid 1
                    digitalWrite(solPin1, HIGH);
                    delay(solDurFix);
                    digitalWrite(solPin1, LOW);
                    Serial.print(correctPre + "\n");
                    Serial.print("1\n");
                    break;
                case 2:                     //Reward sequence for solenoid 2
                    digitalWrite(solPin2, HIGH);
                    delay(solDur);
                    digitalWrite(solPin2, LOW);
                    Serial.print(correctPre + "\n");
                    Serial.print("2\n");
                    break;
                case 3:                    //Reward sequence for solenoid 3
                    digitalWrite(solPin3, HIGH);
                    delay(solDur);
                    digitalWrite(solPin3, LOW);
                    Serial.print(correctPre + "\n");
                    Serial.print("3\n");
                    break;
            }
            trigRew = 0;
        }
    }
 
