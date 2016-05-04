
#include<EEPROM.h>
void whscore(int a){
int x=EEPROM.read(0);
if(x<a)
EEPROM.write(0, a);
}

int rhscore(){
return EEPROM.read(0);
}

void rshscore(){
EEPROM.write(0, 0);
}
