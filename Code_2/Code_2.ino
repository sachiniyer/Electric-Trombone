#include "Adafruit_VL53L0X.h"
#include "MIDIUSB.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure;
int joy1x = 10;
int joy1y = 9;
int joy2x = 7;
int joy2y = 11;
int mod1x;
int mod1y;
int mod2x;
int mod2y;
int but1 = 11;
int but2 = 7;
int but3 = 6;
int but4 = 5;
int but5 = 4;
int positions[][7] = {{10,9,8,7,6,5,4},{17,16,15,14,13,12,11},{22,21,20,19,18,17,16},{29,28,27,26,25,24,23},{34,33,32,31,30,29,28},
										 {38,37,36,35,34,33,32},{41,40,39,38,37,36,35},{44,43,42,41,40,39,38},{46,45,44,43,42,41,40},
										 {50,49,48,47,46,45,44},{53,52,51,50,49,48,47},{56,55,54,53,52,51,50},{58,57,56,55,54,53,52}};
void setup() {
  Serial.begin(115200);

  pinMode(joy1x, INPUT);
  pinMode(joy1y, INPUT);
  pinMode(joy2x, INPUT);
  pinMode(joy2y, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  pinMode(but3, INPUT_PULLUP);
  pinMode(but4, INPUT_PULLUP);
  pinMode(but5, INPUT_PULLUP);
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  Serial.println("starting...");

	delay(300);

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  else if (lox.begin()) {
    Serial.println("dw booted");
  }

}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
	MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
	MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
	MidiUSB.flush();
}

void pitchBend(byte channel, int bend) {
	bend = bend * 81.92;
	if (bend > 8192) {
		bend = 8192;
	}
	else if (bend < -8192) {
		bend = -8192;
	}
	unsigned int change = 0x2000 + bend;
	unsigned char low = bend & 0x7F;
	unsigned char high = (bend >> 7) & 0x7F;
	midiEventPacket_t event = {0xE0, 0xE0 | 0, low, high};
	MidiUSB.sendMIDI(event);
	MidiUSB.flush();
}

int get_breath(double pressure) {
  if (pressure < 80) {
    return (0);
  }
	else {
		if (pressure > 499) {
			return (127);
		}
		else {
			return(round(pressure/3.937));
		}
	}
}

int *get_partial() {
	bool b1 = !digitalRead(but1);
  bool b2 = !digitalRead(but2);
  bool b3 = !digitalRead(but3);
  bool b4 = !digitalRead(but4);
  bool b5 = !digitalRead(but5);

	if (!b1 && b2 && b3 && b4 && !b5) {
		return (positions[0]);
	}
	else if (!b1 && b2 && b3 && !b4 && !b5) {
		return (positions[1]);
	}
	else if (!b1 && !b2 && !b3 && !b4 && !b5) {
		return (positions[2]);
	}
	else if (b1 && !b2 && !b3 && !b4 && !b5) {
		return (positions[3]);
	}
	else if (b1 && b2 && !b3 && !b4 && !b5) {
		return (positions[4]);
	}
	else if (b1 && !b2 && b3 && !b4 && !b5) {
		return (positions[5]);
	}
	else if (b1 && b2 && b3 && !b4 && !b5) {
		return (positions[6]);
	}
	else if (b1 && b2 && b3 && b4 && !b5) {
		return (positions[7]);
	}
	else if (b1 && b2 && b3 && b4 && b5) {
		return (positions[8]);
	}
	else if (!b1 && b2 && b3 && b4 && b5) {
		return (positions[9]);
	}
	else if (!b1 && !b2 && b3 && b4 && b5) {
		return (positions[10]);
	}
	else if (!b1 && !b2 && b3 && b4 && !b5) {
		return (positions[11]);
	}
	else if (!b1 && !b2 && b3 && !b4 && !b5) {
		return (positions[12]);
	}
	else {
		return (positions[2]);
	}
}

int get_note() {
	lox.rangingTest(&measure, false);
	int oct = 12;
	int pos = 90;
	int init = 20;
	int dist = measure.RangeMilliMeter;

	if (measure.RangeStatus != 4) {
		if (dist < init + pos/2) {
			return (get_partial()[0] + oct);
		}
		else if (dist < init + pos*1.5) {
			return (get_partial()[1] + oct);
		}
		else if (dist < init + pos*2.5) {
			return (get_partial()[2] + oct);
		}
		else if (dist < init + pos*3.5) {
			return (get_partial()[3] + oct);
		}
		else if (dist < init + pos*4.5) {
			return (get_partial()[4] + oct);
		}
		else if (dist < init + pos*5.5) {
			return (get_partial()[5] + oct);
		}
		else if (dist >= init + pos*5.5) {
			return (get_partial()[6] + oct);
		}
		else {
			return (60);
		}
  }
	else {
		return(60);
  }
}

void play_note(int note, int breath) {
  lox.rangingTest(&measure, false);
	int dist = measure.RangeMilliMeter;
	int cont_breath = 1;
	int partial = get_partial();
	pitchBend(0, 0);
	noteOn(0, note, breath);
  while (cont_breath > 0) {
    delay (1);
		cont_breath = get_breath(analogRead(A0));
		controlChange(0, 2, cont_breath);
		send_mod();
		pitchBend(0, (dist - measure.RangeMilliMeter));
		if (get_partial() != partial) {
			noteOff(0, note, breath);
			note = get_note();
			partial = get_partial();
			noteOn(0, note, breath);
		}
  }
  noteOff(0, note, breath);
	pitchBend(0, 0);
}

void send_mod() {

	int j1x = analogRead(joy1x);
	int j1y = analogRead(joy1y);
	int j2x = analogRead(A2);
	int j2y = analogRead(A4);

	j1x = map(j1x, 0, 1011, 0, 127);
	j1y = map(j1y, 0, 1011, 0, 127);
	j2x = map(j2x, 0, 108, 0, 127);
	j2y = map(j2y, 0, 108, 0, 127);

	if (mod1x != j1x) {
			controlChange(0, 3, j1x);
	}
	if (mod1y != j1y) {
		controlChange(0, 9, j1y);
	}
	if (mod2x > (j2x + 4) || mod2x < (j2x - 4)) {
		controlChange(0, 16, j2x);
	}
	if (mod2y > (j2y + 4) || mod2y < (j2y - 4)) {
		controlChange(0, 17, j2y);
	}

	mod1x = j1x;
	mod1y = j1y;
	mod2x = j2x;
	mod2y = j2y;
}

void loop() {

  int breath = get_breath(analogRead(A0));
	send_mod();
  if (breath > 0) {
	play_note(get_note(), breath);
  }
  Serial.print("breath: ");
  Serial.println(breath);
  Serial.println("JOY 1");
  Serial.print("JOY 1 X: ");
  Serial.println(analogRead(joy1x));
  Serial.print("JOY 1 Y: ");
  Serial.println(analogRead(joy1y));
  Serial.println("JOY 2");
  Serial.print("JOY 2 X: ");
  Serial.println(analogRead(A2));
  Serial.print("JOY 2 Y: ");
  Serial.println(analogRead(A4));
  Serial.print("button1 = ");
  Serial.println(digitalRead(but1));
  Serial.print("button2 = ");
  Serial.println(digitalRead(but2));
  Serial.print("button3 = ");
  Serial.println(digitalRead(but3));
  Serial.print("button4 = ");
  Serial.println(digitalRead(but4));
  Serial.print("button5 = ");
  Serial.println(digitalRead(but5));
  Serial.print("breath" );
  Serial.println(analogRead(A0));
  delay (1);
}
