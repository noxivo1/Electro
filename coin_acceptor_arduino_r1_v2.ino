// Definición de constantes para pines y configuraciones
const int PIN_MONEDA = 2;  // Pin digital para la entrada de señal de moneda
const int PIN_LED = 13;    // Pin digital para el LED indicador

// Variables para el manejo de interrupciones y temporización
volatile int conteoPulsos = 0;  // Contador de pulsos de interrupción
volatile bool monedaInsertada = false;  // Bandera de inserción de moneda
unsigned long tiempoAnterior = 0;  // Almacena la última vez que se actualizó el LED

// Configuración inicial del programa
void setup() {
  Serial.begin(9600);  // Inicialización del puerto serial a 9600 baudios
  pinMode(PIN_LED, OUTPUT);  // Configuración del pin del LED como salida
  attachInterrupt(digitalPinToInterrupt(PIN_MONEDA), interrupcionMoneda, RISING);  // Interrupción
}

// Función principal del programa
void loop() {
  gestionarMoneda();
  actualizarLED();
}

// Gestiona la detección e inserción de monedas
void gestionarMoneda() {
  if (monedaInsertada) {
    monedaInsertada = false;  // Reinicia la bandera de inserción de moneda
    Serial.println("Moneda detectada e insertada.");  // Mensaje al puerto serial
    conteoPulsos = 0;  // Reinicia el contador de pulsos
    activarLED();  // Activa el LED indicador
  }
}

// Actualiza el estado del LED basado en el tiempo transcurrido
void actualizarLED() {
  if (millis() - tiempoAnterior >= 1000 && digitalRead(PIN_LED) == HIGH) {  // Verifica si ha pasado 1 segundo
    desactivarLED();  // Desactiva el LED indicador
  }
}

// Activa el LED y registra el tiempo actual
void activarLED() {
  digitalWrite(PIN_LED, HIGH);
  tiempoAnterior = millis();  // Reinicia el temporizador
}

// Desactiva el LED
void desactivarLED() {
  digitalWrite(PIN_LED, LOW);
}

// Función de interrupción para detectar los pulsos de la moneda
void interrupcionMoneda() {
  conteoPulsos++;  // Incrementa el contador de pulsos
  if (conteoPulsos == 1) {  // Solo establece la bandera en el primer pulso
    monedaInsertada = true;
  }
}
