#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 7, 8);  //Initialise display object

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)


/********* constants *******/
#define LEFT 8
#define DOWN 10
#define RIGHT 11
#define UP 12
#define BACK 14
#define ENTER 9
#define MAX_WIDTH 84        //display 84x48
#define MAX_HEIGHT 48

boolean dl = false, dr = false, du = false, dd = false; // to check in which direction the snake is currently moving
boolean bordura = false, end = false;

int x[100], y[100], i, slength, tempx = 10, tempy = 10, xx, yy;
unsigned int high;

int xegg, yegg;
int freq, tb;
int l, r, u, d, p;
int score = 0, flag = 0;
int time = 280;
byte marime_sarpe = 1;
int q = 1, a = 1;

void setup()
{
  Serial.begin(9600);         //Begin Serial Communication
  display.begin();
  display.clearDisplay();

  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  pinMode(LEFT, INPUT);        //Directional keys and pause
  pinMode(RIGHT, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(BACK, INPUT);

  digitalWrite(LEFT, HIGH);    //Active low keys
  digitalWrite(RIGHT, HIGH);
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, HIGH);
  digitalWrite(BACK, HIGH);


  display.setContrast(25);
  initial();
}


//Movement Begins after here
//############################
void loop()
{
  movesnake();
}
//############################


void initial() {
  slength = 18;               //Start with snake length 8

  xegg = (display.width()) / 2;

  yegg = (display.height()) / 2;

  display.clearDisplay();
  display.setTextSize(2);          //Initial Display
  display.setTextColor(BLACK);
  display.setCursor(15, 10);
  display.print( F("Amaf"));
  display.display();
  delay(1000);
  display.clearDisplay();

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
  }
  else if (r == LOW and dl == false)
  {
    dr = true; du = false; dd = false;
    tempx = x[0] + 3;
    tempy = y[0];
    flag = 1;
  }
  else if (u == LOW and dd == false)
  {
    du = true; dl = false; dr = false;
    tempy = y[0] - 3;
    tempx = x[0];
    flag = 1;
  }
  else if (d == LOW and du == false)
  {
    dd = true; dl = false; dr = false;
    tempy = y[0] + 3;
    tempx = x[0];
    flag = 1;
  }
  else if (p == LOW)            //Pause game for 5 seconds
  {
    pauza();
    redraw();          //Redraw the snake and egg at the same position as it was
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
  {
    display.drawCircle(x[i], y[i], marime_sarpe, BLACK);

  }
  display.display();
}






void pauza() {
  display.clearDisplay();
  display.setTextColor(BLACK);
  for (byte i = 5; i > 0; i--)
  {
    display.setCursor(25, 10);
    display.setTextSize(1);
    display.print("PAUZA");
    display.setCursor(40, 30);
    display.print(i);
    display.display();
    delay(1000);
    display.clearDisplay();
  }
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
  delay(1000);
}

void reset_game() {
  while (digitalRead(BACK) != LOW) {
    display.clearDisplay();
    display.setTextColor(BLACK);
    display.setTextSize(1);
    display.setCursor(13, 5);
    display.print( F ("Apasa BACK pentru a reincerca"));
    display.display();
  }
  display.clearDisplay();
  slength = 8;
  score = 0;
  time = 280;
  redraw();
}


void depasit() {
  if (tempx <= 0 || tempx >= MAX_WIDTH || tempy <= 0 || tempy >= MAX_HEIGHT)
    if (bordura == true) {
      gameover();
      //reset_game();
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
