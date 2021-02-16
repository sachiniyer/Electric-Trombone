#include "Adafruit_VL53L0X.h"
#include "MIDIUSB.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  else if (lox.begin()) {
    Serial.println("dw booted");
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
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

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

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
  int x;
  x = round((measure.RangeMilliMeter)/2.756);
  Serial.println("Sending note on");
  noteOn(0, x , 64);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(50);
  Serial.println("Sending note off");
  noteOff(0, x , 64);  // Channel 0, middle C, normal velocity
  MidiUSB.flush();

  // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65
}
