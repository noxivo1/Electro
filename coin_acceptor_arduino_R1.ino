



// Declaración de la función coinInterrupt antes de su primer uso.
void coinInterrupt();

const int coinpin = 2;
const int ledpin = 13;
volatile int pulse = 0;
boolean bInserted = false;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(coinpin), coinInterrupt, RISING);
  pinMode(ledpin, OUTPUT);
  
}

void loop() {
  if (bInserted) {
    bInserted = false;
    digitalWrite(ledpin, HIGH);
    Serial.println("COIN");
    delay(1000);
    
    if (pulse == 1) {
     
      pulse = 0; // Reinicia el contador de pulsos
    }
  } else {
    digitalWrite(ledpin, LOW);
  }
}

void coinInterrupt() {
  pulse++;
  bInserted = true;
}
