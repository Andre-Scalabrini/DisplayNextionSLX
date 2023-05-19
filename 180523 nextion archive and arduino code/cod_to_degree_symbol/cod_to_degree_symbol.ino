

float pi = 3.1415;

String nextion = String((int)pi);

const int botao = 15;
int apertou = 0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  apertou = analogRead(botao);
  Serial.println(apertou);
 
}
