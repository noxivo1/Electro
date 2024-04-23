#include <Wire.h> 
#include <LiquidCrystal_I2C.h>  // Incluye la biblioteca LCD I2C
#include <Servo.h>              // Incluye la biblioteca para controlar el servo

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

// Configuración inicial del programa
void setup() {
  // Inicialización del puerto serial a 9600 baudios
  Serial.begin(115200);

  // Inicialización del LCD
  lcd.init();
  lcd.backlight();

  // Inicialización del servo
  servoMotor.attach(PIN_SERVO);
  servoMotor.write(0);  // Posiciona el servo en 0 grados

  // Mostrar mensajes centrados
  lcd.setCursor((LCD_COLS - 13) / 2, 0);  // Centrar "BIENVENIDO"
  lcd.print("BIENVENIDO");
  lcd.setCursor((LCD_COLS - 15) / 2, 1);  // Centrar "Inserte Monedas"
  lcd.print("Inserte Monedas");

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
    // Reinicia la bandera de inserción de moneda
    monedaInsertada = false;

    // Enciende el LED
    digitalWrite(PIN_LED, HIGH);

    // Actualiza el LCD
    lcd.clear();
    lcd.setCursor((LCD_COLS - 11) / 2, 0);  // Centrar "$500 Pesos"
    lcd.print("$500 Pesos");

    // Apaga el LED indicador
    digitalWrite(PIN_LED, LOW);

    delay(1000);  // Espera 1 segundos

    // Muestra "Dispensando" centrado en el LCD
    lcd.clear();
    lcd.setCursor((LCD_COLS - 12) / 2, 0);  // Centrar "Dispensando..."
    lcd.print("Dispensando...");

    // Mueve el servo a 130 grados mientras se muestra "Dispensando..."
    servoMotor.write(130);
    delay(1000);  // Espera 1 segundo

    // Detiene y apaga el servo
    servoMotor.detach();
    pinMode(PIN_SERVO, INPUT);

    // Regresa el servo a 0 grados
    servoMotor.attach(PIN_SERVO);
    servoMotor.write(0);
    delay(1000);  // Espera 1 segundo

    // Vuelve al mensaje inicial en el LCD
    mostrarMensajeInicial();
    
    // Reinicia el contador de pulsos
    conteoPulsos = 0;
  }

  // Parpadeo del mensaje "Inserte Monedas"
  if (millis() - ultimoTiempoParpadeo >= 500) {
    ultimoTiempoParpadeo = millis();
    parpadeo = !parpadeo;
    lcd.setCursor((LCD_COLS - 15) / 2, 1);
    if (parpadeo) {
      lcd.print("Inserte Monedas");
    } else {
      lcd.print("               ");  // Espacio en blanco para el parpadeo
    }
  }
}

// Función para mostrar el mensaje inicial en el LCD
void mostrarMensajeInicial() {
  lcd.clear();
  lcd.setCursor((LCD_COLS - 13) / 2, 0);  // Centrar "BIENVENIDO"
  lcd.print("BIENVENIDO");
  lcd.setCursor((LCD_COLS - 15) / 2, 1);  // Centrar "Inserte Monedas"
  lcd.print("Inserte Monedas");
}

// Función de interrupción para detectar los pulsos de la moneda
void interrupcionMoneda() {
  // Incrementa el contador de pulsos
  conteoPulsos++;

  // Establece la bandera de inserción de moneda
  monedaInsertada = true;
}
