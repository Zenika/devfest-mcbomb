#include <Adafruit_NeoPixel.h>

#define ACCELERATOR  A1 // input pin Accelerator cable
#define DEFUSE       A0 // input pin Defuse cable
#define BOOM         A2 // input pin Explose cable
#define FAKE_LINK    A3 
#define LED          3  // input pin Debug LED
#define RLED         2	// input pin Neopixel is attached to
#define NUMPIXELS    48 // number of neopixels in Ring
#define BUZZER       4  // input pin buzzer
#define BUTTON_START 7  // input pin Start push button

//TODO Define 7Seg analog pin in A4-A5

const long MAX_TIME= 10000; // Party max time

enum state_t {STANBY_STATE, RUNNING_STATE,ENDING_STATE};
  
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RLED, NEO_GRB + NEO_KHZ800);

int ms_delay;
bool acceleratorCutted;
long maxTime;
long leftTime;
long loopTime;
long ledTime;
int ledStatus;
int buzzerTime;
int buzzerDelay;
int state = STANBY_STATE;
bool startPushed;

void setup()
{
  Serial.begin(9600);
  pixels.begin();
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(ACCELERATOR, INPUT);
  pinMode(DEFUSE, INPUT);
  pinMode(BOOM, INPUT);
  pinMode(BUTTON_START, INPUT);
  
}

void initGame(){
  leftTime = MAX_TIME; // Party time
  ms_delay = 1000;     // Delay per round. Default 1sec
  acceleratorCutted = false;
  loopTime = 0;
  ledTime = 0;
  buzzerTime = 0 ;
  ledStatus = LOW;
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  pixels.show();


}

void win(){
  state = ENDING_STATE;
  Serial.println("YOU WIN :)");
  
  // Blink green for 10 seconds
  for(int j=0; j < 10;j++){
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0, 255,0)); // Moderately bright green color.
    }
    pixels.show();
    delay(500);
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0, 0,0)); // Moderately bright green color.
    }
    pixels.show();
    delay(500);
  }
  delay(3000);
}

void loose(){
  state = ENDING_STATE;
  Serial.println("YOU LOOSE :(");
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(255, 0,0)); // Moderately bright green color.
  }
  pixels.show();
  tone(BUZZER, 4300, 3000);
  delay(6000);

  // TODO: smoke lv10
}

void accelerate(){
  Serial.println("accelerator down");
  // TODO: smoke lv 2
}

void running(){
  // Connectivity check
  if ((digitalRead(ACCELERATOR) == LOW) && not acceleratorCutted )  {
      ms_delay = ms_delay / 2;
      acceleratorCutted = true;
      accelerate();
  }
  if (digitalRead(BOOM) == LOW)  {
    loose();
    return;
  }
  if (digitalRead(DEFUSE) == LOW)  {
    win();
    return;
  }
  
  // APS per seconds
  int bps = 1/((leftTime * 1.0)/MAX_TIME);
  int blink_delay = ms_delay / bps;
  
  // Update LEDs every blink_delay/2 (blink)
  if (millis() - ledTime > blink_delay/2){
    if (ledStatus == LOW) {
      digitalWrite(LED, HIGH);
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(255, 0,0)); // Moderately bright green color.
      }
      pixels.show();
      ledStatus = HIGH;
    } else {
      digitalWrite(LED, LOW);
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0, 0,0)); // Moderately bright green color.
      }
      pixels.show();
      ledStatus = LOW;
    }
    ledTime = millis();
  }
  
  
  // Buzzer
  if (millis() - buzzerTime > blink_delay){
    tone(BUZZER, 4300, 50);
    Serial.print("Beep");
    buzzerTime = millis();
  } 
  
  
  // Update timer 
  if (millis() - loopTime > ms_delay){
      leftTime = leftTime - 1000; // 1 sec btw ;)
      Serial.println("");
      Serial.println("---- ");
      Serial.println("SEGMENT: ");
      Serial.println(leftTime);
      Serial.print("BPS: "+ bps);
      Serial.println(bps);
      loopTime = millis();
  }
  
  // Game over...
  if ( leftTime <= 0 ){
    loose();
  }

}

// TODO: McGiver 8bit? 
void standby() {
  
  // 
  int r = random(0,255);
  int g = random(0,255);
  int b = random(0,255);
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(r, g, b)); 
    pixels.show();
    
    delay(1000/NUMPIXELS);
    
    startPushed = digitalRead(BUTTON_START);
    if (startPushed){
      Serial.println("START");
      state = RUNNING_STATE;
      initGame();
      break;
    }

  }

}

void ending(){
  Serial.println("END");
  state = STANBY_STATE;
}

void loop()
{
  delay(200); // simulator only...

  switch(state){
    case STANBY_STATE:
      standby();
      break;
    case RUNNING_STATE:
      running();
      break;
    case ENDING_STATE: 
      ending();
      break;
  }
}
