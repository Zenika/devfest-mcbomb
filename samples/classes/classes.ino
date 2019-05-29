#define BUTTON  7
#define BUZZER   11

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




//const char *song = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
//const char *song = "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
//const char *song = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
//const char *song = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
//const char *song = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
//const char *song = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6,8p$";
//const char *song = "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
//const char *song = "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
//const char *song = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
//const char *song = "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
//const char *song = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
//const char *song = "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
//const char *song = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
const char *song = "MacGyver:d=4,o=6,b=150:8b4,8e5,8a5,8b5,a5,8e5,8b4,8p,8e5,8a5,8b5,8a5,8e5,b4,8p,8e5,8a5,8b5,a5,8e5,8b4,8p,8a5,8d,8c,8d,8c,8b5,8a5,8b4,8e5,8a5,8b5,a5,8e5,8b4,8p,8e5,8a5,8b5,8a5,8e5,b4,8p,8e5,8a5,8b5,a5,8e5,8b4,8p,8a5,8d,8c,8d,8c,8b5,8a5,b5,8p,2b5,8p,b5,8p,a5,d.,b5,8p,2b5,8p,8b5,8p,2a5,p,8c,8c,8c,8c,8c,8c,8c,8c,2b5,16p,8f#5,8a5,8p,2g5,8p,8c,8c,8p,b5,8a5,8b5,8a5,8g5,8p,e,2a5,16p,8c,8c,8p,2b5,8p,8f#5,8a5,8p,2g5,8p,8c,8c,8p,4b5,8a5,8b5,8a5,8g5,8p,4e,2a5,2b5,32p,8c,8b5,8a5,c,8b5,8a5,8d,8c,8b5,d,8c,8b5,e,8d,8e,f#,b5,g,8p,f#,f,b5,8g,8e,8b5,8f#,8d,8a5,8e,8c,8g5,8d,8b5,8g5,8c,8e5,8b5,8d5,8c,8b5,8a5,8g5,a#5,a5,8g,8g5,8d,8g5,8d#,8d#5,8a#5,8a5,8g5,8g4,8d5,8g4,8d#5,8g4,8a#4,8a4,8g4,8g4,8g4,8g4,8g4,8g4,8g4,8p$";

enum e_state {WAITING, STARTING, PLAYING, NOISING, ENDING};
int state = WAITING;

SongPlayer *sp;

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  sp = new SongPlayer(song, BUZZER);

  Serial.begin(9600);
  while(!Serial);
  Serial.println("Application is Running !");
}

void loop() {
  switch(state){
    case WAITING:
      waiting();
      break;
    case STARTING:
      starting();
      break;
    case PLAYING:
      playing();
      break;
    case NOISING:
      noise(300, 3000);
      break;
    case ENDING:
      ending();
      break;
  }
}

void waiting() {
  if (digitalRead(BUTTON) == HIGH) {
    Serial.println("Button pushed");
    state = STARTING;
    sp->start_playing();
  }
}

void starting() {
  Serial.println("Playing start in 1 second");
  delay(1000);
  Serial.println("Playing now");
  state = PLAYING;
}

void playing() {
  sp->play_note();
  if (!sp->is_playing_song()) {
    Serial.println("End of the song");
    state = ENDING;
  }
  if (digitalRead(BUTTON) == HIGH) {
    Serial.println("Button pushed");
    state = NOISING;
    sp->stop_playing();
  }
}

void noise(int freq, int duration)  {
  long frequency = ((long) freq) * 100;
  Serial.println(frequency);
  unsigned long time = millis();
  while(millis() - time <= duration)  {
    Serial.println(frequency);
    frequency -= 5;
    int low = (frequency / 100) - 10;
    int high = (frequency / 100) + 10;
    tone(BUZZER, random(low, high));
  }
  noTone(BUZZER);
  state = ENDING;
}


void ending() {
  Serial.println("Ending start in 10 seconds");
  delay(10000);
  Serial.println("Ending now");
  state = WAITING;
}
