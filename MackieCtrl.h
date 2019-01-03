
#ifndef MackieCtrl_h
#define MackieCtrl_h

#include <WProgram.h>






//check for incoming midi messages
void MidiMessages();
void BlinkLed(int times);


//respond to incoming control changes
void handleControlChange(byte channel, byte control, byte value);//event handler midi control change
void handleSysEx(const byte* sysExData, uint16_t sysExSize, bool complete);


#endif //MackieCtrl_h
