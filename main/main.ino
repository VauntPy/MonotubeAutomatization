#include "view.h"
#include "model.h"
#include "config.h"

volatile int posX, posY;
volatile unsigned long lastMillis;
unsigned long mSec;

void setup(){
  Serial.begin(115200);

  const int interruptRight = 0, interruptBottom = 1;
  pinMode(interruptRight, INPUT_PULLUP);
  pinMode(interruptBottom, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptRight), cursorRight, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptBottom), cursorBottom, FALLING);
  
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.blink();

  
  rtc.begin();

  if(!rtc.begin()) {
    while(1) {
      Serial.println("NO RTC");
      delay(100);
    }
  }
  if (!rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  posX = 0; posY = 0; mSec = millis();
  updateLastMillis();
}

void moveCursor() {
  lcd.setCursor(posY, posX);
}

void updateLastMillis() {
  lastMillis = millis();
}

boolean preventBouncing() {
  // Serial.println("DEBOUNCING");
  if(millis() - lastMillis >= antiDebounceLatency) return true;
  
  return false;
}

void cursorRight() {
  if(!preventBouncing()) return;
  if(posY == LCD_COLUMNS - 1) posY = 0;
  else posY = posY + 1;

  moveCursor();
  updateLastMillis();
}

void cursorBottom() {
  if(!preventBouncing()) return;
  if(posX == 0) posX = posX + 1;
  else posX = 0;

  moveCursor();
  updateLastMillis();
}

String printDate() {
  DateTime now = rtc.now();
  const String hour = String(now.hour());
  const String minutes = String(now.minute());
  const String seconds = String(now.second());

  const String dateTime = hour + ":" + minutes + ":" + seconds;
  return dateTime;
}

void loop(){
  // if(millis() - mSec >= 1000){
  //   const String dateTime = printDate();
  //   Serial.println(dateTime);
  //   lcd.clear();
  //   lcd.home();
  //   lcd.print(dateTime);
  //   mSec = millis();
  // }

  const String dateTime = printDate();
  Serial.println(dateTime);
  delay(1000);

}