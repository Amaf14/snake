#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 7, 8);  //pinii ecranului

#define SUS 10
#define JOS 11
#define STANGA 9
#define DREAPTA 12 //pinii tastelor

#define FLASH 13 //pinul ledului

#define MAX_WIDTH 84
#define MAX_HEIGHT 48 //dimensiunea ecranului in pixeli

#define lumina 2
#define marime_sarpe 1


boolean dstanga = false, ddreapta = false, dsus = false, djos = false; // directia in care se misca sarpele
int stanga, dreapta, sus, jos;

int x[50], y[50], tempx = 10, tempy = 10, xx, yy;
int i, lung_sarpe;

int xfruct, yfruct; //coordonatele fructului

int Scor = 0, flag = 0;
int timp = 280;
boolean sfarsit = false;

int pozitie = -1, a, b;
bool slumina = true, rama = true;


void flash() {
  for(int p=0;p<=2;p++) {
    puls();
    delay(50);
  }
}

void puls() {
  digitalWrite(FLASH, HIGH);
  delay(50);
  digitalWrite(FLASH, LOW);
}

//#################################################
void whscore(int a){
  if(EEPROM.read(0) < a)
    EEPROM.write(0, a);
}

int rhscore(){
return EEPROM.read(0);
}

void rshscore(){
EEPROM.write(0, 0);
}

//#################################################
void ramaf() {
  display.setCursor(55, 20);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  if (rama == true)
    display.print( F("ON"));
  if (rama == false)
    display.print( F("OFF"));
  display.display();
}

void selectare(int a) {
  //display.fillRect(8, 15 * a + 5, 4, 7, BLACK);
  display.drawTriangle(5, 15*a+3, 5, 15*a+13, 10, 15*a+8, BLACK);
}


void menu(int a) {
  display.clearDisplay();
  selectare(a);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(15, 5);
  display.print( F("Joc"));
  display.setCursor(15, 20);
  display.print( F("Setari"));
  display.setCursor(15, 35);
  display.print( F("Scor"));
  display.display();
}

void setari() {
  display.clearDisplay();
  selectare(a);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(15, 5);
  display.print( F("Backlight"));
  display.setCursor(15, 20);
  display.print( F("Rama"));
  display.setCursor(15, 35);
  display.print( F("Reset Scor"));
}


//################################################################

void ramaf2() {
  int i;
  display.clearDisplay();
  for(i=4;i<=46;i+=3){
    display.drawCircle(1,i,3,BLACK);
    display.drawCircle(82,i,3,BLACK);
  }
  for(i=4;i<=82;i+=3){
    display.drawCircle(i,1,3,BLACK);
    display.drawCircle(i,46,3,BLACK);
  }
}

void logo0() {
  int i, j;
  display.clearDisplay();
  for (i = 4; i <= 46; i += 4)
    for (j = 4; j <= 82; j += 4) 
      display.drawCircle(j, i, 3, BLACK);
  display.display();
}

void logo1() {
  ramaf2();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(13, 10);
  display.print(F("SNAKE"));
  display.setCursor(42, 26);
  display.print(F("AVR"));
  display.display();
}

void logo2() {
  ramaf2();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(7, 7);
  display.print(F("Realizat de"));
  display.setCursor(7, 16);
  display.print(F("Maftei"));
  display.setCursor(7, 25);
  display.print(F("Alexandru"));
  display.setCursor(7, 34);
  display.print(F("Constantin"));
  display.display();
}


//##############################################

void initial() {
  lung_sarpe = 5; // marimea initiala a sarpelui este de 5 unitati
  xfruct = display.width() / 2;
  yfruct = display.height() / 2;
  for (i = 0; i <= lung_sarpe; i++) {
    x[i] = 25 - 3 * i;
    y[i] = 10;
  }
  for (i = 0; i < lung_sarpe; i++)
    display.drawCircle(x[i], y[i], marime_sarpe, BLACK);
  ddreapta = true;
}

void initial2() {
  lung_sarpe = 5;
  xfruct = display.width() / 2;
  yfruct = display.height() / 2;
  for (i = 0; i <= lung_sarpe; i++) {
    x[i] = 25 - 3 * i;
    y[i] = 10;
  }
  ddreapta = true;
}


void miscare_sarpe() {
  while (sfarsit != true) {
    stanga = digitalRead(STANGA); //verificarea directiei
    jos = digitalRead(JOS);
    dreapta = digitalRead(DREAPTA);
    sus = digitalRead(SUS);

    if (flag == 0)
      directie();
    if (millis() % timp == 0) {
      if (flag == 0) {
        if (ddreapta == true) {
          tempx = x[0] + 3;
          tempy = y[0];
        }
        if (dstanga == true) {
          tempx = x[0] - 3;
          tempy = y[0];
        }
        if (dsus == true) {
          tempy = y[0] - 3;
          tempx = x[0];
        }
        if (djos == true) {
          tempy = y[0] + 3;
          tempx = x[0];
        }
      }
      flag = 0;
      verificare_joc();
      verificare_fruct();
      depasit();
      for (i = 0; i <= lung_sarpe; i++) { //schimbarea coordonatelor
        xx = x[i];
        yy = y[i];
        x[i] = tempx;
        y[i] = tempy;
        tempx = xx;
        tempy = yy;
      }
      deseneaza_sarpe();
    }
  }
}

void verificare_joc() {
  for (i = 1; i < lung_sarpe; i++)
    if (x[i] == x[0] && y[i] == y[0]){
      sfarsit_joc();
      reseteaza_joc();
    }
}

