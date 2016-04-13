#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 7, 8);  //Initialise display object

#define UP 10
#define DOWN 11
#define LEFT 8
#define RIGHT 15
#define ENTER 9
#define BACK 12
#define MAX_WIDTH 84        //display 84x48
#define MAX_HEIGHT 39

boolean dl = false, dr = false, du = false, dd = false; // to check in which direction the snake is currently moving

int x[50], y[50], i, slength, tempx = 10, tempy = 10, xx, yy;
unsigned int high;

int xegg, yegg;
int freq, tb;
int l, r, u, d, p;
int score = 0, flag = 0;
int time = 280;
byte marime_sarpe = 1;
boolean end = false;


int p1 = -1, a;
bool light = true, border = true;


//#################################################
void borderf() {
  display.setCursor(55, 20);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  if (border == true)
    display.print( F("ON"));
  if (border == false)
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
  display.print( F("Play"));
  display.setCursor(15, 20);
  display.print( F("Settings"));
  display.setCursor(15, 35);
  display.print( F("Score"));
  display.display();
}

void settings() {
  display.clearDisplay();
  selectare(a);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(15, 5);
  display.print( F("Backlight"));
  display.setCursor(15, 20);
  display.print( F("Border"));
  display.setCursor(15, 35);
  display.print( F("Reset Score"));
}


//################################################################

void rama() {
  int i;
  display.clearDisplay();
  for (i = 4; i <= 46; i += 4) {
    display.drawCircle(1, i, 3, BLACK);
    display.drawCircle(82, i, 3, BLACK);
  }
  for (i = 4; i <= 82; i += 4) {
    display.drawCircle(i, 1, 3, BLACK);
    display.drawCircle(i, 46, 3, BLACK);
  }
}

void logo0(bool d) {
  int i, j;
  display.clearDisplay();
  for (i = 4; i <= 46; i += 4)
    for (j = 4; j <= 82; j += 4) {
      display.drawCircle(j, i, 3, BLACK);
      if (d == true)
        display.display();
    }
  display.display();
}

void logo1() {
  rama();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(13, 10);
  display.print(F("SNAKE"));
  display.setCursor(42, 26);
  display.print(F("AVR"));
  display.display();
}

void logo2() {
  rama();
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
  slength = 18;               //Start with snake length 8
  xegg = (display.width()) / 2;
  yegg = (display.height()) / 2;

  for (i = 0; i <= slength; i++) //Set starting coordinates of snake
  {
    x[i] = 25 - 3 * i;
    y[i] = 10;
  }

  for (i = 0; i < slength; i++)  //Draw the snake
  {
    display.drawCircle(x[i], y[i], marime_sarpe, BLACK);
  }
  display.display();

  dr = true;  //Going to move right initially
}

void movesnake()
{
  while (end != true) {
    l = digitalRead(LEFT);    //Continuously Check the Keys
    d = digitalRead(DOWN);
    r = digitalRead(RIGHT);
    u = digitalRead(UP);
    p = digitalRead(BACK);


    if (flag == 0)
      direct();    //When key is pressed,this will change the coordinates accordingly and set flag to 1
    //flag would be set to 1 so that direction is not changed multiple times in a short duration
    //this condition becomes true after every 'time' milliseconds...millis() returns the time since launch of program
    if (millis() % time == 0) {
      if (flag == 0) {                               //flag 0 means no directional key has been pressed in the last 'time' milliseconds
        if (dr == true) {
          tempx = x[0] + 3;  // so the snake moves one step in the direction it is moving currently
          tempy = y[0];
        }
        if (dl == true) {
          tempx = x[0] - 3;  //The new coordinates of head of snake goes in tempx,tempy
          tempy = y[0];
        }
        if (du == true) {
          tempy = y[0] - 3;
          tempx = x[0];
        }
        if (dd == true) {
          tempy = y[0] + 3;
          tempx = x[0];
        }
      }
      flag = 0;
      checkgame();                              //Check if snake has met egg or coincided with itself
      checkegg();
      depasit();
      for (i = 0; i <= slength; i++) { //Change the coordinates of all points of snake
        xx = x[i];
        yy = y[i];
        x[i] = tempx;
        y[i] = tempy;
        tempx = xx;
        tempy = yy;
      }
      drawsnake();           //Draw the snake and egg at the new coordinates
    }
  }
}


void checkgame()       //Game over checker
{
  for (i = 1; i < slength; i++)        //Checking if the coordinates of head have become equal to one of the non head points of snake
    if (x[i] == x[0] && y[i] == y[0])
    {
      gameover();
      reset_game();               //Restart game by drawing snake with the resetted length and score
    }
}

