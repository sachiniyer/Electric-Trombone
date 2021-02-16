
// Declare the pins for the Button and the LED<br>
int but1 = 11;
int but2 = 7;
int but3 = 6;
int but4 = 5;
int but5 = 4;
int joy1x = 10;
int joy1y = 9;
int joy1b = 8;
int joy2x = 7;
int joy2y = 11;
int joy2b = 12;
void setup() {
   // Define pin #12 as input and activate the internal pull-up resistor
   pinMode(but1, INPUT_PULLUP);
   pinMode(but2, INPUT_PULLUP);
   pinMode(but3, INPUT_PULLUP);
   pinMode(but4, INPUT_PULLUP);
   pinMode(but5, INPUT_PULLUP);
     digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
    pinMode(A1, OUTPUT);
  pinMode(0, OUTPUT);
}

void loop(){
   // Read the value of the input. It can either be 1 or 0
   int button1 = digitalRead(but1);
   int button2 = digitalRead(but2);
   int button3 = digitalRead(but3);
   int button4 = digitalRead(but4);
   int button5 = digitalRead(but5);
   if (button1 == LOW || button2 == LOW || button3 == LOW || button4 == LOW || button5 == LOW){
      // If button pushed, turn LED on
      digitalWrite(LED_BUILTIN,HIGH);
   } else {
      // Otherwise, turn the LED off
      digitalWrite(LED_BUILTIN, LOW);
   }
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
     Serial.println("JOY 1");
  Serial.print("JOY 1 X: ");
  Serial.println(analogRead(joy1x));
  Serial.print("JOY 1 Y: ");
  Serial.println(analogRead(joy1y));
  Serial.print("JOY 1 Button: ");
  Serial.println(analogRead(joy1b));
  Serial.println("JOY 2");
  Serial.print("JOY 2 X: ");
  Serial.println(analogRead(A2));
  Serial.print("JOY 2 Y: ");
  Serial.println(analogRead(A4));
  Serial.print("JOY 2 Button: ");
  Serial.println(analogRead(A5));
   delay(200);
}
