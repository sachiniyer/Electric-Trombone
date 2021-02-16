#include "Adafruit_VL53L0X.h"
#include "MIDIUSB.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("starting...");
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  else if (lox.begin()) {
    Serial.println("dw booted");
  }
  // power

}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    if (measure.RangeMilliMeter < 200 && measure.RangeMilliMeter != 0){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  } else {
    Serial.println(" out of range ");
    digitalWrite(LED_BUILTIN, LOW);
  }
  int breath = round(analogRead(A0)/3.937);
  int cont_breath;
  if (breath*3.937 < 60) {
    breath = 0;
  }
  int note = round((measure.RangeMilliMeter)/2.756);
  if (note == 0) {
    note = 1;
  }
  else if (note > 127) {
    note = 127;
  }

  Serial.print("note: ");
  Serial.println(note);
  Serial.print("breath: ");
  Serial.println(breath);
  Serial.println(measure.RangeMilliMeter);
  Serial.println(analogRead(A0));
  if (breath > 0) {
    breath = breath + 60;
    if (breath > 127) {
      breath = 127;
    }
    noteOn(0, note, breath);   // Channel 0, middle C, normal velocity
    MidiUSB.flush();
    cont_breath = analogRead(A0);
    while (cont_breath > 60) {
      delay (10);
      cont_breath = analogRead(A0);
    }
    noteOff(0, note, breath);  // Channel 0, middle C, normal velocity
    MidiUSB.flush();
  }

  delay (10);
}