void checkegg()      //Snake meets egg
{
  if (x[0] == xegg or x[0] == (xegg + 1) or x[0] == (xegg + 2) or x[0] == (xegg - 1)) //Snake in close vicinity of egg
  {
    if (y[0] == yegg or y[0] == (yegg + 1) or y[0] == (yegg + 2) or y[0] == (yegg - 1))
    {
      score += 1;                     //Increase length,score and increase movement speed by decreasing 'time'
      slength += 1;
      if (time >= 90)
      {
        time -= 20;
      }

      display.fillRect(xegg, yegg, 3, 3, WHITE);  //Delete the consumed egg

      display.display();

      xegg = random(1, 80);           //Create New egg randomly
      yegg = random(1, 40);
    }
  }
}


void direct()                  //Check if user pressed any keys and change direction if so
{
  if (l == LOW and dr == false)  //when key LEFT is pressed ,L will become low
  {
    dl = true; du = false; dd = false;
    tempx = x[0] - 3;            //Save the new coordinates of head in tempx,tempy
    tempy = y[0];
    flag = 1;                   //Do not change direction any further for the ongoing 'time' milliseconds
    Serial.println("l");
  }
  else if (r == LOW and dl == false)
  {
    dr = true; du = false; dd = false;
    tempx = x[0] + 3;
    tempy = y[0];
    flag = 1;
    Serial.println("r");
  }
  else if (u == LOW and dd == false)
  {
    du = true; dl = false; dr = false;
    tempy = y[0] - 3;
    tempx = x[0];
    flag = 1;
    Serial.println("u");
  }
  else if (d == LOW and du == false)
  {
    dd = true; dl = false; dr = false;
    tempy = y[0] + 3;
    tempx = x[0];
    flag = 1;
    Serial.println("d");
  }
}





void drawsnake()        //Draw snake and egg at newly changed positions
{
  display.fillRect(xegg, yegg, 3, 3, BLACK);   //Draw egg at new pos
  display.drawCircle(x[0], y[0], marime_sarpe, BLACK);    //Draw new head of snake
  display.drawCircle(x[slength], y[slength], marime_sarpe, WHITE); //Delete old tail of snake
  display.display();
}





void redraw()   //Redraw ALL POINTS of snake and egg
{
  display.fillRect(xegg, yegg, 3, 3, BLACK);
  for (i = 0; i < slength; i++)
    display.drawCircle(x[i], y[i], marime_sarpe, BLACK);
  display.display();
}


void gameover() {
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(20, 12);
  display.print( F("Game Over"));
  display.setCursor(15, 30);
  display.print( F("Score: "));
  display.print(score);
  display.display();
  end = true;
  p1=-1;
  reset_game();
  delay(1000);
}

void reset_game() {
  display.clearDisplay();
  slength = 8;
  score = 0;
  time = 280;
  redraw();
}


void depasit() {
  if (tempx <= 0 || tempx >= MAX_WIDTH || tempy <= 0 || tempy >= MAX_HEIGHT)
    if (border == true) {
      gameover();
      reset_game();
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
  display.begin();
  display.clearDisplay();

  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, light);
  display.setContrast(25);

  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, HIGH);
  pinMode(ENTER, INPUT);
  digitalWrite(ENTER, HIGH);
  pinMode(BACK, INPUT);
  digitalWrite(BACK, HIGH);

  logo0(false);
  delay(500);
  logo1();
  delay(500);
  logo2();
  delay(500);
  initial();

}



void loop() {

  menu(a);
  if (digitalRead(UP) == LOW && a != 0)
    a--;
  if (digitalRead(DOWN) == LOW && a != 2)
    a++;
  if (digitalRead(ENTER) == LOW)
    p1 = a;

  switch (p1) {

    case 0:
      while (p1 == 0) {
        display.clearDisplay();
        //initial();
        movesnake();
      }
      //reset_game();
      break;

    case 1:
      a = 0;
      while (p1 == 1) {
        settings();
        borderf();
        if (digitalRead(UP) == LOW && a != 0)
          a--;
        if (digitalRead(DOWN) == LOW && a != 2)
          a++;
        if (digitalRead(ENTER) == LOW) {
          delay(10);
          if (a == 0) {
            if ( light == true )
              light = false;
            else
              light = true;
            digitalWrite(2, light);
            delay(100);
          }
          if (a == 1)
            if ( border == true )
              border = false;
            else
              border = true;
          if (a == 2) {
            //score=0;
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(BLACK);
            display.setCursor(15, 5);
            display.print(F("0000"));
            display.display();
          }
        }
        if (digitalRead(BACK) == LOW && p1 != 0)
          p1 = -1;
        delay(50);
      }
      break;

    case 2:
      while (p1 = 2 && digitalRead(BACK) != LOW) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(15, 5);
        display.print(F("1337"));
        display.display();
        delay(20);
      }
      Serial.println(p1);
      break;
  }
  delay(50);
}