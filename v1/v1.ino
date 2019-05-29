#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

Adafruit_7segment matrix = Adafruit_7segment();

#define BUZZER  11
#define BUTTON   7
#define LED      4

unsigned long one_second_to_micros = 1000000;

int WIRE_1 = A0;
int WIRE_2 = A1;
int WIRE_3 = A2;

#define OCTAVE_OFFSET 0

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951

int notes[] = { 0,
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};


const char *song = "MacGyver:d=4,o=6,b=150:8b4,8e5,8a5,8b5,a5,8e5,8b4,8p,8e5,8a5,8b5,8a5,8e5,b4,8p,8e5,8a5,8b5,a5,8e5,8b4,8p,8a5,8d,8c,8d,8c,8b5,8a5,8b4,8e5,8a5,8b5,a5,8e5,8b4,8p,8e5,8a5,8b5,8a5,8e5,b4,8p,8e5,8a5,8b5,a5,8e5,8b4,8p,8a5,8d,8c,8d,8c,8b5,8a5,b5,8p,2b5,8p,b5,8p,a5,d.,b5,8p,2b5,8p,8b5,8p,2a5,p,8c,8c,8c,8c,8c,8c,2b5,16p,8f#5,8a5,8p,2g5,8p,8c,8c,8p,b5,8a5,8b5,8a5,8g5,8p,e,2a5,16p,8c,8c,8p,2b5,8p,8f#5,8a5,8p,2g5,8p,8c,8c,8p,4b5,8a5,8b5,8a5,8g5,8p,4e,2a5,2b5,32p,8c,8b5,8a5,c,8b5,8a5,8d,8c,8b5,d,8c,8b5,e,8d,8e,f#,b5,g,8p,f#,f,b5,8g,8e,8b5,8f#,8d,8a5,8e,8c,8g5,8d,8b5,8g5,8c,8e5,8b5,8d5,8c,8b5,8a5,8g5,a#5,a5,8g,8g5,8d,8g5,8d#,8d#5,8a#5,8a5,8g5,8g4,8d5,8g4,8d#5,8g4,8a#4,8a4,8g4,8g4,8g4,8g4,8g4,8g4,8g4";
//const char *short_song = "MacGyver:d=4,o=6,b=150:1p,8c,8c,8c,8c,8c,8c,8c,8c,2b5,16p,8f#5,8a5,8p,2g5,8p,8c,8c,8p,b5,8a5,8b5,8a5,8g5,8p,e,2a5,16p,8c,8c,8p,2b5,8p,8f#5,8a5,8p,2g5,8p,8c,8c,8p,4b5,8a5,8b5,8a5,8g5,8p,4e,2a5,2b5,32p,8c,8b5,8a5,c,8b5,8a5,8d,8c,8b5,d,8c,8b5,e,8d,8e,f#,b5,g,8p,f#,f,b5,8g,8e,8b5,8f#,8d,8a5,8e,8c,8g5,8d,8b5,8g5,8c,8e5,8b5,8d5,8c,8b5,8a5,8g5,a#5,a5,8g,8g5,8d,8g5,8d#,8d#5,8a#5,8a5,8g5,8g4,8d5,8g4,8d#5,8g4,8a#4,8a4,8g4,8g4,8g4,8g4,8g4,8g4,8g4,8p$";

class SongPlayer {
    public:
    SongPlayer(const char *s, char pin) {
      _pin = pin;
      song_to_play = s;
    }
  
