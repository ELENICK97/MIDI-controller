//FADERS
#define pot1_CC     50
#define pot2_CC     51
#define pot3_CC     52
#define pot4_CC     53
#define pot5_CC     54
#define pot6_CC     55
#define pot7_CC     56
#define pot8_CC     57
#define pot9_CC     58
//ROTARY POTS
#define pot10_CC    59
#define pot11_CC    60
#define pot12_CC    61
#define pot13_CC    62
#define pot14_CC    63
#define pot15_CC    64
#define pot16_CC    65
#define pot17_CC    66
//JoyStick
#define modWheel_CC 1
//LATCHED SWITCHES
#define S1_CC       67
#define S2_CC       68
#define S3_CC       69
#define S4_CC       70
#define S5_CC       71
#define S6_CC       72
#define S7_CC       73
#define S8_CC       74
//MOMENTARY PUSHBUTTONS
#define SW1_CC      75
#define SW2_CC      76
#define SW3_CC      77
#define SW4_CC      78
#define SW5_CC      79
#define SW6_CC      80
#define SW7_CC      81
#define SW8_CC      82


#include <Control_Surface.h>
#include <SoftwareSerial.h>
#include "C:\Users\xxxxxx\Documents\Arduino\libraries\Control_Surface\src\MIDI_Interfaces\SoftwareSerialMIDI_Interface.hpp"


SoftwareSerial SFSer1(8, -1); 

USBMIDI_Interface midi_usb;
SoftwareSerialMIDI_Interface midi_ser = {SFSer1, MIDI_BAUD};
MIDI_Pipe pipe_ctrl_USB, pipe_DIN_USB; 

CD74HC4051 mux_pot1 { //U5
  A0,
  {13, 12, 11}
};

CD74HC4051 mux_pot2 { //U2
  A1,
  {6, 5, 4}
};

CD74HC4051 mux_SW1 {  //U3
  7,
  {3, 2, 1}
};

CD74HC4051 mux_SW2 {  //U4
  A5,
  {0, 9, 10}
};



CCPotentiometer potentiometers[]{ 
//FADERS
  {mux_pot2.pin(4),{pot1_CC, Channel_1}}, //RV1
  {mux_pot2.pin(5),{pot2_CC, Channel_1}}, //RV2
  {mux_pot2.pin(6),{pot3_CC, Channel_1}}, //RV3
  {mux_pot2.pin(7),{pot4_CC, Channel_1}}, //RV4
  {mux_pot1.pin(3),{pot5_CC, Channel_1}}, //RV5
  {mux_pot1.pin(7),{pot6_CC, Channel_1}}, //RV6
  {mux_pot1.pin(5),{pot7_CC, Channel_1}}, //RV7
  {mux_pot1.pin(6),{pot8_CC, Channel_1}}, //RV8 
  {mux_pot1.pin(4),{pot9_CC, Channel_1}}, //RV9
//ROTARY POTS
  {mux_pot2.pin(0),{pot10_CC, Channel_1}}, //RV10
  {mux_pot2.pin(1),{pot11_CC, Channel_1}}, //RV11
  {mux_pot2.pin(2),{pot12_CC, Channel_1}}, //RV12
  {mux_pot2.pin(3),{pot13_CC, Channel_1}}, //RV13
  {mux_pot1.pin(2),{pot14_CC, Channel_1}}, //RV14
  {mux_pot1.pin(1),{pot15_CC, Channel_1}}, //RV15
  {mux_pot1.pin(0),{pot16_CC, Channel_1}}, //RV16
  {A2, {pot17_CC, Channel_1}}, //RV17
  {A3, {modWheel_CC, Channel_1}},  //potH joystick
};

PBPotentiometer PitchBend2 {  //potV joystick
  A4,        // Analog pin connected to potentiometer
  Channel_1, // MIDI Channel 1
};

CCButton buttons[]{
  {mux_SW1.pin(1), {S1_CC, Channel_1}}, //S1
  {mux_SW1.pin(6), {S2_CC, Channel_1}}, //S2
  {mux_SW1.pin(2), {S3_CC, Channel_1}}, //S3
  {mux_SW1.pin(4), {S4_CC, Channel_1}}, //S4
  {mux_SW2.pin(1), {S5_CC, Channel_1}}, //S5
  {mux_SW2.pin(2), {S6_CC, Channel_1}}, //S6
  {mux_SW2.pin(4), {S7_CC, Channel_1}}, //S7
  {mux_SW2.pin(6), {S8_CC, Channel_1}}, //S8
};



CCButton pushbuttons[]{
  {mux_SW1.pin(0), {SW1_CC, Channel_1}},  //SW1
  {mux_SW1.pin(3), {SW2_CC, Channel_1}},  //SW2
  {mux_SW1.pin(5), {SW3_CC, Channel_1}},  //SW3
  {mux_SW1.pin(7), {SW4_CC, Channel_1}},  //SW4

  {mux_SW2.pin(0), {SW5_CC, Channel_1}},  //SW5
  {mux_SW2.pin(3), {SW6_CC, Channel_1}},  //SW6
  {mux_SW2.pin(5), {SW7_CC, Channel_1}},  //SW7
  {mux_SW2.pin(7), {SW8_CC, Channel_1}},  //SW8
};


/*
analog_t mappingFunction(analog_t raw) {
  return map(raw, 0, 16383, 16383, 0);
}
*/


void setup() {

  midi_usb.setAsDefault();
  Control_Surface >> pipe_ctrl_USB >> midi_usb; //Control surface sends messages to USB interface
  midi_ser >> pipe_DIN_USB >> midi_usb;         //MIDI coming from the DIN socket sends messages to USB interface

  for (int i=9; i<17; i++){   // Faders are left inverted for Hammond drawbars 

    //potentiometers[i].map(mappingFunction);
    potentiometers[i].invert();
  }
  //MIDI_Interface::beginAll();
  Control_Surface.begin();


}

void loop() {
  Control_Surface.loop();
  //MIDI_Interface::updateAll();

  //delay(5);
}
