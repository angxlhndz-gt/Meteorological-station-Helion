#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int motorSpeed = 800; 

int lookupDerecha[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

int lookupIzquierda[8] = {B00001, B00011, B00010, B00110, B00100, B01100, B01000, B01001};

int motorPin1 = 2; 
int motorPin2 = 3; 
int motorPin3 = 4; 
int motorPin4 = 5; 
const int csPin = 6;
int switchPin = 7;
int motor2Pin1 = 8; 
int motor2Pin2 = 9; 
int motor2Pin3 = 10; 
int motor2Pin4 = 11; 

int ldrPin = A0; 

int switchState = 0; 
int h1, h2, h3, h4, h5, h6, h7, h8;
int v1, v2, v3, v4,v5;
int he1, he2, he3, he4;

float porcentaje = 0.20; 
float Tempv;

void setup() {
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2Pin3, OUTPUT);
  pinMode(motor2Pin4, OUTPUT);
  pinMode(ldrPin, INPUT); 
  pinMode(switchPin, INPUT); 
  SPI.begin();
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);

  lcd.init();              
  lcd.backlight();          
  lcd.setCursor(0, 0);      
  lcd.print("    Iniciando   "); 
  lcd.setCursor(0, 1);    
  lcd.print("     Sistema    "); 
  delay(2000);
  lcd.clear();

  Serial.begin(115200);
}

void setOutputMotor1(int out) {
  digitalWrite(motorPin1, bitRead(out, 0));
  digitalWrite(motorPin2, bitRead(out, 1));
  digitalWrite(motorPin3, bitRead(out, 2));
  digitalWrite(motorPin4, bitRead(out, 3));
}

void setOutputMotor2(int out) {
  digitalWrite(motor2Pin1, bitRead(out, 0));
  digitalWrite(motor2Pin2, bitRead(out, 1));
  digitalWrite(motor2Pin3, bitRead(out, 2));
  digitalWrite(motor2Pin4, bitRead(out, 3));
}

void MoverMotorDerecha(int pasos){
  for (int i = 0; i < pasos; i++) { 
    for (int j = 0; j < 8; j++) {
      setOutputMotor1(lookupDerecha[j]);
      delayMicroseconds(motorSpeed);
    }
  }
}

void MoverMotorIzquierda(int pasos){
  for (int i = 0; i < pasos; i++) { 
    for (int j = 0; j < 8; j++) {
      setOutputMotor1(lookupIzquierda[j]);
      delayMicroseconds(motorSpeed);
    }
  }
} 

void MoverMotor2Derecha(int pasos){
  for (int i = 0; i < pasos; i++) { 
    for (int j = 0; j < 8; j++) {
      setOutputMotor2(lookupDerecha[j]);
      delayMicroseconds(motorSpeed);
    }
  }
}

void MoverMotor2Izquierda(int pasos){
  for (int i = 0; i < pasos; i++) { 
    for (int j = 0; j < 8; j++) {
      setOutputMotor2(lookupIzquierda[j]);
      delayMicroseconds(motorSpeed);
    }
  }
}

void ReconocimientoV(){

  v1 = analogRead(ldrPin);
  delay(2000);
  MoverMotor2Derecha(32);

  v2 = analogRead(ldrPin);
  delay(2000);
  MoverMotor2Derecha(32);

  v3 = analogRead(ldrPin);
  delay(2000);
  MoverMotor2Derecha(32);

  v4 = analogRead(ldrPin);
  delay(2000);
  MoverMotor2Derecha(32);

  v5 = analogRead(ldrPin);
  delay(2000);
  MoverMotor2Izquierda(128);

}
void ReconocimientoEsp(){

  he1 = analogRead(ldrPin);
  MoverMotorDerecha(16);
  delay(2000);

  he2 = analogRead(ldrPin);
  MoverMotorDerecha(16);
  delay(2000);

  he3 = analogRead(ldrPin);
  MoverMotorDerecha(16);
  delay(2000);

  he4 = analogRead(ldrPin);
  MoverMotorDerecha(16);
  delay(2000);

  MoverMotorIzquierda(64);
}

void ReconocimientoH() {

  h1 = analogRead(ldrPin);
  MoverMotorDerecha(64);
  delay(2000);

  h2 = analogRead(ldrPin);
  MoverMotorDerecha(64);
  delay(2000);
  
  h3 = analogRead(ldrPin);
  MoverMotorDerecha(64);
  delay(2000);

  h4 = analogRead(ldrPin);
  MoverMotorDerecha(64);
  delay(2000);
  
  MoverMotorIzquierda(256);
  delay(2000);
  
  MoverMotorIzquierda(64);
  h5 = analogRead(ldrPin);
  delay(2000);

  MoverMotorIzquierda(64);
  h6 = analogRead(ldrPin);
  delay(2000);
  
  MoverMotorIzquierda(64);
  h7 = analogRead(ldrPin);
  delay(2000);
  

  MoverMotorIzquierda(64);
  h8 = analogRead(ldrPin); 
  delay(2000);
  
  MoverMotorDerecha(256);
}

