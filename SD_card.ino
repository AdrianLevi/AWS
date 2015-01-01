// Construct String to transmit or save for later resend.

// Write sensor values to SD with Timestring
bool writeFile(char *fileName,char *lineText){
  // Open the output file
  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(lineText);
    dataFile.close();
    // print to the serial port too:
    Serial.print(lineText);
    return(true);
  }
  else {
    //Serial.println("error opening datalog.txt");
    return(false);
  } 
}

// Read a line from the file into the buffer.
bool readFile(char *fileName){
  File dataFile = SD.open("datalog.txt");

  // if the file is available, read from it:
  if (dataFile) {
    // 1st test for data availability.
    if (dataFile.available() && (dataFile.peek() != 'EOF')) {
      int i = -1;
      do {
        i++;
        buffer[i] = dataFile.read();
        // while i less than the size of the buffer and the next character isn't end of file.
      } while ( i < sizeof(buffer) && (dataFile.peek() != 'EOF') );
      // The data is in the char buffer array.
      return true;
    }
    dataFile.close();
  } else {
    // Could not open the file.
    return false;
  }
}


// Return true if the read character is'nt EOF.
bool isData(char *fileName){
  File dataFile = SD.open(fileName, FILE_WRITE);
  // if the file is available.
  if (dataFile) {
    if ( dataFile.peek() != 'EOF' ) {
      return true;
    }
  }
  dataFile.close();
  return(false);
}

// Clear our buffer of extraneous information. (Possibly not necessary?)
void clearBuffer(){
  for (char i = 0 ; i < sizeof(buffer) ; i++ ){
  buffer[i] = ' ';
  }
}
