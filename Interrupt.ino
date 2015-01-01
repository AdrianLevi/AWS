
//  http://gammon.com.au/interrupts

// RTC Timer for measurement time.
// Interrupt service routine for external interrupt on INT0 pin conntected to PD2
void INT0_ISR(){
  static char minutecount;
  minutecount++;
  if (minutecount == 5) {
    measurement = true;
    minutecount = 0;
  }
  else{
    // Processor woken from sleep every minute.
  }
}

// Rain Guage.
// Interrupt service routine for external interrupt on INT1 pin conntected to PD3
void INT1_ISR(){
   // Increment Rain Guage count.
   rainpulses++;
}

// Wind Speed.
// Not using this yet, Just leaving this here as a template.
// I might include this into INT1_ISR with some smarts to differentiate between the 2.
ISR (PCINT2_vect){
   // Increment Anemometer.
   windpulses++;
}