float DatoTemperatura(){
  digitalWrite(csPin, LOW); 
  delayMicroseconds(10);
  byte highByte = SPI.transfer(0x00);
  byte lowByte = SPI.transfer(0x00);
  digitalWrite(csPin, HIGH);
  int value = ((highByte << 8) | lowByte) >> 3; 
  float temperature = value * 0.25; 
  return temperature;
}

int printLDRValue() {
  int ldrValue = analogRead(ldrPin);
  return ldrValue;
}

int findMinIndex(int arr[], int size) {
    int minIdx = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[minIdx]) {
            minIdx = i;
        }
    }
    return minIdx;
}

void E1 (){
  Tempv = analogRead(ldrPin); 
  int porcentajeESP = Tempv * (1 + porcentaje);
  while (true) {
    Tempv = analogRead(ldrPin);
    int ldr = printLDRValue();
    float temperatura = DatoTemperatura();
    int CordY = 90;
    Serial.print(ldr); Serial.print(","); Serial.print(temperatura); Serial.print(","); Serial.println(CordY);
    if (Tempv > porcentajeESP) {  
      break;
    }
    delay(5000);
  }
}

void E2 (){
  MoverMotorDerecha(16);
  Tempv = analogRead(ldrPin); 
  int porcentajeESP = Tempv * (1 + porcentaje); 
  while (true) {
    Tempv = analogRead(ldrPin);
    int ldr = printLDRValue();
    float temperatura = DatoTemperatura();
    int CordY = 90;
    Serial.print(ldr); Serial.print(","); Serial.print(temperatura); Serial.print(","); Serial.println(CordY);
    if (Tempv > porcentajeESP) {  
      break;
    }
    delay(5000);
  }
  MoverMotorIzquierda(16);
}

void E3 (){
  MoverMotorDerecha(32);
  Tempv = analogRead(ldrPin); 
  int porcentajeESP = Tempv * (1 + porcentaje);  
  while (true) {
    Tempv = analogRead(ldrPin);
    int ldr = printLDRValue();
    float temperatura = DatoTemperatura();
    int CordY = 90;
    Serial.print(ldr); Serial.print(","); Serial.print(temperatura); Serial.print(","); Serial.println(CordY);
    if (Tempv > porcentajeESP) { 
      break;
    }
    delay(5000);
  }
  MoverMotorIzquierda(32);
}

void E4 (){
  MoverMotorDerecha(48);
  Tempv = analogRead(ldrPin); 
  int porcentajeESP = Tempv * (1 + porcentaje);  
  while (true) {
    Tempv = analogRead(ldrPin);
    int ldr = printLDRValue();
    float temperatura = DatoTemperatura();
    int CordY = 90;
    Serial.print(ldr); Serial.print(","); Serial.print(temperatura); Serial.print(","); Serial.println(CordY);
    if (Tempv > porcentajeESP) {  
      break;
    }
    delay(5000);
  }
  MoverMotorIzquierda(48);

}

void H1 (){
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
}

void H2 (){
  MoverMotorDerecha(64);
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
  MoverMotorIzquierda(64);
}

void H3 (){
  MoverMotorDerecha(128);
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
  MoverMotorIzquierda(128);
}

void H4 (){
  MoverMotorDerecha(192);
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
  MoverMotorIzquierda(192);
}

void H5 (){
  MoverMotorIzquierda(64);
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
  MoverMotorDerecha(64);
}

void H6 (){
  MoverMotorIzquierda(128);
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
  MoverMotorDerecha(128);
}

void H7 (){
  MoverMotorIzquierda(192);
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
  MoverMotorDerecha(192);
}

void H8 (){
  MoverMotorIzquierda(256);
  ReconocimientoEsp();
  int hArray[4] = {he1,he2,he3,he4};
  int minIndex = findMinIndex(hArray, 4);

  switch (minIndex) {
    case 0: E1(); break;
    case 1: E2(); break;
    case 2: E3(); break;
    case 3: E4(); break;
  }
  MoverMotorDerecha(256);
}

void loop() {

  switchState = digitalRead(switchPin); 

  if (switchState == LOW) { 
    delay(3000);
    ReconocimientoH();

    int hArray[8] = {h1, h2, h3, h4, h5, h6, h7, h8};
    int minIndex = findMinIndex(hArray, 8);

    switch (minIndex) {
      case 0: H1(); break;
      case 1: H2(); break;
      case 2: H3(); break;
      case 3: H4(); break;
      case 4: H5(); break;
      case 5: H6(); break;
      case 6: H7(); break;
      case 7: H8(); break;
    }
  } else {
    MoverMotor2Derecha(128);
    while (digitalRead(switchPin) == HIGH) {
      lcd.init();              
      lcd.backlight();          
      lcd.setCursor(0, 0);      
      lcd.print("      Modo      "); 
      lcd.setCursor(0, 1);    
      lcd.print("  Mantenimiento "); 
      delay(2000);
      lcd.clear();
    }
  }
  MoverMotor2Izquierda(128);
}
  
