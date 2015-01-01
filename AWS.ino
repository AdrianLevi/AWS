// Date and time functions using DS3231 RTC connected via I2C and Wire lib
#include <avr/sleep.h>
#include <avr/power.h>
#include <PString.h>
#include <SPI.h>
#include <Wire.h>
#include "DS3231.h"
#include <SD.h>
#include <Battery.h>

// Pin Definitions & Settings
// On the Seeeduino Stalker, CS is pin 10. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
#define chipSelect 10
#define lm335pin 1
#define INT0pin 2
#define INT1pin 3
#define anemometerpin 2
#define rainguagepin 3
#define SerialSpeed 115200
#define LEDPin 8
#define flashesforfull 10

// Constant Definitions. Something to get started.
#define mmperBucket 0.1
#define windspeedperRPM 1.0

// Object Instantiations.
DS3231     RTC;     //Create the DS3231 object
Battery    battery; //Create a battery object

// Volatile global variables.
volatile unsigned int rainpulses = 0;
volatile unsigned int windpulses = 0;
volatile bool measurement = false;

// Global Variables.
char buffer[257] ="";


void setup () {
  // Initialize INT0 for accepting interrupts.
  pinMode(INT0pin, INPUT);
  digitalWrite(INT0pin, HIGH);
  // Initialize INT1 for accepting interrupts.
  pinMode(INT1pin, INPUT);
  digitalWrite(INT1pin, HIGH);

  Serial.begin(SerialSpeed);

  Wire.begin();

  RTC.begin();
  attachInterrupt(0, INT0_ISR, FALLING); 
  attachInterrupt(1, INT1_ISR, CHANGE);

  //Enable Interrupt
  RTC.enableInterrupts(EveryMinute); //interrupt at  EverySecond, EveryMinute, EveryHour
 
  // Initialise SD card.    
//    Serial.print("Initializing SD card...");
  Serial.print(F("Initializing SD card."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present."));
    // don't do anything more:
    return;
  }
  Serial.println(F("SD Card initialized."));
}

void loop (){
  if (measurement){
    //Reset our measurment boolian variable.
    measurement = false;
    // Do Measurment Stuff.
    readSensors();
    
    //Future try wireless transmission before store to SD card.
    if(false){
      // Do Wireless Transmission.
      // Check SD for additional results to send while wireless working.
    }
    else{
      // If wireless transmission fails and write to SD is sucessfull clear the buffer.
      if( writeFile( "datalog.txt", buffer) ){
      clearBuffer();
      }
    }
      
    // Heartbeat/Battery update on userLED every 5 minutes.
    battery.ledflashStatus(LEDPin,flashesforfull);
    delay(1000); // 1s break after flashes.
  }
  else{
    // Dont do measurement stuff.
    // Sleep.
    cli(); 
    sleep_enable();      // Set sleep enable bit
    sleep_bod_disable(); // Disable brown out detection during sleep. Saves more power
    sei();
//    Serial.println(F("Sleeping."));    
    delay(10); //This delay is required to allow Serial.print to finish.
    //Shut down all peripherals like ADC before sleep. Refer Atmega328 manual
    power_all_disable(); //This shuts down ADC, TWI, SPI, Timers and USART
    sleep_cpu();         // Sleep the CPU as per the mode set earlier(power down)  
    sleep_disable();     // Wakes up sleep and clears enable bit. Before this ISR would have executed
    power_all_enable();  //This shuts enables ADC, TWI, SPI, Timers and USART
    delay(10); //This delay is required to allow CPU to stabilize
//    Serial.println(F("Awake from sleep"));    

  }
  RTC.clearINTStatus();
}
