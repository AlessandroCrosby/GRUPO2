#include <BluetoothSerial.h>
#include <ESP32Servo.h>

// Crear objeto servo
Servo myservo;

// Objeto Bluetooth
BluetoothSerial SerialBT;

// Variables de control
int servoPos = 135;         // Posición inicial (centro)
int targetDirection = -1;   // -1 = STOP, 0 = STRAIGHT, 1 = RIGHT, 2 = LEFT

void setup() {
  myservo.attach(18, 500, 2500);  // Servo en pin 18 (270°)
  
  Serial.begin(9600);
  SerialBT.begin("ESP32_Servo");
  Serial.println("ESP32 listo, conecta desde App Inventor.");

  myservo.write(servoPos);
}

void loop() {
  // Revisar comandos desde la app
  if (SerialBT.available()) {
    int command = SerialBT.read();
    Serial.print("Comando recibido: ");
    Serial.println(command);

    switch (command) {
      case 0:   // STRAIGHT → solo mantener posición actual
        targetDirection = 0;
        break;

      case 45:  // TURN RIGHT
        targetDirection = 1;
        break;

      case 315: // TURN LEFT
        targetDirection = 2;
        break;

      case 200: // STOP
        targetDirection = -1;
        break;

      case 250: // RESET → volver al centro
        servoPos = 135;
        myservo.write(servoPos);
        targetDirection = -1; // se queda quieto después
        break;
    }
  }

  // Movimiento del servo según targetDirection
  if (targetDirection == 1) {        // Girar a la derecha
    if (servoPos < 270) {
      servoPos++;
      myservo.write(servoPos);
      delay(15); // velocidad del giro (ajustable)
    }
  } 
  else if (targetDirection == 2) {   // Girar a la izquierda
    if (servoPos > 0) {
      servoPos--;
      myservo.write(servoPos);
      delay(15);
    }
  }
  // Si targetDirection = 0 (straight) → se queda fijo
  // Si targetDirection = -1 (stop) → se queda fijo
}
