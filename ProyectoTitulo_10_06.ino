// 1. Definición de pines para los 8 pasos (Salidas)
const int pinesPasos[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// 2. Definición de pines para los botones (Entradas)
const int boton4Pasos = 10;
const int boton6Pasos = 11;
const int boton8Pasos = 12;

// 3. Pin analógico para el potenciómetro de ritmo
const int pinPotenRitmo = A0; 

void setup() {
  // Configurar los pines de los pasos como salidas limpias
  for (int i = 0; i < 8; i++) {
    pinMode(pinesPasos[i], OUTPUT);
    digitalWrite(pinesPasos[i], LOW); // Asegura que arranquen apagados
  }
  
  // Configurar los botones con la resistencia interna PULLUP
  pinMode(boton4Pasos, INPUT_PULLUP);
  pinMode(boton6Pasos, INPUT_PULLUP);
  pinMode(boton8Pasos, INPUT_PULLUP);
}

void loop() {
  // Establecer el límite de pasos por defecto
  int maxPasos = 8; 
  
  // Leer el estado de los botones (LOW significa presionado)
  if (digitalRead(boton4Pasos) == LOW) {
    maxPasos = 4;
  } 
  else if (digitalRead(boton6Pasos) == LOW) {
    maxPasos = 6;
  } 
  else if (digitalRead(boton8Pasos) == LOW) {
    maxPasos = 8;
  }

  // Ejecutar la secuencia de pasos
  for (int pasoActual = 0; pasoActual < maxPasos; pasoActual++) {
    
    // Leer el potenciómetro de ritmo (Valor entre 0 y 1023)
    int lecturaPoten = analogRead(pinPotenRitmo);
    
    // Convertir la lectura a milisegundos de espera (Ajustable a tu gusto)
    int tempo = map(lecturaPoten, 0, 1023, 60, 600);
    
    // Enciende el paso actual (LED y potenciómetro de nota)
    digitalWrite(pinesPasos[pasoActual], HIGH); 
    
    // Mantiene la nota encendida según el tempo
    delay(tempo); 
    
    // Apaga el paso antes de moverse al siguiente
    digitalWrite(pinesPasos[pasoActual], LOW); 
  }
}