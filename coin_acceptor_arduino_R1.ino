/*
  Programa para verificar la inserción de monedas en un Arduino Uno.
  
  Descripción:
  Este código verifica la inserción de monedas mediante una interrupción en un pin digital.
  Cuando se detecta la inserción de una moneda, se enciende un LED y se imprime un mensaje en el puerto serial.
  
  Mejoras y optimizaciones:
  Este código ha sido mejorado y optimizado por noxivo, un entusiasta de Arduino y programación.
  Noxivo es conocido por su pasión por la electrónica y su contribución a la comunidad de código abierto.
  
  Créditos:
  Mejorado y optimizado por: noxivo
  Contribuciones: Pasión por la electrónica, programación y contribuciones a la comunidad de código abierto.
*/

// Definición de constantes para pines y configuraciones
const int PIN_MONEDA = 2;  // Pin digital para la entrada de la señal de moneda
const int PIN_LED = 13;     // Pin digital para el LED de indicación

// Variables volátiles para el manejo de interrupciones
volatile int conteoPulsos = 0;  // Contador de pulsos de interrupción
volatile bool monedaInsertada = false;  // Indicador de inserción de moneda

// Configuración inicial del programa
void setup() {
  // Inicialización del puerto serial a 9600 baudios
  Serial.begin(9600);
  
  // Configuración del pin del LED como salida
  pinMode(PIN_LED, OUTPUT);

  // Configuración de la interrupción para el pin de la moneda
  attachInterrupt(digitalPinToInterrupt(PIN_MONEDA), interrupcionMoneda, RISING);
}

// Función principal del programa
void loop() {
  // Verifica si se ha insertado una moneda
  if (monedaInsertada) {
    // Reinicia el indicador de inserción de moneda
    monedaInsertada = false;
    
    // Enciende el LED de indicación
    digitalWrite(PIN_LED, HIGH);
    
    // Imprime un mensaje en el puerto serial
    Serial.println("Moneda insertada");
    
    // Espera 1 segundo
    delay(1000);
    
    // Reinicia el contador de pulsos
    conteoPulsos = 0;
  } else {
    // Apaga el LED de indicación si no se ha insertado una moneda
    digitalWrite(PIN_LED, LOW);
  }
}

// Función de interrupción para contar los pulsos de la moneda
void interrupcionMoneda() {
  // Incrementa el contador de pulsos
  conteoPulsos++;
  
  // Activa el indicador de inserción de moneda
  monedaInsertada = true;
}
