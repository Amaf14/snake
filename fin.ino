#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 7, 8);  //pinii ecranului

#define SUS 10
#define JOS 11
#define STANGA 9
#define DREAPTA 12 //pinii tastelor

#define MAX_WIDTH 84
#define MAX_HEIGHT 48 //dimensiunea ecranului in pixeli

#define lumina 2
#define marime_sarpe 1

//$$$$$$debug mode
bool debug=0;
//$$$$$$debug mode

boolean dstanga = false, ddreapta = false, dsus = false, djos = false; // directia in care se misca sarpele

int x[50], y[50], tempx = 10, tempy = 10, xx, yy;
int i, lung_sarpe;

int xfruct, yfruct; //coordonatele fructului

int stanga, dreapta, sus, jos;
int Scor = 0, flag = 0;
int time = 280;
boolean sfarsit = false;


int pozitie = -1, a;
bool slumina = false, rama = true;


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
  //f(x)=15x+5
  display.fillRect(8, 15 * a + 5, 4, 7, BLACK);
  //display.display();
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
  lung_sarpe = 18;               //Start with snake length 8
  xfruct = (display.width()) / 2;
  yfruct = (display.height()) / 2;
  for (i = 0; i <= lung_sarpe; i++) //Set starting coordinates of snake
  {
    x[i] = 25 - 3 * i;
    y[i] = 10;
  }
  for (i = 0; i < lung_sarpe; i++)  //Draw the snake
    display.drawCircle(x[i], y[i], marime_sarpe, BLACK);
  ddreapta = true;  //Going to move right initially
}

void initial2(){
  lung_sarpe = 18;               //Start with snake length 8
  xfruct = (display.width()) / 2;
  yfruct = (display.height()) / 2;
  for (i = 0; i <= lung_sarpe; i++) //Set starting coordinates of snake
  {
    x[i] = 25 - 3 * i;
    y[i] = 10;
  }
  ddreapta = true;
}



