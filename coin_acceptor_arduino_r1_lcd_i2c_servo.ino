// Créditos
// Autor: Noxivo
// Descripción: Este código controla un dispensador de monedas utilizando un LCD I2C y un servo motor.

// Bibliotecas necesarias
#include <Wire.h> // Biblioteca para la comunicación I2C
#include <LiquidCrystal_I2C.h> // Biblioteca para el control del LCD I2C
#include <Servo.h> // Biblioteca para el control del servo motor

// Definición de constantes y variables globales
// Dirección I2C del LCD, dimensiones del LCD, inicialización del LCD y del servo motor
// Pines para la entrada de señal de moneda, LED y servo
// Variables para el manejo de interrupciones y parpadeo del mensaje

// Configuración inicial (setup)
// Inicializar comunicación serial, LCD, servo motor, y configurar pines
// Mostrar mensaje de bienvenida y configurar interrupción para la detección de monedas

// Bucle principal (loop)
// Verificar si se ha insertado una moneda y procesar la inserción
// Encender LED, actualizar LCD, mover servo, y mostrar mensajes correspondientes
// Parpadeo del mensaje "Inserte Monedas" en el LCD

// Funciones adicionales
// mostrarMensajeInicial: para mostrar el mensaje de bienvenida en el LCD
// interrupcionMoneda: para manejar la interrupción generada por la inserción de monedas

// Nota: Este pseudo-código sirve como guía para la traducción y mejora del código C++ original.
// Los comentarios detallados y la estructura del código deben adaptarse al estándar de C++ y a las convenciones de nombrado en español.
// Dirección I2C del LCD


// Dirección I2C del LCD
#define I2C_ADDR 0x27

// Dimensiones del LCD
#define LCD_COLS 16
#define LCD_ROWS 2

// Inicialización del LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

// Inicialización del Servo
Servo servoMotor;

// Definición de constantes para pines y configuraciones
const int PIN_MONEDA = 2;      // Pin digital para la entrada de señal de moneda
const int PIN_LED = LED_BUILTIN; // Pin digital para el LED incorporado (LED_BUILTIN = 13)
const int PIN_SERVO = 9;        // Pin digital para el servo

// Variables volátiles para el manejo de interrupciones
volatile int conteoPulsos = 0;  // Contador de pulsos de interrupción
volatile bool monedaInsertada = false;  // Bandera de inserción de moneda

// Variables para el parpadeo del mensaje
bool parpadeo = false;
unsigned long ultimoTiempoParpadeo = 0;
unsigned long tiempoAnterior = 0; // Para controlar eventos temporizados

// Configuración inicial del programa
void setup() {
  // Inicialización del puerto serial a 115200 baudios
  Serial.begin(115200);

  // Inicialización del LCD
  lcd.init();
  lcd.backlight();

  // Inicialización del servo
  servoMotor.attach(PIN_SERVO);
  servoMotor.write(0);  // Posiciona el servo en 0 grados

  // Mostrar mensajes centrados
  mostrarMensajeInicial();

  // Configuración del pin del LED como salida
  pinMode(PIN_LED, OUTPUT);

  // Apaga el LED indicador al inicio
  digitalWrite(PIN_LED, LOW);

  // Configuración de la interrupción para el pin de la moneda
  attachInterrupt(digitalPinToInterrupt(PIN_MONEDA), interrupcionMoneda, RISING);
}

// Función principal del programa
void loop() {
  // Verifica si se ha insertado una moneda
  if (monedaInsertada) {
    procesarMoneda();
  }

  // Parpadeo del mensaje "Inserte Monedas"
  parpadeoMensaje();
}

// Función para mostrar el mensaje inicial en el LCD
void mostrarMensajeInicial() {
  lcd.clear();
  lcd.setCursor((LCD_COLS - strlen("BIENVENIDO")) / 2, 0);
  lcd.print("BIENVENIDO");
  lcd.setCursor((LCD_COLS - strlen("Inserte Monedas")) / 2, 1);
  lcd.print("Inserte Monedas");
}

// Función de interrupción para detectar los pulsos de la moneda
void interrupcionMoneda() {
  // Incrementa el contador de pulsos
  conteoPulsos++;

  // Establece la bandera de inserción de moneda
  monedaInsertada = true;
}

// Procesa la inserción de la moneda
void procesarMoneda() {
  // Reinicia la bandera de inserción de moneda
  monedaInsertada = false;

  // Enciende el LED
  digitalWrite(PIN_LED, HIGH);

  // Actualiza el LCD
  lcd.clear();
  lcd.setCursor((LCD_COLS - strlen("$500 Pesos")) / 2, 0);
  lcd.print("$500 Pesos");

  // Espera sin bloquear
  tiempoAnterior = millis();
  while (millis() - tiempoAnterior < 1000) {
    // Mantiene el programa responsivo
  }

  // Apaga el LED indicador
  digitalWrite(PIN_LED, LOW);

  // Muestra "Dispensando" centrado en el LCD
  lcd.clear();
  lcd.setCursor((LCD_COLS - strlen("Dispensando...")) / 2, 0);
  lcd.print("Dispensando...");

  // Mueve el servo a 130 grados mientras se muestra "Dispensando..."
  servoMotor.write(130);

  // Espera sin bloquear
  tiempoAnterior = millis();
  while (millis() - tiempoAnterior < 1000) {
    // Mantiene el programa responsivo
  }

  // Regresa el servo a 0 grados
  servoMotor.write(0);

  // Vuelve al mensaje inicial en el LCD
  mostrarMensajeInicial();
}

// Parpadeo del mensaje "Inserte Monedas"
void parpadeoMensaje() {
  if (millis() - ultimoTiempoParpadeo >= 500) {
    ultimoTiempoParpadeo = millis();
    parpadeo = !parpadeo;
    lcd.setCursor((LCD_COLS - strlen("Inserte Monedas")) / 2, 1);
    if (parpadeo) {
      lcd.print("Inserte Monedas");
    } else {
      lcd.print("               ");  // Espacio en blanco para el parpadeo
    }
  }
}
