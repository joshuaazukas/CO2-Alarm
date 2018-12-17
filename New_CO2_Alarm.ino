/*
New_CO2_Alarm adapted from:

  AN128_ardunio_cozir CO2 Demonstration code 11/29/2017 
  Written by: Marv Kausch 11/22/2016 at Co2meter.com
  Revised 11/29/17 by John Houck.

Alarm Adaptation:
By Joshua Azukas at Stony Brook University 
  Runs on Ardunio UNO, MEGA or MEGA2560
  Written 12/17/18
     
  This code reads and converts CO2 readings from COZIR CO2 meter to an
  Arduino microcontroller in PPM. It thIf the CO2 level is above threshold,
  the green LED is turned off and the red LED is illuminated and the Piezo 
  electric clicker begins to tick. This can be used as an alarm for 
  experiments that are CO2 dependent. This code can be easily modified
  to alarm if CO2 levels fall bellow a certain value as well.
   
  pin connections:
   
  Arduino________COZIR Sensor
   GND ------------------ 1 (gnd)
   3.3v------------------- 3 (Vcc)  
    13 -------------------- 5 (Rx)
    12 -------------------- 7 (Tx)
  Arduino________Device_Output
    2 --------------------- GLED
    3 --------------------- RLED
    4 --------------------- Piezo
*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 13); // RX, TX pins on Ardunio
int co2l;
int ledPin2 = 2;
int ledPin3 = 3;
int speakerPin = 4;
int co2 =0;
double multiplier = 1;// 1 for 2% =20000 PPM, 10 for 20% = 200,000 PPM
uint8_t buffer[25];
uint8_t ind =0;
uint8_t index =0;

int fill_buffer();  // function prototypes here
int format_output();

void setup() {
  
  pinMode(ledPin3, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(9600);
  Serial.print("\n\n");
  Serial.println("             AN128 Ardunio to Cozir CO2 Sensor - Demonstration code 11/29/2017\n\n"); 
  mySerial.begin(9600); // Start serial communications with sensor
  //mySerial.println("K 0");  // Set Command mode
  mySerial.println("M 6"); // send Mode for Z and z outputs
  // "Z xxxxx z xxxxx" (CO2 filtered and unfiltered)

  mySerial.println("K 1");  // set streaming mode
  }

void loop() {

  fill_buffer();  // function call that reacds CO2 sensor and fills buffer
 
    Serial.print("Buffer contains: ");
  for(int j=0; j<ind; j++)Serial.print(buffer[j],HEX);
 index = 0;
 format_output();
  Serial.print(" Raw PPM        ");
 
 index = 8;  // In ASCII buffer, filtered value is offset from raw by 8 bytes
  format_output();
  Serial.println(" Filtered PPM\n\n");
  if (co2l > 300) { // command sets "greater than" or "less than"
                    // value sets threshold for sensor, use serial
                    // monitor to calibrate levels, or can
                    // estimate in PPM.
    digitalWrite(ledPin3, HIGH);  
    digitalWrite(ledPin2, LOW);
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(100000);
    digitalWrite(speakerPin, LOW);
    }
  else {
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
}
}
int fill_buffer(void){
  

// Fill buffer with sensor ascii data
ind = 0;
while(buffer[ind-1] != 0x0A){  // Read sensor and fill buffer up to 0XA = CR
  if(mySerial.available()){
    buffer[ind] = mySerial.read();
    ind++;
    } 
  }
  // buffer() now filled with sensor ascii data
  // ind contains the number of characters loaded into buffer up to 0xA =  CR
  ind = ind -2; // decrement buffer to exactly match last numerical character
  }

 int format_output(void){ // read buffer, extract 6 ASCII chars, convert to PPM and print
   co2 = buffer[15-index]-0x30;
  co2 = co2+((buffer[14-index]-0x30)*10);
  co2 +=(buffer[13-index]-0x30)*100;
  co2 +=(buffer[12-index]-0x30)*1000;
  co2 +=(buffer[11-index]-0x30)*10000;
  Serial.print("\n CO2 = ");
  Serial.print(co2*multiplier,0);
  co2l = (co2*multiplier);
  //Serial.print(" PPM,");
     // Serial.print("\n");
delay(200);    // Frequency of reading output
 }
 

