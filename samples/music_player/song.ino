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

char _pin;
const char *song_to_play;
const char *note_pointer;
char duration = 4;
char octave = 6;
int bpm = 63;
long wholenote; // don't know what is this ?!
bool playing_song = false;


void choose_song(const char *s, char pin) {
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
  Serial.print("duration: "); Serial.println(duration, 10);

  // get default octave
  if (*note_pointer == 'o') {
    note_pointer++; note_pointer++; // skip "o="
    num = *note_pointer++ - '0';
    if (num >= 3 && num <= 7) {
      octave = num;
    }
    note_pointer++; // skip comma
  }
  Serial.print("octave: "); Serial.println(octave, 10);

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
  Serial.print("bpm: "); Serial.println(bpm, 10);

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)
  Serial.print("wholenote: "); Serial.println(wholenote, 10);

  playing_song = true;
}

void stop_playing() {
  playing_song = false;
  noTone(_pin);
}


char note = 0;
char scale = 0;
unsigned long note_start = 0;
unsigned long note_duration = 0;

void play_note() {
  if (!playing_song) {
    return;
  }
  
  int num; // util var to parse numbers from the notes sequence of the `song` string.

  unsigned long note_micros = micros() - note_start;
  if (note_micros < note_duration) {
    // now, play the note or the pause
    if (note) {
      Serial.print("Playing: ");
      Serial.print(scale, 10); Serial.print(' ');
      Serial.print(note, 10); Serial.print(" (");
      Serial.print(notes[(scale - 4) * 12 + note], 10);
      Serial.print(") ");
      Serial.println(note_duration, 10);
      tone(_pin, notes[(scale - 4) * 12 + note]);
    } else {
      Serial.print("Pausing: ");
      Serial.println(note_duration, 10);
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

//    // now, play the note or the pause
    if (note) {
      Serial.print("Playing: ");
      Serial.print(scale, 10); Serial.print(' ');
      Serial.print(note, 10); Serial.print(" (");
      Serial.print(notes[(scale - 4) * 12 + note], 10);
      Serial.print(") ");
      Serial.println(note_duration, 10);
      tone(_pin, notes[(scale - 4) * 12 + note]);
    } else {
      Serial.print("Pausing: ");
      Serial.println(note_duration, 10);
    }

    note_start = micros();
  }
}

bool is_playing_song() {
  return playing_song;
}
