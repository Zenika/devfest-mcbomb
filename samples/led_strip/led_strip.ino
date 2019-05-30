#include <Adafruit_NeoPixel.h>

#define RLED          4	// input pin Neopixel is attached to
#define NUMPIXELS    57 // number of neopixels in Ring

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RLED, NEO_GRB + NEO_KHZ800);
uint32_t RED = pixels.Color(30, 0, 0);
uint32_t GREEN = pixels.Color(0, 10, 0);
uint32_t BLUE = pixels.Color(0, 0, 10);
uint32_t BLACK = pixels.Color(0, 0, 0);

void setup() {
  Serial.begin(9600);
  //while(!Serial);
  pinMode(RLED, OUTPUT);
  pixels.begin();
  for(int i=0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, BLUE);
  }
  pixels.show();
}

int countdown = 60;
void loop() {
  if (countdown < 0) {
    countdown = 60;
    for(int i=0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, BLUE);
    }
    pixels.show();
  }

  pixels.setPixelColor(countdown, BLACK);
  pixels.show();
  delay(1000);
  countdown--;
}
