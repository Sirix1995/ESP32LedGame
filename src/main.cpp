#include <Arduino.h>

#define LED1 10
#define LED2 11
#define LED3 12
#define LED4 13
#define BUTTON 14

hw_timer_t *theTimer = NULL;
int8_t count = 0;
bool sens = true;
bool sequencerFlag = false;
bool buttonFlag = false;
uint8_t leds[4];
 
void IRAM_ATTR timerInterrupt()
{
  sequencerFlag = true;
}

void buttonInterrupt() {
  buttonFlag = true;
}

void runSequencer() {
  if(sens)
    count++;
  else
    count--;
  
  if(count == -1)
    count = 3;
  
  if(count == 4)
    count = 0;
  sequencerFlag = false;

  switch (count) {
    case 0:
      digitalWrite(LED1, 0);
      digitalWrite(LED2, 1);
      digitalWrite(LED3, 1);
      digitalWrite(LED4, 1);
      Serial.write("Phase 1\n");
      break;
    case 1:
      digitalWrite(LED1, 1);
      digitalWrite(LED2, 0);
      digitalWrite(LED3, 1);
      digitalWrite(LED4, 1);
      Serial.write("Phase 2\n");
      break;
    case 2:
      digitalWrite(LED1, 1);
      digitalWrite(LED2, 1);
      digitalWrite(LED3, 0);
      digitalWrite(LED4, 1);
      Serial.write("Phase 3\n");
      break;
    case 3:
      digitalWrite(LED1, 1);
      digitalWrite(LED2, 1);
      digitalWrite(LED3, 1);
      digitalWrite(LED4, 0);
      Serial.write("Phase 4\n");
      break;
  }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(BUTTON, INPUT);
    leds[0] = LED1;
    leds[1] = LED2;
    leds[2] = LED3;
    leds[3] = LED4;
    theTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(theTimer, &timerInterrupt, true);
    timerAlarmWrite(theTimer, 1000000, true);
    timerAlarmEnable(theTimer);
    
    attachInterrupt(BUTTON, &buttonInterrupt, FALLING);

}

void loop() {
  if(sequencerFlag) {
    runSequencer();
  }
  if(buttonFlag) {
    Serial.write("Bouton !\n");
    if(sens)
      sens = false;
    else
      sens = true;
    buttonFlag = false;
  }
}