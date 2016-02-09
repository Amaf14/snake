#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 7, 8);  //Initialise display object

void rama(){
  int i;
  display.clearDisplay();
  for(i=4;i<=46;i+=4){
    display.drawCircle(1,i,3,BLACK);
    display.drawCircle(82,i,3,BLACK);
  }
  for(i=4;i<=82;i+=4){
    display.drawCircle(i,1,3,BLACK);
    display.drawCircle(i,46,3,BLACK);
  }
}

void logo0(bool d){
  int i,j;
  display.clearDisplay();
  for(i=4;i<=46;i+=4)
    for(j=4;j<=82;j+=4){
       display.drawCircle(j,i,3,BLACK);
       if(d==true)
        display.display();
    }
  display.display(); 
}

void logo1(){
  rama();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(13,10);
  display.print(F("SNAKE"));
  display.setCursor(42,26);
  display.print(F("AVR"));
  display.display();
}

void logo2(){
  rama();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(7,7);
  display.print(F("Realizat de"));
  display.setCursor(7,16);
  display.print(F("Maftei"));
  display.setCursor(7,25);
  display.print(F("Alexandru"));
  display.setCursor(7,34);
  display.print(F("Constantin"));
  display.display();
}
  


void setup() {
  display.begin();
  display.clearDisplay();
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  display.setContrast(25);

}

void loop() {
  logo0(false);
  delay(500);
  logo1();
  delay(500);
  logo2();
  delay(500);
}
