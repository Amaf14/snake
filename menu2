#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 7, 8);  //Initialise display object

#define DOWN 11
#define UP 10
#define ENTER 9
#define BACK 12





int p1, a;
bool light=true, border=false;



void setup(){
  Serial.begin(19200);
  display.begin();
  display.clearDisplay();
  
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  pinMode(2,OUTPUT);
  digitalWrite(2,light);
  display.setContrast(25);

  pinMode(UP,INPUT);
  pinMode(DOWN,INPUT);
  digitalWrite(UP,HIGH);
  digitalWrite(DOWN,HIGH);
  pinMode(ENTER,INPUT);
  digitalWrite(ENTER,HIGH);
  pinMode(BACK,INPUT);
  digitalWrite(BACK,HIGH);

  
}

void borderf(){
  display.setCursor(55,20);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  if(border==true)
    display.print( F("ON"));
  if(border==false) 
    display.print( F("OFF"));
  display.display();    
}

void selectare(int a) {
  //f(x)=15x+5
  display.fillRect(8,15*a+5,4,7,BLACK);
  //display.display();
}




void menu(int a){
  display.clearDisplay();
  selectare(a);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(15,5);
  display.print( F("Play"));
  display.setCursor(15,20);
  display.print( F("Settings"));
  display.setCursor(15,35);
  display.print( F("Score"));
  display.display();
}

void settings(){
  display.clearDisplay();
  selectare(a);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(15,5);
  display.print( F("Backlight"));
  display.setCursor(15,20);
  display.print( F("Border"));
  display.setCursor(15,35);
  display.print( F("Reset Score"));
}

void loop() {

  menu(a);
  if(digitalRead(UP)==LOW && a!=0)
    a--;
  if(digitalRead(DOWN)==LOW && a!=2)
    a++;
  if(digitalRead(ENTER)==LOW)
    p1=a;

  switch(p1) {
    
    case 0:
    //game();    
    break;

    case 1:
    a=0;
    while(p1==1){
      settings();
      Serial.print(a);
      Serial.println(border);
      borderf();
      if(digitalRead(UP)==LOW && a!=0)
        a--;
      if(digitalRead(DOWN)==LOW && a!=2)
        a++;
      if(digitalRead(ENTER)==LOW){
        delay(10);
        if(a==0){
          if ( light == true )
            light = false;
          else 
            light = true;  
          digitalWrite(2, light);
          delay(100);
        }
        if(a==1)
          if ( border == true )
            border = false;
          else 
            border = true;
        if(a==2){
          //score=0;
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(BLACK);
          display.setCursor(15,5);
          display.print(F("0000"));
          display.display();
        }
      }    
      if(digitalRead(BACK)==LOW && p1!=0)
        p1--;     
    delay(50);    
    }
    break;

    case 2:
    while(p1=2 && digitalRead(BACK)!=LOW){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(15,5);
    display.print(F("1337"));
    display.display();
    delay(20);
    }
    break;
  }
  delay(50);
}
