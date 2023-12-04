#include <LiquidCrystal.h>

// Definición de pines
const int buttonPinA = 8;       // Pulsador A
const int buttonPinB = 9;       // Pulsador B
const int buttonPinC = 10;      // Pulsador C
const int buttonPinD = 11;      // Pulsador D
const int selectQuizPin = 12;   // Pulsador para seleccionar cuestionario
const int restartButtonPin = 13; // Pulsador para reiniciar el bucle
const int motorPin = A0;        // Pin del motor DC Gear en A0
const int ledPin = 7;           // LED indicador
const int potPin = 4;           // Pin del potenciómetro
const int lcdBrightnessPin = 3; // Pin de control de brillo LCD

// Variables globales
int seleccionRealizada = 0;     // Variable para registrar si ya se seleccionó A o B
unsigned long tiempoInicio;     // Variable para almacenar el tiempo de inicio del resultado
int nota = 0;                    // Variable para almacenar el puntaje

// Inicialización de LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Función para reiniciar el programa
void reiniciarPrograma() {
  seleccionRealizada = 0;
  nota = 0;
  lcd.clear();
  lcd.print("Escoja un test");
  lcd.setCursor(0, 1);
  lcd.print("A o B");
}

void setup() {
  // Configuración de pines
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  pinMode(buttonPinC, INPUT);
  pinMode(buttonPinD, INPUT);
  pinMode(selectQuizPin, INPUT);
  pinMode(restartButtonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);      // Nuevo pin del motor en A0
  pinMode(lcdBrightnessPin, OUTPUT);

  Serial.begin(9600);

  // Inicialización de LCD
  lcd.begin(16, 2);
  reiniciarPrograma();

  // Ajuste de brillo de LCD basado en potenciómetro
  int brightness = analogRead(potPin) / 2;
  analogWrite(lcdBrightnessPin, brightness);
}

// Función para leer el botón seleccionado
char leerBoton() {
  while (true) {
    if (digitalRead(buttonPinA) == HIGH) {
      return 'A';
    } else if (digitalRead(buttonPinB) == HIGH) {
      return 'B';
    } else if (digitalRead(buttonPinC) == HIGH) {
      return 'C';
    } else if (digitalRead(buttonPinD) == HIGH) {
      return 'D';
    } else if (digitalRead(selectQuizPin) == HIGH) {
      return 0;
    }
  }
}

// Función para leer el botón de reinicio
char leerBotonReinicio() {
  if (digitalRead(restartButtonPin) == HIGH) {
    return 'R';
  }
  return 0;
}

void loop() {
  if (seleccionRealizada == 0) {
    char selectedTest = leerBoton();
    
    // Enciende el motor durante 1 segundo
    digitalWrite(motorPin, HIGH);
    delay(1000);
    digitalWrite(motorPin, LOW);
    delay(500);  // Añade un retraso adicional para asegurar que el motor se apague antes de la siguiente iteración
    
    //Condicional según el test seleccionado
    if (selectedTest == 'A' || selectedTest == 'B') {
      lcd.clear();
      lcd.print("Test ");
      lcd.print(selectedTest);
      delay(2000);
      seleccionRealizada = 1;
      
      
	//Test A
      if (selectedTest == 'A') {
        realizarPregunta("1. 1+1 ", "a. 1   b. 2   c. 3   d. 4", 'B', "Correcto!");
        realizarPregunta("2. 2+2 ", "a. 5   b. 3   c. 4   d. 1", 'C', "Correcto!");
        realizarPregunta("3. 3x5 ", "a. 15   b. 9   c. 22   d. 1", 'A', "Correcto!");
        realizarPregunta("4. 10/2", "a. 5   b. 3   c. 4   d. 1", 'A', "Correcto!");
        realizarPregunta("5. 3+1", "a. 5   b. 3   c. 4   d. 1", 'C', "Correcto!");
      } 
      
      //Test B
      else if (selectedTest == 'B') {
     realizarPregunta("1. Cual es un verbo","a. Comer   b. Azul  c. Mar   d. Lejos", 'A', "Correcto!");
     realizarPregunta("2. Escritor de 'La Odisea'", "a. Platon   b. Homero  c.Lorca  d.Lars", 'B', "Correcto!");
     realizarPregunta("3. Cuantas vocales hay en 'CASA'", "a. 4   b. 3   c. 2   d. 1", 'C', "Correcto!");
     realizarPregunta("4. Antonimo de Alegre", "a. Triste b. Feliz  c.Mal d. Ninguna", 'A', "Correcto!");
     realizarPregunta("5. Dia de la semana", "a.Lunes  b.Mayo  c.Abril   d. Enero", 'A', "Correcto!");
      }
    }
  } else {
    lcd.clear();
    lcd.print("Nota: ");
    lcd.print(nota);
    delay(500);

    if (digitalRead(selectQuizPin) == HIGH) {
      reiniciarPrograma();
    }

    char reiniciar = leerBotonReinicio();
    if (reiniciar == 'R') {
      reiniciarPrograma();
    }
  }
}

// Función para realizar una pregunta
void realizarPregunta(const char *pregunta, const char *opciones, char opcionCorrecta, const char *mensajeCorrecto) {
  lcd.clear();
  lcd.print(pregunta);
  lcd.setCursor(0, 1);
  lcd.print(opciones);

  delay(1000);

  // Desplaza el texto en la pantalla LCD para mostrar todas las opciones
  for (int positionCounter = 1; positionCounter <= 22; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }
  lcd.clear();
  lcd.print(pregunta);
  lcd.setCursor(0, 1);
  lcd.print(opciones);

  char opcionSeleccionada = leerBoton();

  if (opcionSeleccionada == opcionCorrecta) {
    lcd.clear();
    lcd.print(mensajeCorrecto);
    delay(2000);
    nota = nota + 2;
    
    // Enciende el motor durante 1 segundo
    digitalWrite(motorPin, HIGH);
    delay(1000);
    digitalWrite(motorPin, LOW);
    delay(500);  // Añade un retraso adicional para asegurar que el motor se apague antes de la siguiente iteración
  } else {
    lcd.clear();
    lcd.print("Incorrecto");
    delay(2000);
    
    // Enciende el motor durante 1 segundo
    digitalWrite(motorPin, HIGH);
    delay(1000);
    digitalWrite(motorPin, LOW);
    delay(500);  // Añade un retraso adicional para asegurar que el motor se apague antes de la siguiente iteración
  }
}