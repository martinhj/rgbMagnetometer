#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


// OUTPUT: Use digital pins 9-11, the Pulse-width Modulation (PWM) pins
// LED's cathodes should be connected to digital GND
int redPin = 9;   // Red LED,   connected to digital pin 9
int grnPin = 10;  // Green LED, connected to digital pin 10
int bluPin = 11;  // Blue LED,  connected to digital pin 11


// Program variables
int redVal = 0;   // Variables to store the values to send to the pins
int grnVal = 0;
int bluVal = 0;

int magnetometer = 0;
int magVal = 0; //Variable to store the input from the magnetometer


// Pi for calculations - not the raspberry type
const float Pi = 3.14159;

// This is the desired direction of travel
// expressed as a 0-360 degree compass heading
// 0.0 = North
// 90.0 = East
// 180.0 = South
// 270 = West
const float targetHeading = 0.0;



void setup()

{ 
  
  Serial.begin(9600);
  Serial.println("Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  pinMode(redPin, OUTPUT);   // sets the pins as output
  pinMode(grnPin, OUTPUT);   
  pinMode(bluPin, OUTPUT); 
  
}



void loop(void)

{ /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
   
     
  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;  // Normalize to 0-360
  
  
  if (heading < 0)
 
  { heading = 360 + heading;
  }
  
       
  magVal = heading;  // read the magnetometer
  
  Serial.print("mag: "); Serial.print(magVal); Serial.print(" # ");


  if (magVal < 91)  // Lowest fourth of the magnetometer's range (0-90)
  
  { magVal = (magVal * 3) / 4; // Normalize to 0-68

    redVal = 69 - magVal;  // Red from full to off
    grnVal = magVal;        // Green from off to full
    bluVal = 1;             // Blue off
    
    Serial.print("1: "); Serial.print(redVal); Serial.print(":");Serial.print(grnVal);
    Serial.print(":"); Serial.println(bluVal);
  }
  
  
  else if (magVal < 181) // Lower middle fourth of magnetometer's range (91-180)
  
  { magVal = ( (magVal-91) * 3) / 4; // Normalize to 0-68

    redVal = 1;            // Red off
    grnVal = 69 - magVal; // Green from full to off
    bluVal = magVal;       // Blue from off to full
    
    Serial.print("2: "); Serial.print(redVal); Serial.print(":");Serial.print(grnVal);
    Serial.print(":"); Serial.println(bluVal);
  }
  
  
  else if (magVal < 271)// Upper middle fourth of magnetometer"s range (181-270)
 
  { magVal = ( (magVal-182) * 3) / 4; // Normalize to 0-68

    redVal = 1;           // Red off
    grnVal = 1;            // Green off
    bluVal = 69 - magVal; // Blue from full to off
    
    Serial.print("3: "); Serial.print(redVal); Serial.print(":");Serial.print(grnVal);
    Serial.print(":"); Serial.println(bluVal);
  }
  
  
  else  // Upper fourth of magnetometer"s range (271-360)
 
  { magVal = ( (magVal-272) * 3) / 4; // Normalize to 0-68

    redVal = 1;       // Red of
    grnVal = 1;       // Green off
    bluVal = 1;       // Blue off
    
    Serial.print("4: "); Serial.print(redVal); Serial.print(":");Serial.print(grnVal);
    Serial.print(":"); Serial.println(bluVal);
  }
  

  analogWrite(redPin, redVal);   // Write values to LED pins
  analogWrite(grnPin, grnVal); 
  analogWrite(bluPin, bluVal);  
    
}
