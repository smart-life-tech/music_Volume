// please use the image attached in this repo, let me know what we have on serial

#include <DigiPotX9Cxxx.h>

DigiPot pot(2,3,4);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Starting");  

  for (int i=0; i<100; i++) {
    pot.increase(1);
    Serial.println(analogRead(A0));
    delay(20);
  }
  
  
  for (int i=0; i<100; i++) {
    pot.decrease(1);
    Serial.println(analogRead(A0));
    delay(20);
  }

}
