//******************************************************************************************************
//
//                       Testing the Loco Decoder part of the AP_DCC_library
//
// purpose:   This sketch shows what information is received from Loco Decoder Commands
//            Results are displayed on the serial monitor
// author:    Aiko Pras
// version:   2021-06-01 V1.0 ap initial version
//
// usage:     This sketch should declare the following objects:
//            - extern Dcc           dcc;     // The main DCC object
//            - extern Loco          locoCmd;  // To retrieve the data from Loco commands
//
//            Setup() should call dcc.begin(dccPin). dccPin is the interrupt pin for the DCC signal
//            The main loop() should call dcc.input() as often as possible. If there is input,
//            dcc.cmdType tells what kind of command was received.
//            In this sketch we react on TODO.
//
//
// hardware:  - Timer 2 is used
//            - a free to chose interrupt pin (dccpin) for the DCC input signal
//            - a free to chose digital output pin for the DCC-ACK signal
//
// This source file is subject of the GNU general public license 2,
// that is available at the world-wide-web at http://www.gnu.org/licenses/gpl.txt
//
//******************************************************************************************************
#include <Arduino.h>
#include <AP_DCC_library.h>

//const uint8_t dccPin = 11;       // Pin 11 on the ATMega 16 is INT1
const uint8_t dccPin = 20;       // Pin 20 on the ATMega2560 - Liftdecoder is INT1

extern Dcc dcc;                  // This object is instantiated in DCC_Library.cpp
extern Loco locoCmd;             // To retrieve the data from loco commands  (7 & 14 bit)


unsigned long onTime;            // Timeout for the LED used to indicate if trains are running.


void setup() {
  dcc.begin(dccPin);
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  Serial.println("");
  // For testing, the following variables can be changed
  locoCmd.myLocoAddressFirst = 5000;
  locoCmd.myLocoAddressLast = 5000;
  delay(1000);
  Serial.println("Test DCC lib - Loco commands");
}


void loop() {
  if (dcc.input()) {
    switch (dcc.cmdType) {

      case Dcc::ResetCmd :
        Serial.println("Reset command (all engines stop)");
      break;

      case Dcc::SomeLocoMovesFlag :
        // We set the LED and clear it after some time
        digitalWrite(LED_BUILTIN, HIGH);
        onTime = millis();
      break;

      case Dcc::MyLocoSpeedCmd :
        Serial.print("Loco speed command for my decoder address: ");
        Serial.println(locoCmd.address);
        Serial.print(" - Speed: ");
        Serial.println(locoCmd.speed);
        Serial.print(" - Direction: ");
        if (locoCmd.forward) Serial.println(" Forward");
          else Serial.println(" Reverse");
        Serial.println();
        if (locoCmd.speed > 0) {
          // We set the LED and clear it after some time
          digitalWrite(LED_BUILTIN, HIGH);
          onTime = millis();
        }
      break;

      case Dcc::MyEmergencyStopCmd:
        Serial.println("Emergency stop command for this loco");
      break;

      case Dcc::MyLocoF0F4Cmd:
        Serial.print("F0-F4 command. Value: ");
        Serial.println(locoCmd.F0F4);
      break;

      case Dcc::MyLocoF5F8Cmd:
        Serial.print("F5-F5 command. Value: ");
        Serial.println(locoCmd.F5F8);
      break;

      case Dcc::MyLocoF9F12Cmd:
        Serial.print("F9-F12 command. Value: ");
        Serial.println(locoCmd.F9F12);
      break;

      case Dcc::MyLocoF13F20Cmd:
        Serial.print("F13-F20 command. Value: ");
        Serial.println(locoCmd.F13F20);
      break;

      case Dcc::MyLocoF21F28Cmd:
        Serial.print("F21-F28 command. Value: ");
        Serial.println(locoCmd.F21F28);
      break;

    }
  }

  if ((millis() - onTime) > 5000) {
    digitalWrite(LED_BUILTIN, LOW);  
  }
  
}
