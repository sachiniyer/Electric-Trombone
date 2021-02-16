#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure;
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  // wait until serial port opens for native USB devices
  /*
  while (! Serial) {
    delay(1);
  }
  */
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

int readthings() {
	lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
     return(measure.RangeMilliMeter);
    if (measure.RangeMilliMeter < 200 && measure.RangeMilliMeter != 0){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  } else {
    return(1000000000000000);
    digitalWrite(LED_BUILTIN, LOW);
  }
}


void loop() {


  Serial.print("Reading a measurement... ");
	Serial.println(readthings());

  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);

  delay(10);
}
