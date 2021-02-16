#include <Arduino.h>
#include <unity.h>

int butt[] = {0,1,15,16,17,18,19,22,23,25,32,33};
int channels[] = {3,9,14,15,20,21,22};
boolean on_off[] = {false, false, false, false, false, false, false};
boolean values[12];
int current_note;
int pressure;
int positions[][7] = {{10,9,8,7,6,5,4},{17,16,15,14,13,12,11},{22,21,20,19,18,17,16},{29,28,27,26,25,24,23},{34,33,32,31,30,29,28},
		      {38,37,36,35,34,33,32},{41,40,39,38,37,36,35},{44,43,42,41,40,39,38},{46,45,44,43,42,41,40},
		      {50,49,48,47,46,45,44},{53,52,51,50,49,48,47},{56,55,54,53,52,51,50},{58,57,56,55,54,53,52}};


void setup() {
  Serial.begin(115200);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  playing = false;
}

void set_values() {
  for (int i = 0; i < 12; i++) {
    if (touchRead(butt[i]) > 450) {
      values[i] = true;
    }
  }
  pressure = analogRead(20);
}

int send_partial() {
  int new_partial;
  if (!values[0] && values[1] && values[2] && values[3] && !values[4]) {
    new_partial = 0;
  }
  else if (!values[0] && values[1] && values[2] && !values[3] && !values[4]) {
    new_partial = 1;
  }
  else if (!values[0] && values[1] && !values[2] && !values[3] && !values[4]) {
    new_partial = 2;
  }
  else if (!values[0] && !values[1] && !values[2] && !values[3] && !values[4]) {
    new_partial = 3;
  }
  else if (values[0] && !values[1] && !values[2] && !values[3] && !values[4]) {
    new_partial = 4;
  }
  else if (values[0] && values[1] && !values[2] && !values[3] && !values[4]) {
    new_partial = 5;
  }
  else if (values[0] && values[1] && values[2] && !values[3] && !values[4]) {
    new_partial = 6;
  }
  else if (values[0] && values[1] && values[2] && values[3] && !values[4]) {
    new_partial = 7;
  }
  else if (values[0] && values[1] && values[2] && values[3] && values[4]) {
    new_partial = 8;
  }
  else if (!values[0] && values[1] && values[2] && values[3] && values[4]) {
    new_partial = 9;
  }
  else if (!values[0] && !values[1] && values[2] && values[3] && values[4]) {
    new_partial = 10;
  }
  else if (!values[0] && !values[1] && values[2] && values[3] && !values[4]) {
    new_partial = 11;
  }
  else if (!values[0] && !values[1] && values[2] && !values[3] && !values[4]) {
    new_partial = 12;
  }
  return (new_partial);
}

int send_position() {
  int size = 40;
  int value = analogRead(19);
  int position = round(value/size);
  return (position);
}

void create_note(int note) {
  if (note != current_note) {
    usbMidi.sendNoteOff(current_note, 0, 1, 0);
    usbMidi.sendNoteOn(note, get_pressure, 1, 0);
    current_note = note;
  }
}

void send_note () {
  partial = send_partial();
  position = send_position();
  note = positions[parital][position];
  create_note(note);
}

int get_pressure() {
  value = analogRead(19);
  scale = 5;
  pressure = (value - 40)/scale;
  if (pressure > 127) {
    pressure = 127;
  }
  return (pressure);
}

void send_control_change() {
  for (int i = 5; i < 12; i++) {
    if (values[i]) {
      if (on_off[i]) {
	usbMidi.sendControlChange(0, channels[i], 1, 0);
      }
      else {
	usbMidi.sendControlChange(127, channels[i], 1, 0);
      }
      on_off[i] = !on_off[i];
    }
  }
}

void send_aftertouch() {
  usbMIDI.sendAfterTouch(get_pressure(), 1, 0);
}

void send_values() {
  send_note();
  send_control_change();
  send_aftertouch();
}

void loop() {
  set_values();
  send_values();
  while (usbMidi.read()) {
  }
  delay(100);
}
