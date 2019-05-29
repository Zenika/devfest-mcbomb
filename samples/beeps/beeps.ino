
#define BUZZER 11
#define BUTTON 7

unsigned long one_second_to_micros = 1000000;



void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);

  Serial.begin(9600);
  while(!Serial);
  Serial.println("Application is running");
}

void loop() {
  beep();
  button();
}

unsigned long beep_duration = 100000;
unsigned long beep_start = micros();
unsigned long beep_frequency = 1;
  
void beep() {
  unsigned long beep_micros = micros() - beep_start;
  if (beep_micros >= beep_duration) {
    noTone(BUZZER);
  } else {
    tone(BUZZER, 1000);
  }

  if (beep_micros >= one_second_to_micros/beep_frequency) {
    beep_start = micros();
  }
}


int button_state = LOW;

void button() {
  if (digitalRead(BUTTON) == HIGH && button_state == LOW) {
    button_state = HIGH;
    beep_frequency *= 2;
    Serial.println("double");

  }
  if (digitalRead(BUTTON) == LOW) {
    button_state = LOW;
  }
}
