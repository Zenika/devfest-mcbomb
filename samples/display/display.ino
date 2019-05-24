#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>


#define BUTTON 7
#define LED 4

Adafruit_7segment matrix = Adafruit_7segment();

double GAME_DURATION = 60.00;

double _countdown = GAME_DURATION;
double _time_step = 0.02;
double speed = 1.00;


void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);


  Wire.begin();                       /* Initialise le bus i2C */
  Serial.begin(9600);                 /* Initialise le port serie */
  while (!Serial);                    /* Attends la liaison du port série */
  Serial.println("Serial is ready");  

  matrix.begin(0x70);                 /* Adresse I2C de l'afficheur 7 segments */
  matrix.setBrightness(2);
  display(_countdown);

  Serial.println("Application Up !!!");
}

int led_state = HIGH;
void loop() {
  if (digitalRead(BUTTON) == HIGH) {
    if (speed < 2 && _countdown < 58.00){
      speed = 2;
    }
    if (_countdown == -1) {
      speed = 1;
      _countdown = GAME_DURATION;
    }
  }

  if (_countdown == -1) {
    return;
  } else if (_countdown <= 0 ) {
    display(8888);
    matrix.blinkRate(HT16K33_BLINK_2HZ);
    matrix.setBrightness(15);
    
    delay(5000);
    
    display(10000); // displays "----" on the 7seg
    matrix.blinkRate(HT16K33_BLINK_OFF);
    matrix.setBrightness(2);

    _countdown=-1;
    return;
  }

  _countdown -= _time_step;

  int double_countdown = (int)(_countdown * 2);
  int remaining = double_countdown % 2;
  if (remaining == 0 && led_state == HIGH) {
    Serial.println("LIGHT OFF");
    led_state = LOW;
    digitalWrite(LED, led_state);
  }
  if (remaining == 1 && led_state == LOW) {
    Serial.println("LIGHT ON");
    led_state = HIGH;
    digitalWrite(LED, led_state);
  }

  display(_countdown);
  delay(_time_step * 1000 / speed);
}

/**
  Display remaining time on Adafruit_7segment
*/
void display(double value) {
   matrix.println(value, 2);
   matrix.writeDisplay();
}
