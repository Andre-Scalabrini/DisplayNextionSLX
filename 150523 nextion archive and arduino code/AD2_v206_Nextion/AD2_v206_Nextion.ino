
/* Pin mapping

Board   Arduino
 TADC   15
 L1     3
 L2     2
----------------Display
 BL     23
 EN     24
 RS     4
 D4     8
 D5     7
 D6     6
 D7     5
----------------Battery
SVB     25
SCB     25
----------------BMA255
INT1    9
INT2    10
SDA     18
SCL     19
----------------RS-232
RX      0
TX      1
----------------NC
PE3     26
PC0     14
PC2     16
PC3     17
------------------------
*/

//Loading libs
#include <Wire.h>
#include <math.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial Nextion(10, 11); //RX, TX

//DISPLAY PINS
const int PIN_RS = 4;
const int PIN_D4 = 8;
const int PIN_D5 = 7;
const int PIN_D6 = 6;
const int PIN_D7 = 5;
const int PIN_EN = 24;

const int PIN_RX = 0;
const int PIN_TX = 1;

const int PIN_BMA_SDA = 18;
const int PIN_BMA_SCL = 19;
const int PIN_BMA_INT1 = 9;
const int PIN_BMA_INT2 = 10;

const int PIN_LASER_1 = 3;
const int PIN_LASER_2 = 2;
const int PIN_TECLADO = 15;
const int PIN_BVOLTAGE = 25;
const int PIN_BACKLIGHT = 23;
const int PIN_CHARGER_STATUS = 25;

//EEPROM
const int AcXAdrs = 0;
const int AcYAdrs = 10;
const int AcZAdrs = 20;
const int whpAdrs = 40;
const int trnAdrs = 60;
const int ltmAdrs = 70;
const int dtmAdrs = 75;
const int senAdrs = 80;
const int kpiAdrs = 100;
const int batAdrs = 120;
const int txtAdrs = 140;
const int epsAdrs = 150;
const int cnlAdrs = 160;
const int chdAdrs = 170;
const int bpvAdrs = 180;
//const int blvAdrs = 180;
//const int bhvAdrs = 190;
const int kbdAdrs = 200;
const int txsAdrs = 240;
const int tx1Adrs = 280;
const int tx2Adrs = 380;
const int nvlAdrs = 480;
const int sbsAdrs = 490;
const int splAdrs = 500; 

char txtInic[80] = {
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ','S','L','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ','T','E','C','N','O','L','O','G','I','A',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
  };

char txtSerial[5] = {
  ' ',' ',' ',' ',' '
};

byte buf2[2];

//Next page status
bool next;

//Status do Display
bool displayStat;

//Status do Laser
bool laserStat1;
bool laserStat2;
uint8_t timerLaser;
uint8_t timerDisplay;
//uint8_t timerBateria;

//Spoiler
bool sp;

//Lock Status
bool lock;

//Refresh flags
bool refreshDisplay;
bool refreshData;

//B icons

const int b00 = 0;
const int b25 = 1;
const int b50 = 2;
const int b75 = 3;
const int b100 = 4;
const int bcar = 5;
const int bub = 6;
const int wif = 7;


byte charging[8] = {
  B00001,
  B00110,
  B11100,
  B11110,
  B01111,
  B00111,
  B01100,
  B10000,
};

byte bZero[8] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
};

byte bLow[8] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
};

