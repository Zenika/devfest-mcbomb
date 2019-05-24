#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>


#define BUTTON  7
#define LED     4
#define BUZZER  5

int WIRE_1 = A0;
int WIRE_2 = A1;
int WIRE_3 = A2;

Adafruit_7segment matrix = Adafruit_7segment();

double GAME_DURATION = 60.00;

double _countdown = GAME_DURATION;
double speed = 1.00;

double TIME_STEP = 0.02;
enum e_state {STARTING, PLAYING, WINNING, LOOSING, ENDING};
double STARTING_DURATION = 3.00;
double PLAYING_DURATION = 60.00;
double WINNING_DURATION = 10.00;
double LOOSING_DURATION = 10.00;

int led_frequency = 1;
int led_state = LOW;

void setup() {
  // input pin for digitalRead()
  pinMode(WIRE_1, INPUT);
  pinMode(WIRE_2, INPUT);
  pinMode(WIRE_3, INPUT);
  pinMode(BUTTON, INPUT);

  // output pin for digitalWrite()
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);


  Wire.begin();                       /* Initialise le bus i2C */
  Serial.begin(9600);                 /* Initialise le port serie */
//  while (!Serial);                    /* Attends la liaison du port série */
  Serial.println("Serial is ready");  

  matrix.begin(0x70);                 /* Adresse I2C de l'afficheur 7 segments */
  matrix.setBrightness(2);

  Serial.println("Application Up !!!");
}


int state = ENDING;
double main_time = 0;
double led_time = 0;
void loop() {
  switch(state){
    case STARTING:
      starting();
      break;
    case PLAYING:
      playing();
      blinkLed();
      break;
    case WINNING:
      winning();
      break;
    case LOOSING:
      loosing();
      break;
    case ENDING: 
      ending();
      break;
  }

  delay(TIME_STEP * 1000 / speed);
  main_time += TIME_STEP;
  led_time += TIME_STEP;
}

void starting() {
//  Serial.println("STARTING");
  display(_countdown);
  delay(STARTING_DURATION * 1000);
  changeState(PLAYING);
}

void playing() {
//  Serial.println("PLAYING");
  _countdown -= TIME_STEP;

  if (digitalRead(WIRE_2) == LOW) {
    speed = 2;
  }
  if (digitalRead(WIRE_1) == LOW) {
    changeState(WINNING);
  }
  if (_countdown <= 0.00 || digitalRead(WIRE_3) == LOW) {
    changeState(LOOSING);
  }

  display(_countdown);

//  delay(PLAYING_DURATION * 1000);
//  changeState(WINNING);
}

void winning() {
//  Serial.println("WINNING");
  matrix.setBrightness(0);

  switchLedOff();

//  display(_countdown);
  delay(WINNING_DURATION * 1000);
  changeState(ENDING);
}

void loosing() {
//  Serial.println("LOOSING");

  switchLedOff();

  display(8888);
  matrix.blinkRate(HT16K33_BLINK_2HZ);
  matrix.setBrightness(15);
  
  if (main_time >= LOOSING_DURATION) {
    changeState(ENDING);
  }
}

void ending() {
//  Serial.println("ENDING");
  
  display(10000); // displays "----" on the 7seg
  matrix.blinkRate(HT16K33_BLINK_OFF);
  matrix.setBrightness(2);

  if (digitalRead(BUTTON) == HIGH) {
    reset();
  }
  
  delay(100);
}

void reset() {
  _countdown = PLAYING_DURATION;
  speed = 1;
  led_frequency = 1;
  randomizeTriggers();
  changeState(STARTING);
}

void randomizeTriggers() {
  randomSeed(millis());
  WIRE_1 = random(A0, A2+1);
  WIRE_2 = WIRE_1;
  while (WIRE_2 == WIRE_1) {
    WIRE_2 = random(A0, A2+1);
    WIRE_3 = WIRE_2;
  }
  while (WIRE_3 == WIRE_1 || WIRE_3 == WIRE_2) {
    WIRE_3 = random(A0, A2+1);
  }
  Serial.println(WIRE_1);
  Serial.println(WIRE_2);
  Serial.println(WIRE_3);
}


void changeState(int _state) {
  state = _state;
  main_time = 0;
}

void switchLedOn() {
  Serial.println("LIGHT ON");
  led_state = HIGH;
  digitalWrite(LED, led_state);
}

void switchLedOff() {
  Serial.println("LIGHT OFF");
  led_state = LOW;
  digitalWrite(LED, led_state);
}

void blinkLed() {
  // do I have to change the LED state ?
  if (led_time >= (0.5/led_frequency)) {
    led_time = 0;
    if (led_state == HIGH) {
      switchLedOff();
    } else if (led_state == LOW) {
      switchLedOn();
    }
  }

  if (_countdown <= 20.00) {
    led_frequency = 2;
  }
  if (_countdown <= 10.00) {
    led_frequency = 4;
  }
  if (_countdown <= 5.00) {
    led_frequency = 8;
  }
  if (_countdown <= 2.00) {
    led_frequency = 1000;
  }
}

/**
  Display remaining time on Adafruit_7segment
*/
void display(double value) {
  matrix.println(value, 2);
  matrix.writeDisplay();
}
