# Avances Proyecto

## Etapa Uno - ¿Que se diseña?

Para comenzar, diseñé y construí un secuenciador de pasos analógico/digital (*8-Step Sequencer*) controlado por Arduino. El sistema cuenta con control dinámico de la estructura rítmica,
permitiendo alternar la longitud de la secuencia en tiempo real entre patrones de 4, 6 u 8 pasos mediante pulsadores dedicados.

## Etapa Dos - Componentes

### COMPONENTE   -  CANTIDAD

Arduino 101 - 1

Potenciómetros 100K OHM - 9, 8 para notas + 1 para volumen Control

Potenciómetro 10K OHM - 1 Tempo (velocidad)

Botones táctiles (Tact switch) - 3 Selección de 4, 6 u 8 pasos

LEDs (3mm o 5mm) - 8 Indicadores de paso

Resistencias 220 OHM - 8 Protección para LEDs

Circuito Integrado NE555 - 1 Generador de onda/tono

Condensador Cerámico 100 nF - 1 Marcado como "104"

Parlante - 1 Salida de sonido

Diodos 1N4148 - 8 Aislamiento de notas

Resistencia de descarga 10K OHM - 47K OHM - 1 Limpieza de bus de audio

Resistencias para 555 1K OHM y 10K OHM - 1 c/u Configuración de oscilación

Resistencia atenuadora 220K OHM - 1 Para el volumen

Condensador Electrolítico 100 uf - 1 Filtrado de ruido eléctrico

Protoboards - 3 Base física del prototipo

Cables Jumper / USB - Varios Conexión total

## Etapa Tres - Fabricación y Código

Corriente (El puente entre las dos placas)
El Arduino es el que manda la energía a todo mi sistema, y de ahí la paso a las placas para que no meta ruido en el audio:
5V del Arduino: Va directo al riel positivo de mi Placa 1.
GND del Arduino: Va directo al riel negativo de mi Placa 1.
Puente a la Placa 2: Saco un cable del riel positivo de la Placa 1 al positivo de la Placa 2, y otro del riel negativo de la Placa 1 al riel negativo de la Placa 2.
Puentes verticales: En las dos placas hice los puentes verticales para unir el riel de arriba con el de abajo.

Placa 1: El Secuenciador (Los 8 Pasos)
Aquí tengo mi fila de los 8 bloques idénticos de pasos, que dejé amarrados a los pines digitales del Arduino del 2 al 9.
Cómo quedó cada paso (El ejemplo con el Paso 1):
La señal: Mi cable viene desde el Pin 2 del Arduino a una fila horizontal de la Placa 1. Esa fila le da vida al paso.
El LED: En esa misma fila horizontal dejé mi resistencia de 220K OHM El otro extremo va a la pata larga del LED, y la pata corta del LED va directo al riel negativo.
El potenciómetro de nota 100K OHM: Desde esa misma fila horizontal donde entra la señal del Arduino, tiro el cable a la pata izquierda (Terminal 1) del potenciómetro.
La tierra del pote: La pata derecha (Terminal 3) del potenciómetro quedó directo al riel negativo.
El diodo: Clavo el ánodo (el lado sin raya) del diodo 1N4148 en la pata central (Terminal 2) del potenciómetro.
El Bus de Notas (Donde se junta mi melodía):
Busco una fila horizontal larga libre en esta Placa 1.
Conecto el cátodo (el lado con la banda negra) de mis 8 diodos a esa misma fila.
Resistencia de descarga: En esa misma fila donde se juntan todos mis diodos, dejo una resistencia de 10K OHM directo al riel negativo para limpiar los residuos de voltaje.
El cable de salida: De esa fila de los diodos saco mi único cable largo que viaja hacia la Placa 2 con la señal de las notas.
Para los pasos del 2 al 8 mantengo exactamente la misma lógica: Pin 3 al Paso 2, Pin 4 al Paso 3... hasta el Pin 9 para el Paso 8.

Placa 2: Control, Audio y Salida
En esta placa concentro mi "cerebro del audio", el tempo, los botones de los tiempos y la salida del sonido.
Potenciómetro de Tempo 10K OHM
Pata izquierda (Terminal 1): Al riel positivo.
Pata central (Terminal 2): Al pin analógico A0 del Arduino.
Pata derecha (Terminal 3): Al riel negativo.
Los 3 Botones (4, 6 y 8 tiempos)
Para cada pulsador dejo una pata directo al riel negativo y la pata del mismo lado al Arduino:
Botón de 4 al Pin 10.
Botón de 6 al Pin 11.
Botón de 8 al Pin 12.
El Chip de Audio (NE555)
Montado al medio de la placa cruzando el canal central. Mirando el puntito de guía, mis conexiones quedaron así:
Pin 1: Al riel negativo.
Pin 2: Un puente corto de alambre directo al Pin 6.
Pin 3 (Salida): Dejo mi resistencia atenuadora de 220K OHM desde aquí hacia una fila libre para que el volumen no me salte de golpe.
Pin 4 (Reset): Lo conecto con un puente directo al riel positivo para que el chip se mantenga encendido de forma permanente.
Pin 5 (Control): Aquí conecto el cable largo que viene desde el Bus de Notas (la unión de los diodos de la Placa 1).
Pin 6: Unido al Pin 2, y además dejo mi condensador cerámico 100 nF (104) conectado al riel negativo.
Pin 7: Una resistencia de 10K OHM puenteada al Pin 6, y una resistencia de 1K OHM directo al riel positivo.
Pin 8: Al riel positivo.
El Volumen Potenciómetro de 100K OHM y el Buzzer
Pata izquierda (Terminal 1): Al riel negativo.
Pata central (Terminal 2): Al positivo del buzzer pasivo.
Pata derecha (Terminal 3): Va conectada a la fila libre donde llega la resistencia de 220K OHM que sale del Pin 3 del 555.
Buzzer Pasivo: La pata que me quedaba suelta va directo al riel negativo.

