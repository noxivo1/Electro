/*
  Programa de Detección de Monedas de Alta Precisión para nodeMCU ESP8266.
  
  Descripción:
  Este programa, creado y meticulosamente perfeccionado por NOXIVO, ofrece una detección de monedas excepcionalmente precisa y fiable.
  Se han implementado avanzadas técnicas y estrategias para reducir los falsos positivos y aumentar la robustez del sistema.
  
  Características Destacadas:
  - Debounce de Software: Implementación de un algoritmo de debounce para filtrar los rebotes mecánicos y asegurar una detección precisa.
  - Manejo Eficiente de Interrupciones: Optimización del manejo de interrupciones para reducir la latencia y mejorar la respuesta del sistema.
  - Conteo de Monedas: Incorporación de un contador para llevar un registro exacto del número de monedas insertadas.
  - Indicación Visual: Uso de un LED para proporcionar una confirmación visual inmediata de la detección de monedas.
  
  Créditos:
  Desarrollado y Optimizado por: NOXIVO
  Contacto y Consultas: NOXIVO es un experto en electrónica y programación, comprometido con la mejora continua y el apoyo a la comunidad de código abierto.
*/

const int COIN_PIN = 4;    // Pin GPIO4 para la detección de monedas
const int LED_PIN = 16;    // Pin GPIO16 para el LED indicador

volatile int coinCount = 0;  // Contador de monedas detectadas
volatile bool coinDetected = false;  // Bandera de detección de moneda

unsigned long lastDebounceTime = 0;  // Tiempo de la última detección para debounce
const unsigned long debounceDelay = 50;  // Retardo de debounce en milisegundos

void ICACHE_RAM_ATTR handleCoinInterrupt() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    coinDetected = true;  // Señal de detección de moneda activada
    lastDebounceTime = millis();  // Actualización del tiempo de debounce
  }
}

void setup() {
  Serial.begin(115200);  // Inicialización del puerto serial a 115200 baudios
  pinMode(COIN_PIN, INPUT_PULLUP);  // Configuración del pin de moneda como entrada con resistencia pull-up
  pinMode(LED_PIN, OUTPUT);  // Configuración del pin del LED como salida
  digitalWrite(LED_PIN, HIGH);  // Estado inicial del LED: encendido
  attachInterrupt(digitalPinToInterrupt(COIN_PIN), handleCoinInterrupt, FALLING);  // Interrupción para la detección de monedas
}

void loop() {
  if (coinDetected) {  // Verificación de detección de moneda
    coinDetected = false;  // Reinicio de la bandera de detección
    coinCount++;  // Incremento del contador de monedas
    handleCoin();  // Procesamiento de la moneda detectada
  }
}

// Función para procesar la detección de monedas
void handleCoin() {
  digitalWrite(LED_PIN, LOW);  // Apagado del LED indicador
  Serial.printf("Moneda detectada - Total de monedas: %d\\n", coinCount);  // Mensaje de detección con conteo total
  delay(1000);  // Retardo para visualización del mensaje
  digitalWrite(LED_PIN, HIGH);  // Encendido del LED indicador
}
