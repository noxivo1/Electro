#include <Servo.h>  // Incluye la biblioteca Servo

// Definición de constantes para pines y configuraciones
const int PIN_MONEDA = 2;  // Pin digital para la entrada de señal de moneda
const int PIN_LED = 13;    // Pin digital para el LED indicador
const int PIN_SERVO = 9;   // Pin digital para el servo motor

Servo miServo;  // Crea un objeto servo para controlar el servo motor

// Variables volátiles para el manejo de interrupciones
volatile int conteoPulsos = 0;  // Contador de pulsos de interrupción
volatile bool monedaInsertada = false;  // Bandera de inserción de moneda

// Configuración inicial del programa
void setup() {
  // Inicialización del puerto serial a 9600 baudios
  Serial.begin(9600);
  
  // Configuración del pin del LED como salida
  pinMode(PIN_LED, OUTPUT);
  
  // Configuración del pin del servo motor como salida
  miServo.attach(PIN_SERVO);
  
  // Establece la posición inicial del servo en 0 grados
  miServo.write(0);
  
  // Configuración de la interrupción para el pin de la moneda
  attachInterrupt(digitalPinToInterrupt(PIN_MONEDA), interrupcionMoneda, RISING);
}

// Función principal del programa
void loop() {
  // Verifica si se ha insertado una moneda
  if (monedaInsertada) {
    // Reinicia la bandera de inserción de moneda
    monedaInsertada = false;
    
    // Activa el LED indicador
    digitalWrite(PIN_LED, HIGH);
    
    // Mueve el servo motor 130 grados
    miServo.write(130);
    
    // Espera 1 segundo
    delay(1000);
    
    // Regresa el servo motor a su posición inicial
    miServo.write(0);
    
    // Envía un mensaje al puerto serial
    Serial.println("Moneda detectada e insertada.");
    
    // Espera 1 segundo
    delay(1000);
    
    // Reinicia el contador de pulsos
    conteoPulsos = 0;
  } else {
    // Desactiva el LED indicador si no se ha insertado una moneda
    digitalWrite(PIN_LED, LOW);
  }
}

// Función de interrupción para detectar los pulsos de la moneda
void interrupcionMoneda() {
  // Incrementa el contador de pulsos
  conteoPulsos++;
  
  // Establece la bandera de inserción de moneda
  monedaInsertada = true;
}