void verificare_fruct() {
  if (x[0] == xfruct or x[0] == (xfruct + 1) or x[0] == (xfruct + 2) or x[0] == (xfruct - 1)){
    if (y[0] == yfruct or y[0] == (yfruct + 1) or y[0] == (yfruct + 2) or y[0] == (yfruct - 1)){
      Scor += 1; //se mareste scorul si viteza sarpelui
      lung_sarpe += 1;
      puls();
      if (timp >= 90)
        timp -= 20;

      //display.fillRect(xfruct, yfruct, 3, 3, WHITE); // stergerea fructului
      display.fillCircle(xfruct, yfruct, 3, WHITE);
      display.display();

      xfruct = random(2, 82); // crearea unui fruct nou
      yfruct = random(2, 46);
    }
  }
}


void directie() {
  if (stanga == LOW and ddreapta == false){
    dstanga = true; dsus = false; djos = false;
    tempx = x[0] - 3;
    tempy = y[0];
    flag = 1;
  }
  else if (dreapta == LOW and dstanga == false){
    ddreapta = true; dsus = false; djos = false;
    tempx = x[0] + 3;
    tempy = y[0];
    flag = 1;
  }
  else if (sus == LOW and djos == false){
    dsus = true; dstanga = false; ddreapta = false;
    tempy = y[0] - 3;
    tempx = x[0];
    flag = 1;
  }
  else if (jos == LOW and dsus == false){
    djos = true; dstanga = false; ddreapta = false;
    tempy = y[0] + 3;
    tempx = x[0];
    flag = 1;
  }
}





void deseneaza_sarpe() {
  display.fillCircle(xfruct, yfruct, 2, BLACK);
  display.drawCircle(x[0], y[0], marime_sarpe, BLACK);
  display.drawCircle(x[lung_sarpe], y[lung_sarpe], marime_sarpe, WHITE);
  display.display();
}

void sfarsit_joc() {
  flash();
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(20, 12);
  display.print( F("Game Over"));
  display.setCursor(20, 30);
  display.print( F("Scor: "));
  display.print(Scor);
  display.display();
  whscore(Scor);
  sfarsit = true;
  pozitie=-1;
  reseteaza_joc();
  delay(1500);
}

void reseteaza_joc() {
  display.clearDisplay();
  lung_sarpe = 5;
  Scor = 0;
  timp = 280;
}


void depasit() {
  if (tempx <= 0 || tempx >= MAX_WIDTH || tempy <= 0 || tempy >= MAX_HEIGHT)
    if (rama == true) {
      sfarsit_joc();
      reseteaza_joc();
    }
    else {
      if (tempx <= 0)
        tempx = MAX_WIDTH + tempx;
      if (tempx >= MAX_WIDTH)
        tempx = tempx - MAX_WIDTH;
      if (tempy <= 0)
        tempy = MAX_HEIGHT + tempy;
      if (tempy >= MAX_HEIGHT)
        tempy = tempy - MAX_HEIGHT;
    }
}





//####################################################

void setup() {
  pinMode(3, OUTPUT);
  pinMode(lumina, OUTPUT);
  pinMode(FLASH, OUTPUT);
  
  digitalWrite(3, HIGH);
  digitalWrite(FLASH, LOW);
  digitalWrite(lumina, slumina);
  
  display.begin();
  display.clearDisplay();
  display.setContrast(48);

  pinMode(SUS, INPUT);
  pinMode(JOS, INPUT);
  pinMode(DREAPTA, INPUT);
  pinMode(STANGA, INPUT);
  
  digitalWrite(SUS, HIGH);
  digitalWrite(JOS, HIGH);
  digitalWrite(DREAPTA, HIGH);
  digitalWrite(STANGA, HIGH);
  
  logo0();
  delay(500);
  logo1();
  delay(500);
  logo2();
  delay(500);
  
  initial();
}


void loop() {
  
  menu(b);
  if (digitalRead(SUS) == LOW && b != 0)
    b--;
  if (digitalRead(JOS) == LOW && b != 2)
    b++;
  if (digitalRead(DREAPTA) == LOW)
    pozitie=b;

  switch (pozitie) {

    case 0:
      while (pozitie == 0) {
        display.clearDisplay();
        miscare_sarpe();
      }
      sfarsit=false;
      initial2();
      break;

    case 1:
      a = 0;
      while (pozitie == 1) {
        setari();
        ramaf();
        if (digitalRead(SUS) == LOW && a != 0)
          a--;
        if (digitalRead(JOS) == LOW && a != 2)
          a++;
        if (digitalRead(DREAPTA) == LOW) {
          delay(10);
          if (a == 0) {
            if ( slumina == true )
              slumina = false;
            else
              slumina = true;
            digitalWrite(2, slumina);
            delay(100);
          }
          if (a == 1)
            if ( rama == true )
              rama = false;
            else
              rama = true;
          if (a == 2)
            rshscore();
        }
        if (digitalRead(STANGA) == LOW && pozitie != 0)
          pozitie = -1;
        delay(50);
      }
      break;

      case 2:  
      while (pozitie = 2 && digitalRead(STANGA) != LOW) {  
        display.clearDisplay();  
        display.setTextSize(3);  
        display.setTextColor(BLACK);  
        display.setCursor(10, 15);  
        display.print(rhscore());  
        display.display();  
      }  
      pozitie=-1;
      break;  
  }
  delay(50);
}
