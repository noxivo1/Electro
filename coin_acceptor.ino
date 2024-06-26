/*
  Código creado y mejorado por NOXIVO para nodeMCU ESP8266.

  Este código ha sido creado, optimizado y mejorado significativamente por NOXIVO para ofrecer una detección de monedas
  más precisa y confiable. Se han implementado diversas técnicas y estrategias para minimizar los falsos positivos
  y mejorar la robustez del sistema. Entre las mejoras implementadas se incluyen:

  - Debounce de software: Se ha implementado un mecanismo de debounce de software para filtrar los rebotes del
    interruptor y garantizar una detección precisa de las monedas.
  
  - Manejo de interrupciones: Se ha optimizado el manejo de interrupciones para minimizar la latencia y mejorar
    la respuesta del sistema ante la detección de una moneda.
  
  - Conteo de monedas: Se ha añadido un contador de monedas para llevar un registro del número total de monedas
    insertadas y ofrecer una retroalimentación al usuario a través del puerto serial.
  
  - Indicación visual: Se utiliza un LED para indicar visualmente cuando se ha detectado una moneda, proporcionando
    una confirmación rápida y clara al usuario sobre la aceptación de la moneda.

  Para cualquier consulta, comentario o mejora adicional, por favor contactar a NOXIVO.
*/

const int COIN_PIN = 4;    // Pin GPIO4 para la señal del monedero
const int LED_PIN = 16;    // Pin GPIO13 para el LED

volatile int coinCount = 0;  // Contador de monedas
volatile bool coinDetected = false;  // Bandera para detectar la inserción de una moneda

unsigned long lastDebounceTime = 0;  // Último tiempo de rebote del interruptor
unsigned long debounceDelay = 50;    // Tiempo de debounce en milisegundos

void ICACHE_RAM_ATTR handleCoinInterrupt() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    coinDetected = true;  // Activamos la bandera de detección de moneda
    lastDebounceTime = millis();  // Actualizamos el tiempo de rebote
  }
}

void setup() {
  Serial.begin(115200);  // Inicializamos el puerto serial
  pinMode(COIN_PIN, INPUT_PULLUP);  // Configuramos el pin del monedero como entrada con pull-up interno
  pinMode(LED_PIN, OUTPUT);  // Configuramos el pin del LED como salida
  digitalWrite(LED_PIN, HIGH);  // Encendemos el LED inicialmente
  attachInterrupt(digitalPinToInterrupt(COIN_PIN), handleCoinInterrupt, FALLING);  // Configuramos la interrupción en el pin indicado
}

void loop() {
  if (coinDetected) {  // Verificamos si se detectó una moneda
    coinDetected = false;  // Reiniciamos la bandera de detección de moneda
    coinCount++;  // Incrementamos el contador de monedas
    handleCoin();  // Manejamos la inserción de la moneda
  }
}

// Función para manejar la inserción de monedas
void handleCoin() {
  digitalWrite(LED_PIN, LOW);  // Apagamos el LED
  Serial.print("Moneda detectada - Total de monedas: ");  // Imprimimos un mensaje
  Serial.println(coinCount);  // Imprimimos el total de monedas
  delay(1000);  // Esperamos un segundo
  digitalWrite(LED_PIN, HIGH);  // Encendemos el LED
}