    void start_playing() {
      note_pointer = song_to_play;
    
      // Absolutely no error checking in here
      int num; // utility to parse numbers from the notes sequence of the `song` string.
    
      // format: d=N,o=N,b=NNN:
      // find the start (skip name, etc)
    
      while (*note_pointer != ':') {
        note_pointer++; // ignore name
      }
      note_pointer++; // skip ':'
    
      // get default duration
      if (*note_pointer == 'd') {
        note_pointer += 2; // skip "d="
        num = 0;
        while (isdigit(*note_pointer)) {
          num = (num * 10) + (*note_pointer++ - '0');
        }
        if (num > 0) {
          duration = num;
        }
        note_pointer++; // skip comma
      }
    
      // get default octave
      if (*note_pointer == 'o') {
        note_pointer++; note_pointer++; // skip "o="
        num = *note_pointer++ - '0';
        if (num >= 3 && num <= 7) {
          octave = num;
        }
        note_pointer++; // skip comma
      }
    
      // get BPM
      if (*note_pointer == 'b') {
        note_pointer++; note_pointer++; // skip "b="
        num = 0;
        while (isdigit(*note_pointer)) {
          num = (num * 10) + (*note_pointer++ - '0');
        }
        bpm = num;
        note_pointer++; // skip colon
      }
    
      // BPM usually expresses the number of quarter notes per minute
      wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)
    
      playing_song = true;
    }

    void stop_playing() {
      playing_song = false;
      noTone(_pin);
    }

    void play_note() {
      if (!playing_song) {
        return;
      }
      
      int num; // util var to parse numbers from the notes sequence of the `song` string.
    
      unsigned long note_micros = micros() - note_start;
      if (note_micros < note_duration) {
        // now, play the note or the pause
        if (note) {
          tone(_pin, notes[(scale - 4) * 12 + note]);
        } else {
          // pause
        }
      } else {
    //  if (note_micros >= note_duration) {
        noTone(_pin);
        
        // first, get note duration, if available
        num = 0;
        while (isdigit(*note_pointer)) {
          num = (num * 10) + (*note_pointer++ - '0');
        }
        if (num) {
          note_duration = wholenote / num;
        } else {
          note_duration = wholenote / duration;  // we will need to check if we are a dotted note after
        }
        note_duration *= 1000; // convert milliseconds to microseconds
    
        // now get the note
        switch (*note_pointer) {
          case 'c':
            note = 1;
            break;
          case 'd':
            note = 3;
            break;
          case 'e':
            note = 5;
            break;
          case 'f':
            note = 6;
            break;
          case 'g':
            note = 8;
            break;
          case 'a':
            note = 10;
            break;
          case 'b':
            note = 12;
            break;
          case 'p':
          default:
            note = 0;
        }
        note_pointer++;
    
        // now, get optional '#' sharp
        if (*note_pointer == '#') {
          note++;
          note_pointer++;
        }
    
        // now, get optional '.' dotted note
        if (*note_pointer == '.') {
          note_duration += note_duration/2;
          note_pointer++;
        }
        
        // now, get scale
        if (isdigit(*note_pointer)) {
          scale = *note_pointer - '0';
          note_pointer++;
        } else {
          scale = octave;
        }
        scale += OCTAVE_OFFSET;
    
        if (*note_pointer == ',') {
          // skip comma for next note (or we may be at the end)
          note_pointer++;
        } else if (*note_pointer == '$') {
          // stop the song if it's the end (materialized with $ char)
          stop_playing();
        }
    
        note_start = micros();
      }
    }

    bool is_playing_song() {
      return playing_song;
    }
    
  private:
    char _pin;
    const char *song_to_play;
    const char *note_pointer;
    char duration = 4;
    char octave = 6;
    int bpm = 63;
    long wholenote; // don't know what is this ?!
    bool playing_song = false;

    char note = 0;
    char scale = 0;
    unsigned long note_start = 0;
    unsigned long note_duration = 0;
};

SongPlayer *sp;

enum e_state {WAITING, STARTING, PLAYING, WINNING, LOOSING};
int state = WAITING;
unsigned long STARTING_DURATION  =  3000000; // microseconds ( 3.00 s)
unsigned long BEEP_DURATION      =   100000; // microseconds ( 0.10 s)
unsigned long WINNING_DURATION   = 60000000; // microseconds (60.00 s)
unsigned long LOOSING_DURATION   = 10000000; // microseconds (10.00 s)
unsigned long EXPLOSION_DURATION =  4000000; // microseconds ( 4.00 s)
unsigned long TIME_STEP_US       =    20000; // microseconds ( 0.02 s)

