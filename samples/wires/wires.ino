#define WIRE_1 A0
#define WIRE_2 A1
#define WIRE_3 A2

void setup() {
  pinMode(WIRE_1, INPUT);
  pinMode(WIRE_2, INPUT);
  pinMode(WIRE_3, INPUT);

  Serial.begin(9600);                 /* Initialise le port serie */
  while (!Serial);                    /* Attends la liaison du port série */
  Serial.println("Serial is ready");  

  Serial.println("Application Up !!!");
  Serial.println(A0);
  Serial.println(A1);
  Serial.println(A2);
}

void loop() {
  int w1 = digitalRead(WIRE_1)*100;
  int w2 = digitalRead(WIRE_2)*10;
  int w3 = digitalRead(WIRE_3)*1;
  int wires = w1 + w2 + w3;
  Serial.println(wires);
  delay(500);
}
