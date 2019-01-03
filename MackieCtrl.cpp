#include "MackieCtrl.h"

// Logic Control system exclusive messages
// changed serialnumber of Logic control from 48 to 49
const byte sysExHeader[5]                       = {0xF0,0x00,0x00,0x66,0x10};
const byte sysExDeviceQuery[7]                  = {0xF0,0x00,0x00,0x66,0x10,0x00,0xF7};
const byte sysExHostConnectionQuery[18]         = {0xF0,0x00,0x00,0x66,0x10,0x01,0x49,0x41,0x31,0x33,0x35,0x36,0x38,0x64,0x7A,0x61,0x41,0xF7};
const byte sysExHostConnectionConfirmation[14]  = {0xF0,0x00,0x00,0x66,0x10,0x03,0x49,0x41,0x31,0x33,0x35,0x36,0x38,0xF7};
const byte sysExVersionReply[12]                = {0xF0,0x00,0x00,0x66,0x10,0x14,0x56,0x31,0x2E,0x30,0x32,0xF7};
// Logic Control XT system exclusive messages
const byte sysExHeaderXT[5]                       = {0xF0,0x00,0x00,0x66,0x11};
const byte sysExDeviceQueryXT[7]                  = {0xF0,0x00,0x00,0x66,0x11,0x00,0xF7};
const byte sysExHostConnectionQueryXT[18]         = {0xF0,0x00,0x00,0x66,0x11,0x01,0x49,0x41,0x31,0x33,0x35,0x36,0x38,0x64,0x7A,0x61,0x41,0xF7};
const byte sysExHostConnectionConfirmationXT[14]  = {0xF0,0x00,0x00,0x66,0x11,0x03,0x49,0x41,0x31,0x33,0x35,0x36,0x38,0xF7};
const byte sysExVersionReplyXT[12]                = {0xF0,0x00,0x00,0x66,0x11,0x14,0x56,0x31,0x2E,0x30,0x32,0xF7};
// char array for incoming text message, used for message identification
char messageText[112];
// array for Page message recognition "     Page"
const char pageMessageHeader[10]={"     Page"};
const char insertMessageHeader[9]={" Insert "};


//check for incoming midi messages
void MidiMessages(){
   if(usbMIDI.read()){ //true if new midi message
    BlinkLed(1);
    //Serial.println(usbMIDI.getType());//check if anything is received
   }
}

void BlinkLed(int times)
{
  for(int i=0;i<times;i++)
  {
    digitalWrite(13, LOW);
    digitalWrite(13, HIGH);
  }
}

//respond to incoming control changes
void handleControlChange(byte channel, byte control, byte value){
  BlinkLed(4);
  switch(control){
    //case midiCC:
      //setMidiPos(value,128);
      //return;
     default:
      Serial.println("Control:" + control);
      return;
  }
}

void handleSysEx(const byte* sysExData, uint16_t sysExSize, bool complete){
  uint8_t midiPort;
  boolean isMackieControlMessage = true;
  boolean isMackieControlXTMessage = true;

  midiPort = usbMIDI.getCable();
  
  switch (midiPort) {
    // Logic Control Main on Port 0
    case 0:
      // compare the first five bytes of the message to the sysExHeader to check if it is a Mackie control message
      for (int i=0;i<5;i++) {
        Serial.print(sysExData[i]);
        if (sysExData[i]!=sysExHeader[i]) {
          isMackieControlMessage = false;
          Serial.println(" Sysex, not Mackie." );
        }
      }
      // handle mackie control message
      if (isMackieControlMessage) {
        Serial.print(" Sysex, Mackie: ");
        Serial.println(sysExData[5]);
        // handle different message types
        switch (sysExData[5]) {
          // respond to device query
          case 0:
            usbMIDI.sendSysEx(18, sysExHostConnectionQuery,true,0);
            break;
          // respond to host connection reply with connection confirmation
          case 2:
            usbMIDI.sendSysEx(14, sysExHostConnectionConfirmation,true,0);
            break;
          // respond to firmware version request
          case 19:
            usbMIDI.sendSysEx(12, sysExVersionReply,true,0);
            break;
          // handle LCD display update
          case 18:
            // get message text for determination of message type
            for (int i=7;i<sysExSize-1;i++) {
                messageText[i-7]= sysExData[i];
                Serial.print(sysExData[i]);
              }
            Serial.println(" ");
          break;
            // handle LED display update
          case 16:
            // update led display
            break;
          case 97:
            //faders to minimum
            break;
          case 98:
            //all leds off
            break;
          case 99:
            //reboot in offline mode
            break;
          default: 
            // statements
          break;
        }
      }
      break;
    // Logic Control XT on Port 1
    case 1:
      // compare the first five bytes of the message to the sysExHeader to check if it is a Mackie control XT message
      for (int i=0;i<5;i++) {
        if (sysExData[i]!=sysExHeaderXT[i]) {
          isMackieControlXTMessage = false;
          Serial.println("Sysex, not ControlXT:" );
        }
       }
      // handle mackie control message
      if (isMackieControlXTMessage) {
        Serial.println("Sysex, ControlXT:");
          // handle different message types
        switch (sysExData[5]) {
          // respond to device query
          case 0:
            usbMIDI.sendSysEx(18, sysExHostConnectionQueryXT,true,1);
            break;
          // respond to host connection reply with connection confirmation
          case 2:
            usbMIDI.sendSysEx(14, sysExHostConnectionConfirmationXT,true,1);
            break;
          // respond to firmware version request
          case 19:
            usbMIDI.sendSysEx(12, sysExVersionReplyXT,true,1);
            break;
          // handle LCD display update
          case 18:
            // get message text for determination of message type
            break;
          case 97:
            //faders to minimum
            break;
          case 98:
            //all leds off
            break;
          case 99:
            //reboot in offline mode
            break;
          default: 
            // statements
          break;
        }
      }       
    break;
    // Synth control on Port 2
    case 2:
      // nothing yet
    break;
    default: 
      // ignore sysExData
    break;
  }
}
