/*
  ============================================
   SISTEMA DE RIEGO AUTOMATICO CON ARDUINO
  ============================================
  Mide la humedad de la tierra y enciende una
  bomba de agua cuando la tierra esta seca.
  Muestra el estado en una pantalla LCD 16x2.

  COMPONENTES:
  - 1 Placa Arduino (Uno)
  - 1 Pantalla LCD 1602 con modulo I2C
  - 1 Bomba de agua pequena
  - 1 Modulo rele
  - 1 Sensor de humedad de suelo
  - Protoboard y jumpers

  CONEXIONES:
  - LCD I2C:  VCC -> 5V | GND -> GND | SDA -> A4 | SCL -> A5
  - Sensor:   VCC -> 5V | GND -> GND | AO  -> A0
  - Rele:     VCC -> 5V | GND -> GND | IN  -> Pin 2
  - Bomba:    se conecta a traves de COM y NO del rele

  COMO SUBIR EL CODIGO:
  1. Instalar la libreria "LiquidCrystal I2C"
     (Programa > Incluir libreria > Gestionar librerias).
  2. Elegir la placa: Herramientas > Placa > Arduino Uno.
  3. Elegir el puerto: Herramientas > Puerto.
  4. Pulsar el boton "Subir" (flecha).

  NOTA: Si la pantalla no muestra nada, gira el
  tornillo azul de atras del modulo I2C o cambia
  la direccion 0x27 por 0x3F.
*/

#include <LiquidCrystal_I2C.h>   // Libreria para la pantalla LCD

// ---- Configuracion ----
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Direccion I2C, 16 columnas, 2 filas

const int PIN_SENSOR = A0;   // Pin donde se conecta el sensor de humedad
const int PIN_RELE   = 2;    // Pin donde se conecta el rele (bomba)

const int HUMEDAD_ALTA = 500;  // Por debajo de este valor: tierra mojada
const int HUMEDAD_BAJA = 800;  // Por encima de este valor: tierra seca (riega)

// ---- Se ejecuta una sola vez al encender ----
void setup() {
  Serial.begin(9600);          // Activa el Monitor Serial para ver los valores

  lcd.init();                  // Inicia la pantalla
  lcd.backlight();             // Enciende la luz de la pantalla
  lcd.clear();                 // Borra la pantalla

  pinMode(PIN_RELE, OUTPUT);   // El pin del rele es una salida
  digitalWrite(PIN_RELE, HIGH); // HIGH = bomba apagada (el rele se activa con LOW)
  delay(1000);

  // Mensaje de bienvenida
  lcd.setCursor(0, 0);         // Columna 0, fila 0 (arriba)
  lcd.print("SISTEMA DE RIEGO");
  lcd.setCursor(0, 1);         // Columna 0, fila 1 (abajo)
  lcd.print("ACTIVADO");
  delay(3000);                 // Espera 3 segundos
  lcd.clear();
}

// ---- Se repite una y otra vez ----
void loop() {
  // 1. Leer el sensor (valor entre 0 y 1023)
  //    Valor alto = tierra seca | Valor bajo = tierra mojada
  int valor = analogRead(PIN_SENSOR);
  Serial.println(valor);       // Muestra el valor en el Monitor Serial

  // 2. Encender o apagar la bomba
  if (valor > HUMEDAD_BAJA) {
    digitalWrite(PIN_RELE, LOW);    // Tierra seca: enciende la bomba
    lcd.setCursor(0, 0);
    lcd.print("BOMBA: ENCENDIDA");  // 16 caracteres
  } else {
    digitalWrite(PIN_RELE, HIGH);   // Tierra humeda: apaga la bomba
    lcd.setCursor(0, 0);
    lcd.print("BOMBA: APAGADA  ");  // Espacios para borrar letras sobrantes
  }

  // 3. Mostrar el nivel de humedad
  lcd.setCursor(0, 1);
  if (valor < HUMEDAD_ALTA) {
    lcd.print("HUMEDAD: ALTA   ");
  } else if (valor <= HUMEDAD_BAJA) {
    lcd.print("HUMEDAD: MEDIA  ");
  } else {
    lcd.print("HUMEDAD: BAJA   ");
  }

  delay(500);                  // Pequena pausa antes de volver a medir
}
