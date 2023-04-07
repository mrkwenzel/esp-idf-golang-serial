#include "Arduino.h"

extern "C" void app_main()
{
  initArduino();

  char rx_byte;

  String stringOne = String('\0');

  // Arduino-like setup()
  Serial.begin(115200);

  // Arduino-like loop()
  while(true){
    // if there's any serial available, read it:
    if (Serial.available() > 0) {
      rx_byte = Serial.read();
          
      printf("=====> ");
      printf("%d", (int)rx_byte);
      printf("\n");
      
      if (rx_byte == EOF || rx_byte == '\r' || rx_byte == '\n') {
        printf("you typed: ");
        printf(stringOne.c_str());
        printf("\n");

        printf("len: ");
        printf("%d", stringOne.length());
        printf("\n");
        
        stringOne = String('\0');
      } else {
        stringOne.concat(rx_byte);
      }

    }
  }

  delay(10);

  // WARNING: if program reaches end of function app_main() the MCU will restart.
}