double PLAYING_DURATION_S =    60.00; // seconds (60.00 s)
double TIME_STEP_S = 0.022; // seconds ( 0.022 s)

double _countdown;
double _countdown_fix;
int speed = 1.00;


int led_frequency = 1;
int led_state = LOW;

int beep_frequency = 1;

void setup() {
  // input pin for digitalRead()
  pinMode(WIRE_1, INPUT);
  pinMode(WIRE_2, INPUT);
  pinMode(WIRE_3, INPUT);
  pinMode(BUTTON, INPUT);

  // output pin for digitalWrite()
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  
  sp = new SongPlayer(song, BUZZER);

  Serial.begin(9600);                 /* Initialise le port serie */
  Wire.begin();                       /* Initialise le bus i2C */
  matrix.begin(0x70);                 /* Adresse I2C de l'afficheur 7 segments */
  matrix.setBrightness(2);

  while(!Serial);                     /* Wait until Serial port is enable */
  Serial.println("Application Up !!!");
  waiting();
}


void loop() {
  switch(state){
    case WAITING:
      wait();
      break;
    case STARTING:
      start();
      break;
    case PLAYING:
      play();
      beep();
      blink_led();
      break;
    case WINNING:
      win();
      sp->play_note();
      break;
    case LOOSING:
      loose();
      explode();
      break;
  }

//  delay(10);
}


unsigned long beep_start;  // The start point time in microseconds of the beep
void beep() {
  unsigned long beep_micros = micros() - beep_start;
  Serial.print("beep_start: "); Serial.println(beep_start);
  Serial.print("beep_micros: "); Serial.println(beep_micros);
  if (beep_micros >= BEEP_DURATION) {
    //Serial.println("BEEP OFF");
    noTone(BUZZER);
  } else {
    //Serial.println("BEEP ON");
    tone(BUZZER, 1000);
  }

  double frequency = beep_frequency * speed;
  if (beep_micros >= (one_second_to_micros / frequency)) {
    Serial.println("BEEP");
    _countdown_fix -= 1.00 / frequency;  // this is a tricky fix to synchronize
    _countdown = _countdown_fix;                      // the beep, the blink and the countdown
    beep_start = micros(); // start another beep
  }

  if (_countdown <= 20.00) {
    beep_frequency = 2;
  }
  if (_countdown <= 10.00) {
    beep_frequency = 4;
  }
  if (_countdown <= 5.00) {
    beep_frequency = 8;
  }
  if (_countdown <= 2.00) {
    beep_frequency = 16;
  }
}


