//
// Takes analog pin number, returns temperature.
float readLM335 (char pin){
  /* Analogue reading is taken, between 0 (0v) and 1023(3.3v),
   * This is then scaled to a voltage (* 3300/1024).
   * This answer is in degK, we then subtract 273.15deg to get to C
   * float raw = analogRead(pin);
   * millivolts= (3.3 * raw / 1024.0);
   * kelvin = (millivolts*100);
   * celsius = kelvin - 273.15;
   */
  return(((3.3 * analogRead(pin) / 1024.0) * 100) - 273.15);
} 

// Read all sensors into variables
//PString readSensors(){
void readSensors(){
  static int rainFall;
  static int windSpeed;
  PString str(buffer, sizeof(buffer));
  
  // Print timestamp to string.
  DateTime now = RTC.now();
  str.print(now.year(), DEC);
  str.print(now.month(), DEC);
  str.print(now.date(), DEC);
  str.print(now.hour(), DEC);
  str.print(now.minute(), DEC);
  str.print(now.second(), DEC);
  str.print(F(","));
  
  // Read RTC temp.
  RTC.convertTemperature();             // ensure that the current temp is encoded to the registers.
  str.print(RTC.getTemperature());
  str.print(F(","));

  // Get LM335 Temp.
  str.print(readLM335(lm335pin));
  str.print(F(","));

  // Battery Percentage.
  battery.update();
  str.print(battery.getVoltage());
  str.print(F(","));
  
  str.print(battery.getChStatus());
  str.print(F(","));
  // Disable interrupts while we collect wind and rain values.
  noInterrupts();
  rainFall = rainpulses;
  rainpulses = 0;
  windSpeed = windpulses;
  windpulses = 0;
  interrupts();
  // Interrupts re enabled.
  
  rainFall = rainFall * mmperBucket;
  str.print(rainFall);
  str.print(F(","));
  
  windSpeed = windpulses * windspeedperRPM;
  str.print(windSpeed);
  str.print(F(","));
  
  // Wind direction.
  
  // Barometer.
  
  // End by printing a newline character.
  str.println(F(","));
  // String updated into global buffer[257]
}

bool batteryCharging(void){
  return(battery.isCharging());
}