byte bMid[8] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte bHigh[8] = {
  B01110,
  B11011,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte bFull[8] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte levelBubble[8] = {
  B11111,
  B11111,
  B10001,
  B00000,
  B00000,
  B10001,
  B11111,
  B11111,
};

byte wicon[8] = {
  B00000,
  B01110,
  B10001,
  B00100,
  B01010,
  B00000,
  B00100,
  B00100,
};

//I2C adress BMA255
const int BMA = 0x18;

int id;
int byte01;

//Battery info
//int bvLow;
//int bvHigh;
int bCntr;
unsigned int bVoltage;
int bvParam;
int abVoltage;
bool carregando;

//Valor de parametro
const float vConst = 8.0;
//Tensao atual
float vAtual;
int pBat;

int pswd;

//LEVEL ANIMATION
int runCont;

//Finite State Machine
int state;
//const int lowb = -15;
const int parb = -44;
const int trns = -43;
const int text = -42;
const int vers = -13;
const int rods = -12;
const int timd = -11;
const int timl = -10;
const int skbd = -9;
const int sets = -8;
const int setk = -7;
const int angs = -6;
const int sdis = -5;
const int chan = -4;
const int ebat = -3;
const int splr = -2;
const int inic = -1;
const int camb = 0;
const int kpi1 = 1;
const int kpi2 = 2;
const int kpi3 = 3;
const int kpi4 = 4;
/*const int kpi5 = 5;
  const int kpi6 = 6;*/

//Switch 1 to 6
int swA;
const int sw1 = 1;
const int sw2 = 2;
const int sw3 = 3;
const int sw4 = 4;
const int sw5 = 5;
const int sw6 = 6;
const int swN = 7;
const int swC = 8;
const int swE = 9;
const int swD = 10;

int voltage;

//Wheel position
const int fEsq = 0;
const int fDir = 1;
const int tDir = 3;
const int tEsq = 2;
uint8_t WPos;
int chnl;

//Angular variable
int TurnAngle;

//KPI Function State
int kpis;
const int kpiOn = 1;
const int kpiOff = 0;

//Camber, Caster, KPI
float kpi;
float arm;
float camber;
float caster;

//Camber_L, Caster_L, KPI_L
float lkpi;
float lcamber;
float lcaster;

//Angle Refs - KPI CASTER
float armZero;
float camberZero;

//Display center variable
int chrd;

int eepromSetup;

//Texto de Exibicao
const int TXT1 = 0;
const int TXT2 = 1;
const int TXT3 = 2;
int exibTxt;

//Keyboard
const int PDR1 = 1;
const int PDR2 = 2;
int kboard;

//Nivel
int eNivel;
const int ngrad = 1;
const int nnume = 2;

//Porcentagem de bateria
int bTipo;
const int bShow = 1;
const int bHide = 2;

//Sploiler
int spSts;
const int spOn = 1;
const int spOff = 0;

//Refresh value
const long mrefresh = 500;
const long minute = 60000;
unsigned long lastRefresh;
unsigned long lastClick;
unsigned long timeOn;
unsigned long timeBateria;
unsigned long TimerCamb;
unsigned long timerInicial;

//Sensor Reading
int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
double FzyR, SzyR, FzxR, SzxR;
double SAcX, SAcY, SAcZ;
double MAcX, MAcY, MAcZ;
double MAcXO, MAcYO, MAcZO;
double MAcXN, MAcYN, MAcZN;
double FAcX, FAcY, FAcZ;
double RAcX, RAcY, RAcZ;
double xAngle, yAngle;
int TG;
int i;
int sense;

//Setting LCD
LiquidCrystal lcd(PIN_RS, PIN_EN, PIN_D4, PIN_D5, PIN_D6, PIN_D7);

//DATA OVER WIFI CONFIG
typedef union
{
  float value;
  uint8_t Byte[4];
} FLOAT_UNION;

FLOAT_UNION levl;
FLOAT_UNION cmbr;
FLOAT_UNION cstr;
FLOAT_UNION king;
FLOAT_UNION conv;
FLOAT_UNION cent;

const int DataSize = 42;

uint8_t ChHd;
uint8_t timer;
uint8_t Data[DataSize];

char serialData;

void AcelRead()
{
}

void setup()
{
  MAcX = 0;
  MAcY = 0;
  MAcZ = 0;
  MAcXO = 0;
  MAcYO = 0;
  MAcZO = 0;
  MAcXN = 0;
  MAcYN = 0;
  MAcZN = 0;
  
  //CONVERGENCIA E CENTRALIZACAO
  conv.value = 0;
  cent.value = 0;

  sp = false;
  carregando = false;

  //Setting NC pins
  pinMode(14, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(26, INPUT);

  //Setting display
  //DEFINIR NOVO DISPLAY COM SOFTWARE SERIAL. (NEXTION)
  pinMode(PIN_EN, OUTPUT);
  pinMode(PIN_RS, OUTPUT);
  pinMode(PIN_D4, OUTPUT);
  pinMode(PIN_D5, OUTPUT);
  pinMode(PIN_D6, OUTPUT);
  pinMode(PIN_D7, OUTPUT);


  //Setting keyboard
  pinMode(PIN_TECLADO, INPUT);
  //Setting backlight
  pinMode(PIN_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_BACKLIGHT, HIGH);

  //Setting Battery Status
  pinMode(PIN_BVOLTAGE, INPUT);

  //Setting Laser
  pinMode(PIN_LASER_1, OUTPUT);
  pinMode(PIN_LASER_2, OUTPUT);

  //Setting Serial
  Serial.begin(115200);

  //Setting software Serial
  Nextion.begin(9600);

  //Starting LCD 16x2
  lcd.begin(20, 4);
  //Creating char
  lcd.createChar(b00, bZero);
  lcd.createChar(b25, bLow);
  lcd.createChar(b50, bMid);
  lcd.createChar(b75, bHigh);
  lcd.createChar(b100, bFull);
  lcd.createChar(bcar, charging);
  lcd.createChar(bub, levelBubble);
  lcd.createChar(wif, wicon);
  lcd.setCursor(0,0);
  lcd.noCursor();

  //Start BMA255
  
  //LOADING INITIAL CONFIG

  //LOAD EEPROM STATUS
  EEPROM.get(epsAdrs, eepromSetup);

  //LOAD PADRAO DE TEXTO
  EEPROM.get(txtAdrs, exibTxt);
  
    //Se eepromSetup != 0, configurar valores eeprom!
  if (eepromSetup != 0)
  {
    WPos = fEsq;
    TurnAngle = 10;
    sense = 5;
    
    kpis = kpiOff;
    
    //LOAD WHEEL POSSITION
    EEPROM.put(whpAdrs, WPos);
    EEPROM.put(trnAdrs, TurnAngle);
    EEPROM.put(senAdrs, sense);
  
    //LOAD KPI FUNCTION STATE (ON/OFF)
    EEPROM.put(kpiAdrs, kpis);

    chnl = 1;
    chrd = 0;
    timerLaser = 10;
    timerDisplay = 10;
        
    bvParam = 800;

    eepromSetup = 0;

    kboard = PDR2;

    exibTxt = TXT1;

    EEPROM.put(cnlAdrs, chnl);
    EEPROM.put(chdAdrs, chrd);
    EEPROM.put(ltmAdrs, timerLaser);
    EEPROM.put(dtmAdrs, timerDisplay);
    
    EEPROM.put(bpvAdrs, bvParam);
    
    EEPROM.put(epsAdrs, eepromSetup);

    EEPROM.put(kbdAdrs, kboard);
    
    EEPROM.put(txtAdrs, exibTxt);
    
    EEPROM.put(txsAdrs, txtSerial);

    EEPROM.put(tx1Adrs, txtInic);
    
    EEPROM.put(tx2Adrs, txtInic);

    //MARCADOR DE BATERIA
    bTipo = bShow;
    EEPROM.put(sbsAdrs, bTipo);

    //FUNCAO SPOILER
    spSts = spOn;
    EEPROM.put(splAdrs, spSts);

    //ESTILO NIVEL
    //eNivel = ngrade;
    //EEPROM.put(nvlAdrs, eNivel);
    
  }

  //CARREGA TEXTO DE INICIALIZAÇÃO
  if(exibTxt == TXT2)
  {
    EEPROM.get(tx2Adrs, txtInic);
  }
  else
  {
    EEPROM.get(tx1Adrs, txtInic);
  }

  //EXIBE TEXTO DE INICIALIZAÇÃO
  lcd.clear();
  for (int it=0; it<80; it++)
  {
    if(it == 0)
    {
      lcd.setCursor(0,0);
    }
    else if(it == 20)
    {
      lcd.setCursor(0,1);
    }
    else if(it == 40)
    {
      lcd.setCursor(0,2);
    }
    else if(it == 60)
    {
      lcd.setCursor(0,3);
    }
    lcd.print(txtInic[it]);
  }

  if(exibTxt == TXT3)
  {
    EEPROM.get(tx2Adrs, txtInic);
    delay(2000);
    lcd.clear();
    for (int it=0; it<80; it++)
    {
      if(it == 0)
      {
        lcd.setCursor(0,0);
      }
      else if(it == 20)
      {
        lcd.setCursor(0,1);
      }
      else if(it == 40)
      {
        lcd.setCursor(0,2);
      }
      else if(it == 60)
      {
        lcd.setCursor(0,3);
      }
      lcd.print(txtInic[it]);
    }
  }

  //CARREGA CÓDIGO SERIAL
  EEPROM.get(txsAdrs, txtSerial);

  //Clear switch
  swA = 0;

  //Clear resfresh
  lastRefresh = 0;

  //Clear timeBateria;
  timeBateria = 0;

  //Bateria
  bCntr = 0;
  bVoltage = 0;
  abVoltage = analogRead(PIN_BVOLTAGE);

  pswd = 0;

  //Clean Lock
  lock = LOW;

  //First Refresh
  refreshData = HIGH;

  //Initial state
  state = camb;
  
  //Clean CAMBER, CASTER, KPI;
  kpi = 0;
  arm = 0;
  camber = 0;
  caster = 0;

  //Clean CAMBER_L, CASTER_L, KPI_L;
  lkpi = 0;
  lcamber = 0;
  lcaster = 0;

  //Clean refs
  camberZero = 0;
  armZero = 0;

  //LEVEL ANIMATION
  runCont = 1;

  //LOAD REFS FROM EEPROM
  EEPROM.get(AcXAdrs, SAcX);
  EEPROM.get(AcYAdrs, SAcY);
  EEPROM.get(AcZAdrs, SAcZ);

  //LOAD DISPLAY MODE
  EEPROM.get(chdAdrs, chrd);

  //LOAD KEYBORD MODE
  EEPROM.get(kbdAdrs, kboard);

  //LOAD WHEEL POSSITION
  EEPROM.get(whpAdrs, WPos);

  //ANGLE FOR KPI/CASTER MEASURE
  EEPROM.get(trnAdrs, TurnAngle);

  //LASER TIMER
  EEPROM.get(ltmAdrs, timerLaser);

  //DISPLAY BACKLIGHT TIMER
  EEPROM.get(dtmAdrs, timerDisplay);

  //DEGREES PRECISION
  EEPROM.get(senAdrs, sense);

  //KPI FUNCTION (ON/OFF)
  EEPROM.get(kpiAdrs, kpis);

  //LOAD CHANNEL
  EEPROM.get(cnlAdrs, chnl);

  //BATTERY MIN AND MAX
  EEPROM.get(bpvAdrs, bvParam);

  //BATTERY LEVEL INDICATOR
  EEPROM.get(sbsAdrs, bTipo);

  //SPOILER FUNCTION (ON/OFF)
  EEPROM.get(splAdrs, spSts);

  //ANGLE LEVEL INDICATOR STYLE
  //EEPROM.get(nvlAdrs, eNivel);

  displayStat = HIGH;
  next = LOW;

  serialData = 0;

  //ler acelerometro
  i = 0;
  for(int o = 0; o < 202; o++)
  {
    AcelRead();
    delay(10);
  }
  
  //delay(1000);
  timerInicial = millis() + 1000;
  lcd.clear();
  
}


void printGraus(float graus)
{
  int gminutos;

  gminutos = (int)((graus - (int)graus) * 60);

  if (graus > 0)
  {
    lcd.print(" ");
    lcd.print((int)graus);
    lcd.write(B11011111);
    if (gminutos < 10)
      lcd.print("0");
    lcd.print( (int)(gminutos / sense)*sense);
    lcd.print("' ");
  }
  else if (graus < 0)
  {
    lcd.print("-");
    lcd.print(-1 * (int)graus);
    lcd.write(B11011111);
    //lcd.print(-1* gminutos);
    if (gminutos > -10)
      lcd.print("0");
    lcd.print(-1 * (int)(gminutos / sense)*sense);
    lcd.print("' ");
  }
  else
    lcd.print("NAN");
}

void level(float data)
{
  if (spSts == spOn)
  {
    if (data < 10 || data > -10)
      sp = false;
  
    if (data > 10)
    {
      sp = true;
      data = data - 15;
    }
  
    if (data < -10)
    {
      sp = true;
      data = data + 15;
    }
  }

  lcd.setCursor(0 + chrd, 0);

  if (data > 1.5)
  {
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(byte(bub));
  }
  else if (data > 0.2 && data < 1.5)
  {
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(byte(bub));
    lcd.write(B11111111);
  }
  else if (data < 0.2 && data > -0.2)
  {
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(byte(bub));
    lcd.write(B11111111);
    lcd.write(B11111111);
  }
  else if (data < -0.2 && data > -1.5)
  {
    lcd.write(B11111111);
    lcd.write(byte(bub));
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(B11111111);
  }
  else if (data < -1.5)
  {
    lcd.write(byte(bub));
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(B11111111);
    lcd.write(B11111111);
  }

  if (sp)
    lcd.print("SP");

  if (data > 0.2 || data < -0.2)
  {
    if (runCont == 1)
    {
      lcd.print("  <");
      runCont++;
    }
    else if (runCont == 2)
    {
      lcd.print(" <<");
      runCont++;
    }
    else if (runCont == 3)
    {
      lcd.print("<<<");
      runCont++;
    }
    else if (runCont == 4)
    {
      lcd.print("<< ");
      runCont++;
    }
    else if (runCont == 5)
    {
      lcd.print("<  ");
      runCont++;
    }
    else if (runCont == 6)
    {
      lcd.print("   ");
      runCont = 1;
    }
    if (sp)
      lcd.print("NIVEL");
    else
      lcd.print("NIVELAR");
  }
  else
    lcd.print("          ");

}

int AnalogSW()
{
  if (analogRead(PIN_TECLADO) < 280)
    return 0;

  delay(100);

  voltage = analogRead(PIN_TECLADO);

  if (kboard == PDR1)
  {
    if (timerInicial > millis())
    {
      if (voltage > 860 && voltage < 900)
        return swN;
      else if (voltage > 620 && voltage < 660)
        return swC;
      else
        return 0;
    }
    else if (voltage > 780 && voltage < 880)
      return sw1;
    else if (voltage > 660 && voltage < 780)
      return sw2;
    else if (voltage > 570 && voltage < 660)
      return sw3;
    else if (voltage > 480 && voltage < 570)
      return sw4;
    else if (voltage > 380 && voltage < 480)
      return sw5;
    else if (voltage > 280 && voltage < 380)
      return sw6;
    else
      return 0;
  }
  else //if (kboard == PDR2)
  {
    if (timerInicial > millis())
    {
      if (voltage > 860 && voltage < 900)
        return swC;
      else if (voltage > 620 && voltage < 660)
        return swN;
      else
        return 0;
    }
    else if (voltage > 780 && voltage < 880)
      return sw5;
    else if (voltage > 660 && voltage < 780)
      return sw4;
    else if (voltage > 570 && voltage < 660)
      return sw3;
    else if (voltage > 480 && voltage < 570)
      return sw2;
    else if (voltage > 380 && voltage < 480)
      return sw1;
    else if (voltage > 280 && voltage < 380)
      return sw6;
    else
      return 0;
  }
}

void loop()
{

  swA = AnalogSW();

  if (swA != 0)
  {
    lastClick = millis();
    displayStat = HIGH;
    digitalWrite(PIN_BACKLIGHT, displayStat);
  }

  if (timerLaser != 0 && millis() > (timeOn + timerLaser * minute)
      && laserStat1 == HIGH )
  {
    laserStat1 = LOW;
    laserStat2 = LOW;
    //lcd.setCursor(2, 1);
    //lcd.print("LASER DESLIGADO");
    digitalWrite(PIN_LASER_1, LOW);
    digitalWrite(PIN_LASER_2, LOW);
  }

  if (timerDisplay != 0 && (millis() > (lastClick + timerDisplay * minute)) )
  {
    displayStat = LOW;
    digitalWrite(PIN_BACKLIGHT, displayStat);
  }

  //Initial config.
  if(timerInicial > millis())
  {
    swA = AnalogSW();

    //B1+B2-----------------------------------
    if (swA == swN)
    {
      state = inic;
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("CONFIGURACAO");
      lcd.setCursor(5, 2);
      lcd.print("DE ANGULO");
      delay(1000);
      lcd.clear();
    }
    //B1+B3-----------------------------------
    if (swA == swC)
    {
      state = vers;
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("CONFIGURACAO");
      lcd.setCursor(5, 2);
      lcd.print("DE FABRICA");
      delay(1000);
      lcd.clear();
    }
  }
  
  //if (millis() < (4 * mrefresh + 100))
    //lcd.clear();

  if (swA == 0)
    TimerCamb = 0;
  //B1--------------------------------------
  if (swA == sw1)
  {
    if (state == trns)
    {
      pswd = pswd*10 + 1;
    }
    if (TimerCamb == 0)
    {
      TimerCamb = millis();
    }
    else if (millis() > TimerCamb + 5000)
    {
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("ALTERADO PARA");
      lcd.setCursor(6, 2);
      if (WPos <= 1)
        lcd.print("TRASEIRO");
      else
        lcd.print("DIANTEIRO");

      if (WPos == fDir)
        WPos = tEsq;
      else if (WPos == fEsq)
        WPos = tDir;
      else if (WPos == tEsq)
        WPos = fDir;
      else if (WPos == tDir)
        WPos = fEsq;
      delay(1000);
    }
    refreshDisplay = HIGH;
    if (state == inic - 1)
      state = vers;
    else if (state < (inic - 1) && state > trns)
      state++;
    /*
    if(state == higb)
      state = lowb;
    else if(state == lowb)
      state = higb;
    */
    if (state >= kpi1)
      state = camb;
    lcd.clear();
    lcd.setCursor(7, 1);
    lcd.print("CAMBER");
    delay(500);
    lcd.clear();
  }
  //B2--------------------------------------
  if (swA == sw2)
  {
    if (state == trns)
    {
      pswd = pswd*10 + 2;
    }
    refreshDisplay = HIGH;
    lock = !lock;
    lcd.clear();
    lcd.setCursor(7, 1);
    lcd.print("SALVAR");

    if(state == parb)
    {
      bvParam = abVoltage;
      EEPROM.put(bpvAdrs, bvParam);
    }
    else if (state == text)
    {
      if (exibTxt < TXT3)
        exibTxt++;
      else if (exibTxt == TXT3)
        exibTxt = TXT1;
      else
        exibTxt = TXT1;

      EEPROM.put(txtAdrs, exibTxt);
    }
    else if(state == ebat)
    {
      if(bTipo == bHide)
        bTipo = bShow;
      else
        bTipo = bHide;
        
      EEPROM.put(sbsAdrs, bTipo);
    }
    else if (state == splr)
    {
      if (spSts == spOn)
        spSts = spOff;
      else
        spSts = spOn;
      
      EEPROM.put(splAdrs, spSts);
    }
    else if (state == skbd)
    {
      if (kboard == PDR1)
        kboard = PDR2;
      else
        kboard = PDR1;

      EEPROM.put(kbdAdrs, kboard);
    }
    if (state == sets)
    {
      if (sense == 1)
        sense = 5;
      else if (sense == 5)
        sense = 10;
      else if (sense == 10)
        sense = 1;
      else
        sense = 5;
        
      EEPROM.put(senAdrs, sense);
    }
    if (state == sdis)
    {
      if (chrd == 2)
        chrd = 0;
      else if (chrd == 0)
        chrd = 2;
      else
        chrd = 0;
      EEPROM.put(chdAdrs, chrd);
    }
    if (state == setk)
    {
      if (kpis == kpiOn)
        kpis = kpiOff;
      else if (kpis == kpiOff)
        kpis = kpiOn;
      else
        kpis = kpiOff;
      EEPROM.put(kpiAdrs, kpis);
    }
    if (state == angs)
    {
      if (TurnAngle == 10)
        TurnAngle = 20;
      else if (TurnAngle == 20)
        TurnAngle = 10;
      else
        TurnAngle = 20;
      EEPROM.put(trnAdrs, TurnAngle);
    }
    if (state == rods)
    {
      if (WPos == fDir)
        WPos = fEsq;
      else if (WPos == fEsq)
        WPos = tDir;
      else if (WPos == tDir)
        WPos = tEsq;
      else if (WPos == tEsq)
        WPos = fDir;
      else
        WPos = fDir;
      EEPROM.put(whpAdrs, WPos);
    }
    if (state == timl)
    {
      if (timerLaser == 0)
        timerLaser = 5;
      else if (timerLaser == 5)
        timerLaser = 10;
      else if (timerLaser == 10)
        timerLaser = 20;
      else if (timerLaser == 20)
        timerLaser = 0;
      else
        timerLaser = 0;
      EEPROM.put(ltmAdrs, timerLaser);
    }
    if (state == timd)
    {
      if (timerDisplay == 0)
        timerDisplay = 2;
      else if (timerDisplay == 2)
        timerDisplay = 5;
      else if (timerDisplay == 5)
        timerDisplay = 10;
      else if (timerDisplay == 10)
        timerDisplay = 0;
      else
        timerDisplay = 0;
      EEPROM.put(dtmAdrs, timerDisplay);
    }
    if (state == chan)
    {
      if (chnl < 4)
        chnl++;
      else
        chnl = 1;
      EEPROM.put(cnlAdrs, chnl);
    }
    if (state == inic)
    {
      SAcX = FAcX;
      SAcY = FAcY;
      SAcZ = FAcZ;
      EEPROM.put(AcXAdrs, SAcX);
      EEPROM.put(AcYAdrs, SAcY);
      EEPROM.put(AcZAdrs, SAcZ);
    }
    if (state == camb)
    {
      if (lcamber)
        lcamber = 0;
      else
        lcamber = camber;
    }
    if (state == kpi1)
    { /*
        if(lcaster)
         lcaster = 0;
        else
         lcaster = caster;
        if(lkpi)
         lkpi = 0;
        else
         lkpi = kpi;*/
      state = kpi2;
    }
    if (state == kpi3)
    {
      kpi = (arm - armZero) / (2 * TurnAngle);
      kpi = kpi * (180 / M_PI);

      caster = (camber - camberZero) / (2 * TurnAngle);
      caster = caster * (180 / M_PI);

      lcaster = caster;
      if (kpis == kpiOn)
        lkpi = kpi;

      state = kpi4;
    }
    delay(500);
    lcd.clear();
  }
  //B3--------------------------------------
  if (swA == sw3)
  {
    refreshDisplay = HIGH;
    if (state == trns)
    {
      pswd = pswd*10 + 3;
    }
    if (state == text)
      state = trns;
    if (state == camb && WPos < 2)
      state = kpi1;
    if (state > kpi1)
      state = kpi1;
    lcd.clear();
    lcd.setCursor(7, 1);
    lcd.print("CASTER");
    delay(500);
    lcd.clear();
  }
  //B4--------------------------------------
  if (swA == sw4)
  {
    lcd.clear();
    lcd.setCursor(7, 1);
    lcd.print("ZERAR");
    if (state == trns)
    {
      pswd = pswd*10 + 4;
    }
    if (state == camb)
    {
      next = HIGH;
    }
    else if (state == inic)
    {
      SAcX = 0xFFFF;
      SAcY = 0xFFFF;
      SAcZ = 0XFFFF;
    }
    else if (state == vers)
    {
      state = text;
    }
    else if (state == kpi2)
    {
      armZero = arm;
      camberZero = camber;
      state = kpi3;
    }
    delay(500);
    lcd.clear();
  }
  //B5--------------------------------------
  if (swA == sw5)
  {
    refreshDisplay = HIGH;
    lcd.clear();
    if (state < inic)
    {
      state = camb;
      lcd.setCursor(3, 1);
      lcd.print("SAINDO DO MENU");
    }
    else if (state == inic)
    {
      state = camb;
      lcd.setCursor(2, 1);
      lcd.print("SAINDO DE AJUSTE");
    }
    else if (!laserStat1)
    {
      laserStat1 = HIGH;
      laserStat2 = HIGH;
      lcd.setCursor(4, 1);
      lcd.print("LASER LIGADO");
      digitalWrite(PIN_LASER_1, HIGH);
      digitalWrite(PIN_LASER_2, HIGH);
      timeOn = millis();
    }
    else
    {
      laserStat1 = LOW;
      laserStat2 = LOW;
      lcd.setCursor(2, 1);
      lcd.print("LASER DESLIGADO");
      digitalWrite(PIN_LASER_1, LOW);
      digitalWrite(PIN_LASER_2, LOW);
    }
    delay(1000);
    lcd.clear();
  }
  //B6--------------------------------------
  /*
    if(swA == sw6)
    {
    lcd.setCursor(8,1);
    lcd.print("B6");
    delay(500);
    lcd.clear();
    }*/

  //------------------------------------------------------------

  //MAIN
  if (refreshDisplay)
  {
    //lcd.clear();
    
    if(state == parb)
    {
      lcd.setCursor(2,1);
      lcd.print("BAT. PARAMETRO 8V:");
      lcd.setCursor(4,2);
      lcd.print(bvParam);
    }
    if (state == trns)
    {
      lcd.clear();
      lcd.setCursor(4,1);
      //lcd.print(pswd);

      if(pswd == 4422)
      {
        state = parb;
        pswd = 0;
        lcd.clear();
      }
      else if(pswd > 1000)
      {
        state = camb;
        pswd = 0;
        lcd.clear();
      }
      
    }
    if (state == text)
    {
      lcd.setCursor(4, 1);
      lcd.print("INICIAL");
      
      lcd.setCursor(4, 2);
      if (exibTxt == TXT1)
        lcd.print("TEXTO 1");
      else if (exibTxt == TXT2)
        lcd.print("TEXTO 2");
      else if(exibTxt == TXT3)
        lcd.print("TEXTO 1 E 2");
      else
        lcd.print("TEXTO 1");
    }
    else if (state == vers)
    {
      lcd.setCursor(2, 1);
      lcd.print("PROGRAMACAO SLX");
      lcd.setCursor(2, 2);
      lcd.print("VERSAO 2.04");
      lcd.setCursor(3,3);
      lcd.print(txtSerial[0]);
      lcd.print(txtSerial[1]);
      lcd.print(txtSerial[2]);
      lcd.print(txtSerial[3]);
      lcd.print(txtSerial[4]);
    }
    else if (state == skbd)
    {
      lcd.setCursor(2, 1);
      lcd.print("TECLADO");
      lcd.setCursor(2, 2);
      if (kboard == PDR1)
        lcd.print("PADRAO 1");
      else if (kboard == PDR2)
        lcd.print("PADRAO 2");
    }
    else if (state == sets)
    {
      lcd.setCursor(2, 1);
      lcd.print("SENSIBILIDADE");
      lcd.setCursor(2, 2);
      lcd.print(sense);
      lcd.print("'");
    }
    else if (state == setk)
    {
      lcd.setCursor(2, 1);
      lcd.print("CALCULO DE KPI");
      lcd.setCursor(2, 2);
      if (kpis == kpiOn)
        lcd.print("LIGADO");
      else if (kpis == kpiOff)
        lcd.print("DESLIGADO");
      else
        lcd.print("DEFINIR!");
    }
    else if (state == angs)
    {
      lcd.setCursor(2, 1);
      lcd.print("ANGULO DE GIRO");
      lcd.setCursor(2, 2);
      lcd.print("USAR ");
      lcd.print(TurnAngle);
      lcd.print(" GRAUS");
    }
    else if (state == rods)
    {
      lcd.setCursor(2, 1);
      lcd.print("CONFIGURAR LADO");
      lcd.setCursor(2, 2);
      if (WPos == fDir)
        lcd.print("DIANT PASSAGEIRO");
      else if (WPos == fEsq)
        lcd.print("DIANT MOTORISTA");
      else if (WPos == tDir)
        lcd.print("TRAS PASSAGEIRO");
      else if (WPos == tEsq)
        lcd.print("TRAS MOTORISTA");
      else
        lcd.print("DEFINIR!");
    }
    else if (state == sdis)
    {
      lcd.setCursor(2, 1);
      lcd.print("MODO DO DISPLAY");
      lcd.setCursor(2, 2);
      if (chrd == 0)
        lcd.print("20 CARACTERES");
      else if (chrd == 2)
        lcd.print("20/16 CARACTERES");
      else
        lcd.print("16 CARACTERES");
    }
    else if (state == timl)
    {
      lcd.setCursor(2, 1);
      lcd.print("TEMPO DO LASER");
      lcd.setCursor(2, 2);
      if (timerLaser == 0)
        lcd.print("NAO DESLIGAR");
      else
      {
        lcd.print(timerLaser);
        lcd.print(" MINUTOS");
      }
    }
    else if (state == timd)
    {
      lcd.setCursor(2, 1);
      lcd.print("TEMPO DO DISPLAY");
      lcd.setCursor(2, 2);
      if (timerDisplay == 0)
        lcd.print("NAO DESLIGAR");
      else
      {
        lcd.print(timerDisplay);
        lcd.print(" MINUTOS");
      }
    }

    else if (state == chan)
    {
      lcd.setCursor(2, 1);
      lcd.print("CANAL WIFI");
      lcd.setCursor(2, 2);
      lcd.print("CANAL ");
      lcd.print(chnl);
    }
    else if (state == splr)
    {
      lcd.setCursor(2, 1);
      lcd.print("MODO SPOILER");
      lcd.setCursor(2, 2);
      if (spSts == spOn)
        lcd.print("ATIVADO");
      else if (spSts == spOff)
        lcd.print("DESATIVADO");
      else
        lcd.print("DEFINIR");
    }
    else if (state == ebat)
    {
      lcd.setCursor(2, 1);
      lcd.print("INDICADOR BATERIA");
      lcd.setCursor(2, 2);
      if (bTipo == bShow)
        lcd.print("MOSTRAR");
      else if (bTipo == bHide)
        lcd.print("OCULTAR");
      else
        lcd.print("DEFINIR");
    }
    else if (state == inic)
    {
      lcd.setCursor(2, 0);
      lcd.print("NIVELE A MAQUINA");
      if (SAcX < 20000)
      {
        lcd.setCursor(2, 2);
        lcd.print("ANGULO X= ");
        printGraus(xAngle);
        lcd.setCursor(2, 3);
        lcd.print("ANGULO Y= ");
        printGraus(yAngle);
      }
      else
      {
        lcd.setCursor(2, 2);
        lcd.print("ANGULO X= ");
        lcd.print((int)FAcX);
        lcd.print("  ");
        lcd.setCursor(2, 3);
        lcd.print("ANGULO Y= ");
        lcd.print((int)FAcY);
        lcd.print("  ");
      }
    }
    else if (state == camb)
    {
      lcd.setCursor(7, 1);
      lcd.print("CAMBER:");
      lcd.setCursor(8, 2);
      printGraus(camber);
      if (lcamber)
      {
        lcd.setCursor(7, 3);
        lcd.print("S:");
        printGraus(lcamber);
      }
      level(arm);
    }
    else if (state == kpi1)
    {
      if (arm < 0.5 && arm > -0.5)
      {
        state = kpi2;
        lcd.clear();
      }
      else
      {
        lcd.setCursor(2, 1);
        lcd.print("O BRACO OU SALVE");
        lcd.setCursor(2, 2);
        lcd.print("PARA PROSSEGUIR");
        lcd.setCursor(2, 3);
        lcd.print("DESNIVELADO");
        level(arm);
      }
    }
    else if (state == kpi2)
    {
      lcd.setCursor(2, 1);
      lcd.print("GIRE O PNEU ");
      lcd.print(TurnAngle);
      lcd.write(B11011111);
      lcd.setCursor(2, 2);
      lcd.print("PARA DENTRO");
      lcd.setCursor(2, 3);
      lcd.print("PRESSIONE ZERAR");
    }
    if (state == kpi3)
    {
      lcd.setCursor(2, 0);
      lcd.print("RETORNE A 0");
      lcd.write(B11011111);
      lcd.print(" E");
      lcd.setCursor(2, 1);
      lcd.print("GIRE O PNEU ");
      lcd.print(TurnAngle);
      lcd.write(B11011111);
      lcd.setCursor(2, 2);
      lcd.print("PARA FORA");
      lcd.setCursor(2, 3);
      lcd.print("PRESSIONE SALVAR");
    }
    else if (state == kpi4)
    {

      if (kpis == kpiOff)
      {
        lcd.setCursor(7, 1);
        lcd.print("CASTER:");
        lcd.setCursor(8, 2);
        printGraus(lcaster);
      }

      if (kpis == kpiOn)
      {
        lcd.setCursor(5, 1);
        lcd.print("CTR:");
        lcd.setCursor(10, 1);
        lcd.print("|");
        lcd.setCursor(13, 1);
        lcd.print("KPI:");

        lcd.setCursor(3, 2);
        printGraus(lcaster);
        lcd.setCursor(10, 2);
        lcd.print("|");
        lcd.setCursor(11, 2);
        printGraus(lkpi);
      }

      level(arm);
    }
    refreshDisplay = LOW;
  }
  //------------------------------------------------------------
  //Serial tata transfer
  /*if(Serial.available())
    {
    Serial.read()
    }*/

  //DATA OVER WIFI CONFIG
  /*
     START BYTE
     [0]
     |     levl         |     camber       |      caster      |       kpi        |       conv       |
     | [01][02][03][04] | [05][06][07][08] | [09][10][11][12] | [13][14][15][16] | [17][18][19][20] |
     |      cent        |  TMR ld  |  LS  DS  |  CH  HD  |    SERIAL CODE   |      UNUSED      | sens |  NT  |
     | [21][22][23][24] | [25][26] | [27][28] | [29][30] | [31][32][33][34] | [35][36][37][38] | [39] | [40] |
     END BYTE
     [41]
  */

  levl.value = arm;
  cmbr.value = camber;
  if (lcamber != 0)
    cmbr.value = lcamber;
  cstr.value = lcaster;
  king.value = lkpi;
  //conv.value = converge;
  //cent.value = central;

  Data[0] = 0;

  Data[1] = levl.Byte[0];
  Data[2] = levl.Byte[1];
  Data[3] = levl.Byte[2];
  Data[4] = levl.Byte[3];

  Data[5] = cmbr.Byte[0];
  Data[6] = cmbr.Byte[1];
  Data[7] = cmbr.Byte[2];
  Data[8] = cmbr.Byte[3];

  Data[9] = cstr.Byte[0];
  Data[10] = cstr.Byte[1];
  Data[11] = cstr.Byte[2];
  Data[12] = cstr.Byte[3];

  Data[13] = king.Byte[0];
  Data[14] = king.Byte[1];
  Data[15] = king.Byte[2];
  Data[16] = king.Byte[3];

  Data[17] = conv.Byte[0];
  Data[18] = conv.Byte[1];
  Data[19] = conv.Byte[2];
  Data[20] = conv.Byte[3];

  Data[21] = cent.Byte[0];
  Data[22] = cent.Byte[1];
  Data[23] = cent.Byte[2];
  Data[24] = cent.Byte[3];

  //TIMER
  Data[25] = timerLaser;
  Data[26] = timerDisplay;

  //LASER E DISPLAY STAT
  Data[27] = laserStat1;
  Data[28] = displayStat;

  //CHANNEL AND HEAD
  Data[29] = chnl;
  Data[30] = WPos;

  Data[39] = sense;

  Data[40] = next;

  Data[41] = '\n';


  if (Serial.available())
  {
    serialData = Serial.read();
    if (serialData == '0')
    {
      Serial.write(Data, DataSize);
      Serial.flush();
      if (next)
      {
        next = !next;
      }
    }
    else if (serialData == 'A')
    {
      if (WPos == fDir)
        WPos = tEsq;
      else if (WPos == fEsq)
        WPos = tDir;
      else if (WPos == tEsq)
        WPos = fDir;
      else if (WPos == tDir)
        WPos = fEsq;

      Serial.write(Data, DataSize);
      Serial.flush();
      if (next)
      {
        next = !next;
      }
    }
  }

  //------------------------------------------------------------
  //Battery Status

  if(analogRead(PIN_BVOLTAGE) < 280)
  {
    carregando = true;
  }
  else
  {
    if(carregando)
      abVoltage = 800;
      
    carregando = false;
    bVoltage += analogRead(PIN_BVOLTAGE);
    bCntr++;
    
    if(bCntr == 40)
    {
      bCntr = 0;
      abVoltage = bVoltage/40;
      bVoltage = 0;
    }
    vAtual = (abVoltage*vConst)/(bvParam);
  }

  if (state == camb || state == kpi4)
  {
    lcd.setCursor(chrd, 3);
    //CHARGING!!
    if (carregando)
    {
      //lcd.setCursor(19-chrd, 0);
      lcd.write(byte(bcar));
      lcd.print("     ");
    }
    else if (!carregando) //NOT CHARGING
    {
      if(vAtual < 5.2)
      {
        lcd.clear();
        lcd.setCursor(19 - chrd, 0);
        lcd.write(byte(0));
        lcd.setCursor(3, 1);
        lcd.print("BATERIA BAIXA");
        lcd.setCursor(4, 2);
        lcd.print("RECARREGAR");
        lcd.setCursor(8, 3);
        lcd.print("!!!");
        
        laserStat1 = LOW;
        laserStat2 = LOW;
        digitalWrite(PIN_LASER_1, laserStat1);
        digitalWrite(PIN_LASER_2, laserStat2);
        
        displayStat = LOW;
        digitalWrite(PIN_BACKLIGHT, displayStat);
        while(true)
        {
          if (Serial.available())
          {
            serialData = Serial.read();
            if (serialData == 'T')
            {
              Serial.write('Q');
              Serial.flush();
              while(!Serial.available());
              Serial.readBytes(buf2, 2);
              Serial.flush();
              if ( buf2[0] == 'T' && buf2[1] == '1')
              {
                Serial.write('R');
                Serial.flush();
                while(!Serial.available());
                Serial.readBytes(txtInic, 80);
                Serial.flush();
                Serial.write('D');
                EEPROM.put(tx1Adrs, txtInic);
              }
              else if ( buf2[0] == 'T' && buf2[1] == '2')
              {
                Serial.write('R');
                Serial.flush();
                while(!Serial.available());
                Serial.readBytes(txtInic, 80);
                Serial.flush();
                Serial.write('D');
                EEPROM.put(tx2Adrs, txtInic);
              }
              else if ( buf2[0] == 'T' && buf2[1] == 'S')
              {
                Serial.write('R');
                Serial.flush();
                while(!Serial.available());
                Serial.readBytes(txtSerial, 5);
                Serial.flush();
                Serial.write('D');
                EEPROM.put(txsAdrs, txtSerial);
              }
            }
          }
        }
      }
      else if(bTipo == bShow)
      {
        if (vAtual < 6.5)
        {
          lcd.write(byte(b00));
          if ((vAtual > 6.2 && vAtual <= 6.3) && (millis() > timeBateria + minute))
          {
            timeBateria = millis();
            lcd.clear();
            lcd.setCursor(19 - chrd, 0);
            lcd.write(byte(b00));
            lcd.setCursor(3, 1);
            lcd.print("BATERIA BAIXA");
            lcd.setCursor(4, 2);
            lcd.print("RECARREGAR");
            delay(5000);
            lcd.clear();
          }
          else if(vAtual <= 6.2)
          {
            
            lcd.clear();
            lcd.setCursor(19 - chrd, 0);
            lcd.write(byte(b00));
            lcd.setCursor(3, 1);
            lcd.print("BATERIA BAIXA");
            lcd.setCursor(4, 2);
            lcd.print("RECARREGAR");
            
            laserStat1 = LOW;
            laserStat2 = LOW;
            digitalWrite(PIN_LASER_1, laserStat1);
            digitalWrite(PIN_LASER_2, laserStat2);
            delay(2000);
            //Sleep!
            
            displayStat = LOW;
            digitalWrite(PIN_BACKLIGHT, displayStat);
          }
        }
        else if (vAtual >= 6.5 && vAtual < 7.0){
          lcd.write(byte(b25));
        }
        else if (vAtual >= 7.0 && vAtual < 7.5){
          lcd.write(byte(b50));
        }
        else if (vAtual >= 7.5 && vAtual < 8.0){
          lcd.write(byte(b75));
        }
        else if (vAtual >= 8.0) {
          lcd.write(byte(b100));
        }
        else
          lcd.print(" ");
        
        pBat = int( ((vAtual-6.2)/1.8)*100 );
        if(pBat > 100)
          pBat = 100;
        if(pBat < 0)
          pBat = 0;
        pBat = int(pBat/sense)*sense;
        lcd.print(pBat);
        lcd.print("%  ");
      }
      else if(bTipo == bHide)
      {
        if (vAtual <= 6.2)
        {
          lcd.write(byte(b00));
          if ((vAtual > 6.2 && vAtual <= 6.3) && (millis() > timeBateria + 2*minute))
          {
            timeBateria = millis();
            lcd.clear();
            lcd.setCursor(19 - chrd, 0);
            lcd.write(byte(b00));
            lcd.setCursor(3, 1);
            lcd.print("BATERIA BAIXA");
            lcd.setCursor(4, 2);
            lcd.print("RECARREGAR");
            delay(5000);
            lcd.clear();
          }
        }
        else if (vAtual >= 8.3) {
          lcd.write(byte(b100));
        }
        else
          lcd.print(" ");
      }
    }



    lcd.setCursor(18 - chrd, 3);
    if (WPos == fDir)
      lcd.print("DP");
    else if (WPos == fEsq)
      lcd.print("DM");
    else if (WPos == tDir)
      lcd.print("TP");
    else if (WPos == tEsq)
      lcd.print("TM");
    else
      lcd.print("--");
  }
  

}
