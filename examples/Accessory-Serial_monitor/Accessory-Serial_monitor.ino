//******************************************************************************************************
//
//                       Testing the Accesory Decoder part of the AP_DCC_library
//
// purpose:   This sketch shows what information is received from Accesory Decoder Commands
//            Results are displayed on the serial monitor
// author:    Aiko Pras
// version:   2021-06-01 V1.0 ap initial version
//            2024-12-11 V1.1 ap clarification of the switch position
//
// usage:     This sketch should declare the following objects:
//            - extern Dcc           dcc;     // The main DCC object
//            - extern Accessory     accCmd;  // To retrieve the data from accessory commands
//            Note the 'extern' keyword, since these objects are instantiated in DCC_Library.cpp
//
//            Setup() should call dcc.attach(dccPin). dccPin is the interrupt pin for the DCC signal
//            The main loop() should call dcc.input() as often as possible. If there is input,
//            dcc.cmdType tells what kind of command was received.
//            In this sketch we react on MyAccessoryCmd and AnyAccessoryCmd).
//
//
// hardware:  - Timer 2 or TCB0 is used
//            - a free to chose interrupt pin (dccpin) for the DCC input signal
//
// This source file is subject of the GNU general public license 3,
// that is available at the world-wide-web at http://www.gnu.org/licenses/gpl.txt
//
//******************************************************************************************************
#include <Arduino.h>
#include <AP_DCC_library.h>

const uint8_t dccPin = 3;        // Pin 3 on the UNO, Nano etc. is INT1 / PIN_PD1 is INT1 and Digital Pin 20 on the ATMega2560

extern Dcc dcc;                  // This object is instantiated in DCC_Library.cpp
extern Accessory accCmd;         // To retrieve data from accessory commands

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  delay(1000);
  Serial.println("Test DCC lib - Accesory Commands");
  dcc.attach(dccPin);
  
  // For testing, the following variables can be changed
  accCmd.myMaster = OpenDCC;
  //accCmd.myMaster = Lenz;
  
  // Set Accessory address. We may also specify an address range.
  // Note: my decoder address = output (switch) address / 4
  //accCmd.setMyAddress(24);    // Decoder 24 is switch 97..100
  accCmd.setMyAddress(0,511);   // We listen to all addresses!
}


void loop() {
  if (dcc.input()) {
    switch (dcc.cmdType) {

      case Dcc::MyAccessoryCmd :
        if (accCmd.command == Accessory::basic) {
          Serial.println(" Basic accessory command for my decoder");
          Serial.print(" - Decoder Address: ");
          Serial.println(accCmd.decoderAddress);
          Serial.print(" - Turnout: ");
          Serial.println(accCmd.turnout);
          Serial.print(" - Switch number: ");
          Serial.print(accCmd.outputAddress);
          if (accCmd.position == 1) Serial.println(" 1");  // 1 = Straight / Green / LH10X: +
            else Serial.println(" 0");                     // 0 = Diverging / Red / LH10X: -
          if (accCmd.activate) Serial.println(" - Activate");
          Serial.println();
        }
        else {
          Serial.println(" Extended accessory command for my decoder");
          Serial.print(" - Output Address: ");
          Serial.println(accCmd.outputAddress);
          Serial.print(" - SignalHead: ");
          Serial.println(accCmd.signalHead);
       }
      break;

      case Dcc::AnyAccessoryCmd :
        Serial.print(accCmd.decoderAddress);
        Serial.println(" (any)");
      break;

      default:
      break;

    }
  }
}
