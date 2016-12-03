# W-MazeArduino

The W-Track Alternation Task (Frank, Brown, Wilson 2000) has been used to examine the relationship 
between the spatially related activity of cells in the hippocampus and an animal’s ability to learn to 
successfully navigate to a goal. In this task, animals must learn to navigate a w-shaped maze in an alternating pattern 
to receive reward.

This sketch controls 3 solenoids at the end of each arm of the w-shaped track. 
The solenoid is opened based on the input from 6 infrared sensors; if the animal triggers the sensors in the 
correct w-shaped pattern, the solenoid will open and dispense a food reward.

Dependencies:

Arduino Microcontroller (UNO preferred)

Arduino Software  https://www.arduino.cc/en/Main/Software

6 infrared sensors on pins 6,4,2,8,9,and 10

3 twelve-volt solenoids on pins 7,5,and 3 [You will need a relay as the Arduino board uses 5 volt logic]

