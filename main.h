#ifndef MAIN_H
#define MAIN_H

#include <Watchy.h>
#include "brain_vis.h"

RTC_DATA_ATTR int face = 0;
RTC_DATA_ATTR int faces_vars[6][2] = {{1,0},{1,0},{1,0},{1,0},{2,0},{1,0}}; //variants, current
//Brutus, Bahn, Maze, Redub, Novel, Hobbit

class WatchyBrain : public Watchy {
  using Watchy::Watchy;
  public:
    void drawWatchFace();
    void drawBahn(int variant, float batt);
    virtual void handleButtonPress();//Must be virtual in Watchy.h too
};

#include "bahn.h"

void WatchyBrain::handleButtonPress() {
  if (guiState == WATCHFACE_STATE) {
    //Up and Down switch watch faces
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    
    if (wakeupBit & MENU_BTN_MASK) {
      Watchy::handleButtonPress();
      return;
    }

    RTC.read(currentTime);
    showWatchFace(true);
  } else {Watchy::handleButtonPress();}

  return;
}


void WatchyBrain::drawWatchFace() {
  // ** UPDATE **
  //resets step counter at midnight everyday
  if (currentTime.Hour == 00 && currentTime.Minute == 00) {
    sensor.resetStepCounter();
  }

  // ** GET BATTERY **
  Serial.print(getBatteryVoltage()); 
  float batt = (getBatteryVoltage())-3.0;
  //if (batt > 1) { batt = 1; } else if (batt < 0) { batt = 0; }
  
  // ** DRAW WATCHFACE **
  drawBahn(0, batt);  
}

#endif
