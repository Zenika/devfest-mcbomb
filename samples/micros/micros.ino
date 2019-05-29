/**
 * This piece of code is made to know if it's possible to do some things 
 * each seconds and each ten seconds in parallel mode without blocking the main thread
 */

unsigned long one_second_to_micros = 1000000;
unsigned long ten_seconds_to_micros = one_second_to_micros * 10;

String STR_ONE_SECOND = "One second : ";
String STR_TEN_SECONDS = "Ten seconds : ";

unsigned long one_second_start = 0;
int one_second_count = 0;

unsigned long ten_seconds_start = 0;
int ten_seconds_count = 0;

void setup() { 
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Application UP");
}

void loop() {
  count_one_second();
  count_ten_seconds();
}

void count_one_second() {
  unsigned long one_second_micros = micros() - one_second_start;
  if (one_second_micros >= one_second_to_micros) {
    one_second_count++;
    Serial.println(STR_ONE_SECOND + one_second_count);
    one_second_start = micros();
  }
}

void count_ten_seconds() {
  unsigned long ten_seconds_micros = micros() - ten_seconds_start;
  if (ten_seconds_micros >= ten_seconds_to_micros) {
    ten_seconds_count += 10;
    Serial.println(STR_TEN_SECONDS + ten_seconds_count);
    ten_seconds_start = micros();
  }
}
