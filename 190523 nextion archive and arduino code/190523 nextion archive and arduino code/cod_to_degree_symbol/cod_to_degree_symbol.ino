#include <SoftwareSerial.h>

SoftwareSerial teste (5, 6);

float pi = 3.1415;

String nextion = String((int)pi);

const int botao = 15;
int apertou = 0;
void setup() {
  teste.begin(9600);
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  apertou = analogRead(botao);
  Serial.println(apertou);
  if (apertou > 450 && apertou < 550) {
    teste.print("page camber");
    teste.write(0xff);
    teste.write(0xff);
    teste.write(0xff);
    teste.print("page camber");
    teste.write(0xff);
    teste.write(0xff);
    teste.write(0xff);
  }
  // put your main code here, to run repeatedly:
  teste.print("t1.txt=\"" + String(nextion) + "\xB0" + "\"");
  teste.write(0xff);
  teste.write(0xff);
  teste.write(0xff);
  teste.print("t1.txt=\"" + String(nextion) + "\xB0" + "\"");
  teste.write(0xff);
  teste.write(0xff);
  teste.write(0xff);
}