### Código - Ayuda de GEMINI:

// Definición de pines
const int pinPotRitmo = A0;
const int pinesLEDs[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int pinBoton4 = 10;
const int pinBoton6 = 11;
const int pinBoton8 = 12;

int totalPasos = 8; // Por defecto empieza en 8 tiempos

void setup() {
  // Configurar pines de LEDs como salidas
  for (int i = 0; i < 8; i++) {
    pinMode(pinesLEDs[i], OUTPUT);
  }
  
  // Configurar pines de botones con resistencia pull-up interna
  pinMode(pinBoton4, INPUT_PULLUP);
  pinMode(pinBoton6, INPUT_PULLUP);
  pinMode(pinBoton8, INPUT_PULLUP);
}

void loop() {
  // Leer los botones para cambiar la cantidad de tiempos (4, 6 u 8)
  if (digitalRead(pinBoton4) == LOW) totalPasos = 4;
  if (digitalRead(pinBoton6) == LOW) totalPasos = 6;
  if (digitalRead(pinBoton8) == LOW) totalPasos = 8;

  // Ciclo para recorrer los pasos secuencialmente
  for (int paso = 0; paso < totalPasos; paso++) {
    
    // 1. Leer la velocidad del ritmo desde el potenciómetro A0
    int valorRitmo = analogRead(pinPotRitmo);
    int tiempoPaso = map(valorRitmo, 0, 1023, 50, 800); // Convierte a milisegundos

    // 2. Encender el LED del paso actual
    digitalWrite(pinesLEDs[paso], HIGH);
    
    // 3. Mantener el paso encendido según el tiempo del potenciómetro
    delay(tiempoPaso); 
    
    // 4. Apagar el LED antes de avanzar al siguiente paso
    digitalWrite(pinesLEDs[paso], LOW);
    
    // Verificar si cambié los tiempos a mitad del ciclo para reaccionar rápido
    if (digitalRead(pinBoton4) == LOW) { totalPasos = 4; break; }
    if (digitalRead(pinBoton6) == LOW) { totalPasos = 6; break; }
    if (digitalRead(pinBoton8) == LOW) { totalPasos = 8; break; }
  }
}

## Etapa Cuatro - Modificación del circuito y del código 

Al principio, cuando monté el circuito, dejé el Pin 4 (Reset) del NE555 conectado directamente al riel positivo ($+$). En esa primera versión, el chip de audio estaba encendido de forma permanente, lo que hacía que el sonido fuera un pitido continuo que solo cambiaba de nota, pero no se cortaba entre pasos. No tenía un ritmo marcado.
Para solucionar esto y lograr un tempo real, desconecté el Pin 4 del positivo y lo tiré directamente al Pin 13 del Arduino.

### Código Ayuda de GEMINI: 

// Definición de pines
const int pinPotRitmo = A0;
const int pinesLEDs[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int pinBoton4 = 10;
const int pinBoton6 = 11;
const int pinBoton8 = 12;
const int pinReset555 = 13; // El pin que controla el tempo del 555

int totalPasos = 8; // Por defecto empieza en 8 tiempos

void setup() {
  // Configurar pines de LEDs como salidas
  for (int i = 0; i < 8; i++) {
    pinMode(pinesLEDs[i], OUTPUT);
  }
  
  // Configurar pines de botones con resistencia pull-up interna
  pinMode(pinBoton4, INPUT_PULLUP);
  pinMode(pinBoton6, INPUT_PULLUP);
  pinMode(pinBoton8, INPUT_PULLUP);
  
  // Configurar el pin de control del 555 como salida
  pinMode(pinReset555, OUTPUT);
}

void loop() {
  // Leer los botones para cambiar la cantidad de tiempos (4, 6 u 8)
  if (digitalRead(pinBoton4) == LOW) totalPasos = 4;
  if (digitalRead(pinBoton6) == LOW) totalPasos = 6;
  if (digitalRead(pinBoton8) == LOW) totalPasos = 8;

  // Ciclo para recorrer los pasos secuencialmente
  for (int paso = 0; paso < totalPasos; paso++) {
    
    // 1. Leer la velocidad del ritmo desde el potenciómetro A0
    int valorRitmo = analogRead(pinPotRitmo);
    int tiempoPaso = map(valorRitmo, 0, 1023, 50, 800); // Convierte a milisegundos

    // 2. Encender el LED del paso actual
    digitalWrite(pinesLEDs[paso], HIGH);
    
    // 3. ¡AQUÍ SE CREA EL TEMPO! Despertamos al 555 enviando 5V
    digitalWrite(pinReset555, HIGH);
    
    // Duración de la nota (suena durante el 70% del tiempo del paso)
    delay(tiempoPaso * 0.70); 
    
    // 4. Apagamos el sonido (Pin 4 a 0V) mientras el LED sigue encendido
    digitalWrite(pinReset555, LOW);
    
    // El silencio del paso (el 30% restante para crear el efecto "bip - bip")
    delay(tiempoPaso * 0.30); 
    
    // 5. Apagar el LED para avanzar al siguiente paso
    digitalWrite(pinesLEDs[paso], LOW);
    
    // Verificar si el usuario cambió los tiempos a mitad del ciclo
    if (digitalRead(pinBoton4) == LOW) { totalPasos = 4; break; }
    if (digitalRead(pinBoton6) == LOW) { totalPasos = 6; break; }
    if (digitalRead(pinBoton8) == LOW) { totalPasos = 8; break; }
  }
}

### Link - Tinkercad: 
https://www.tinkercad.com/things/13fOvJyu5VS-prototipo-3/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits

