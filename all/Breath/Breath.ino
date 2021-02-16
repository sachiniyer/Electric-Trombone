
void setup() {
	Serial.begin(115200);
	while (!Serial) {
		delay(1);
	}
}

void loop() {
	Serial.println(analogRead(A0));
}