unsigned long led_start;  // The start point time in microseconds of the step led blinking
// method ran in loop while state is PLAYING
void blink_led() {
  unsigned long led_micros = micros() - led_start;
  // do I have to change the LED state ?
  if (led_micros >= ((one_second_to_micros/2) / (led_frequency*speed))) {
    if (led_state == HIGH) {
      switch_led_off();
    } else if (led_state == LOW) {
      switch_led_on();
    }
    led_start = micros();
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



// ==============================================================================================================
// method ran only once
void waiting() {
  sp->stop_playing(); // when coming from `win()`
  noTone(BUZZER); // when coming from `loose()`

  display(10000); // displays "----" on the 7seg
  matrix.blinkRate(HT16K33_BLINK_OFF);
  matrix.setBrightness(2);

  state = WAITING;  
}

// method ran in loop while state is WAITING
void wait() {
//  Serial.println("WAIT");
  if (digitalRead(BUTTON) == HIGH) {
    starting();
  }
}

// ==============================================================================================================
unsigned long start_start; // The start point time in microseconds of the step STARTING
// method ran only once
void starting() {
//  Serial.println("STARTING");
  // Reseting the game
  _countdown = PLAYING_DURATION_S;
  _countdown_fix = PLAYING_DURATION_S;
  speed = 1;
  led_frequency = 1;
  beep_frequency = 1;
  randomize_triggers();

  // Prepare the next step
  display(_countdown);
  start_start = micros();
  state = STARTING;
}

// method ran in loop while state is STARTING
void start() {
//  Serial.println("START");
  unsigned long start_micros = micros() - start_start;
  if (start_micros >= STARTING_DURATION) {
    playing();
  }
}

// ==============================================================================================================
unsigned long countdown_start; // The start point time in microseconds of the step PLAYING
// method ran only once
void playing() {
//  Serial.println("PLAYING");
  // Prepare the next step
  countdown_start = micros();
  beep_start = micros();
  led_start = micros() - one_second_to_micros; // "- one_second_to_micros" to light on the led as soon as playing
  state = PLAYING;
}

// method ran in loop while state is PLAYING
void play() {
  Serial.print("PLAY => "); Serial.println(_countdown);
  unsigned long countdown_micros = micros() - countdown_start;
  if (countdown_micros >= TIME_STEP_US) {
    _countdown -= TIME_STEP_S * speed;
    display(_countdown);
    countdown_start = micros(); // start another countdown
  }

  if (digitalRead(WIRE_1) == LOW) {
    winning();
  }
  if (digitalRead(WIRE_2) == LOW) {
    speed = 2;
  }
  if (_countdown <= 0.00 || digitalRead(WIRE_3) == LOW) {
    loosing();
  }

}

// ==============================================================================================================
unsigned long win_start;  // The start point time in microseconds of the step WINNING
// method ran only once
void winning() {
  Serial.println("WINNING");
  win_start = micros();
  state = WINNING;
  matrix.setBrightness(0);
  switch_led_off();
  sp->start_playing();
}

// method ran in loop while state is WINNING
void win() {
  Serial.println("WINNING");
  unsigned long win_micros = micros() - win_start;
  if (win_micros >= WINNING_DURATION ) {
    waiting();
  }
}


// ==============================================================================================================
unsigned long explode_start;  // The start point time in microseconds of the step LOOSING
int explode_frequency; // The frequency of the explosion
// method ran in loop while state is LOOSING
void explode() {
  Serial.println("EXPLODE");
  unsigned long explode_micros = micros() - explode_start;
  if (explode_micros >= EXPLOSION_DURATION) {
    noTone(BUZZER);
  } else {
    explode_frequency *= 100;
    explode_frequency -= 5;
    explode_frequency /= 100;
    int low = explode_frequency - 10;
    int high = explode_frequency + 10;
    tone(BUZZER, random(low, high));    
  }
}

unsigned long loose_start;  // The start point time in microseconds of the step LOOSING
// method ran only once
void loosing() {
  Serial.println("LOOSING");
  loose_start = micros();
  state = LOOSING;
  switch_led_off();

  explode_start = micros();
  explode_frequency = 300;

  display(8888);
  matrix.blinkRate(HT16K33_BLINK_2HZ);
  matrix.setBrightness(15);
}

// method ran in loop while state is LOOSING
void loose() {
  Serial.println("LOOSE");
  unsigned long loose_micros = micros() - loose_start;
  if (loose_micros >= LOOSING_DURATION) {
    waiting();
  }
}



void randomize_triggers() {
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


void switch_led_on() {
//  Serial.println("LIGHT ON");
  led_state = HIGH;
  digitalWrite(LED, led_state);
}


void switch_led_off() {
//  Serial.println("LIGHT OFF");
  led_state = LOW;
  digitalWrite(LED, led_state);
}


/**
  Display remaining time on Adafruit_7segment
*/
void display(double value) {
  matrix.println(value, 2);
  matrix.writeDisplay();
}