void miscare_sarpe()
{
  while (sfarsit != true) {
    stanga = digitalRead(STANGA);    //Continuously Check the Keys
    jos = digitalRead(JOS);
    dreapta = digitalRead(DREAPTA);
    sus = digitalRead(SUS);


    if (flag == 0)
      directie();    //When key is pressed,this will change the coordinates accordingly and set flag to 1
    //flag would be set to 1 so that direction is not changed multiple times in a short duration
    //this condition becomes true after every 'time' milliseconds...millis() returns the time since launch of program
    if (millis() % time == 0) {
      if (flag == 0) {                               //flag 0 means no directional key has been pressed in the last 'time' milliseconds
        if (ddreapta == true) {
          tempx = x[0] + 3;  // so the snake moves one step in the direction it is moving currently
          tempy = y[0];
        }
        if (dstanga == true) {
          tempx = x[0] - 3;  //The new coordinates of head of snake goes in tempx,tempy
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
      verificare_joc();                              //Check if snake has met egg or coincided with itself
      verifica_fruct();
      depasit();
      for (i = 0; i <= lung_sarpe; i++) { //Change the coordinates of all points of snake
        xx = x[i];
        yy = y[i];
        x[i] = tempx;
        y[i] = tempy;
        tempx = xx;
        tempy = yy;
      }
      deseneaza_sarpe();           //Draw the snake and egg at the new coordinates
    }
  }
}


void verificare_joc()       //Game over checker
{
  for (i = 1; i < lung_sarpe; i++)        //Checking if the coordinates of head have become equal to one of the non head points of snake
    if (x[i] == x[0] && y[i] == y[0])
    {
      sfarsit_joc();
      reseteaza_joc();               //Restart game by drawing snake with the resetted length and score
    }
}

void verifica_fruct()      //Snake meets egg
{
  if (x[0] == xfruct or x[0] == (xfruct + 1) or x[0] == (xfruct + 2) or x[0] == (xfruct - 1)) //Snake in close vicinity of egg
  {
    if (y[0] == yfruct or y[0] == (yfruct + 1) or y[0] == (yfruct + 2) or y[0] == (yfruct - 1))
    {
      Scor += 1;                     //Increase length,score and increase movement speed by decreasing 'time'
      lung_sarpe += 1;
      if (time >= 90)
      {
        time -= 20;
      }

      display.fillRect(xfruct, yfruct, 3, 3, WHITE);  //Delete the consumed egg

      display.display();

      xfruct = random(1, 80);           //Create New egg randomly
      yfruct = random(1, 40);
    }
  }
}


void directie()                  //Check if user pressed any keys and change direction if so
{
  if (stanga == LOW and ddreapta == false)  //when key LEFT is pressed ,L will become low
  {
    dstanga = true; dsus = false; djos = false;
    tempx = x[0] - 3;            //Save the new coordinates of head in tempx,tempy
    tempy = y[0];
    flag = 1;                   //Do not change direction any further for the ongoing 'time' milliseconds
  }
  else if (dreapta == LOW and dstanga == false)
  {
    ddreapta = true; dsus = false; djos = false;
    tempx = x[0] + 3;
    tempy = y[0];
    flag = 1;
  }
  else if (sus == LOW and djos == false)
  {
    dsus = true; dstanga = false; ddreapta = false;
    tempy = y[0] - 3;
    tempx = x[0];
    flag = 1;
  }
  else if (jos == LOW and dsus == false)
  {
    djos = true; dstanga = false; ddreapta = false;
    tempy = y[0] + 3;
    tempx = x[0];
    flag = 1;
  }
}





void deseneaza_sarpe()        //Draw snake and egg at newly changed positions
{
  display.fillRect(xfruct, yfruct, 3, 3, BLACK);   //Draw egg at new pos
  display.drawCircle(x[0], y[0], marime_sarpe, BLACK);    //Draw new head of snake
  display.drawCircle(x[lung_sarpe], y[lung_sarpe], marime_sarpe, WHITE); //Delete old tail of snake
  display.display();
}





void redeseneaza()   //Redraw ALL POINTS of snake and egg
{
  display.fillRect(xfruct, yfruct, 3, 3, BLACK);
  for (i = 0; i < lung_sarpe; i++)
    display.drawCircle(x[i], y[i], marime_sarpe, BLACK);
  display.display();
}


void sfarsit_joc() {
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(20, 12);
  display.print( F("Game Over"));
  display.setCursor(15, 30);
  display.print( F("Scor: "));
  display.print(Scor);
  display.display();
  sfarsit = true;
  pozitie=-1;
  reseteaza_joc();
  delay(1000);
}

void reseteaza_joc() {
  display.clearDisplay();
  lung_sarpe = 8;
  Scor = 0;
  time = 280;
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
  Serial.begin(19200);
  
  pinMode(3, OUTPUT);
  pinMode(lumina, OUTPUT);
  
  digitalWrite(3, HIGH);
  digitalWrite(lumina, slumina);
  
  display.begin();
  display.clearDisplay();
  display.setContrast(50);

  pinMode(SUS, INPUT);
  pinMode(JOS, INPUT);
  pinMode(DREAPTA, INPUT);
  pinMode(STANGA, INPUT);
  
  digitalWrite(SUS, HIGH);
  digitalWrite(JOS, HIGH);
  digitalWrite(DREAPTA, HIGH);
  digitalWrite(STANGA, HIGH);

  //$$$$$$debug mode
  if(debug==false){
    logo0();
    delay(500);
    logo1();
    delay(500);
    logo2();
    delay(500);
  }
  //$$$$$$debug mode
  
  initial();

}



void loop() {

  sfarsit=false;
  initial2();
  menu(a);
  if (digitalRead(SUS) == LOW && a != 0)
    a--;
  if (digitalRead(JOS) == LOW && a != 2)
    a++;
  if (digitalRead(DREAPTA) == LOW)
    pozitie = a;

  switch (pozitie) {

    case 0:
      while (pozitie == 0) {
        display.clearDisplay();
        miscare_sarpe();
      }
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
          if (a == 2) {
            //Scor=0;
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(BLACK);
            display.setCursor(15, 5);
            display.print(F("0000"));
            display.display();
          }
        }
        if (digitalRead(STANGA) == LOW && pozitie != 0)
          pozitie = -1;
        delay(50);
      }
      break;

    case 2:
      while (pozitie = 2 && digitalRead(STANGA) != LOW) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(15, 5);
        display.print(F("1337"));
        display.display();
        delay(20);
      }
      break;
  }
  delay(50);
}
