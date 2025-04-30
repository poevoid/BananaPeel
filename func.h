#include "Arduboy2Core.h"

#include "vars.h"


void playerInput() {
  if (displayTextTimer == 0 && (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))) {
    peeling = true;
  } else {
    peeling = false;
  }
}

void updateNana() {
  if (peeling) {
    nanacounter+=16;
  } else {
    if (nanacounter > 0) {
      nanacounter--;
    }
  }

  if (nanacounter <= 100) {
    player.currentframe = 0;
    peeled = false;
  } else if (nanacounter <= 200) {
    player.currentframe = 1;
  } else if (nanacounter <= 300) {
    player.currentframe = 2;
  } else if (nanacounter <= 400) {
    player.currentframe = 3;
  } else if (nanacounter <= 500) {
    player.currentframe = 4;
  } else if (nanacounter > 500) {
    player.currentframe = 4;
    if (!peeled) {
      peeled = true;
      peelCount++;
      displayTextTimer = 60;  // 2 seconds at 30fps
      
      EEPROM.put(EEPROM_BASE+2, peelCount); // Save to EEPROM
    }
  }
}
template<typename T>
//This animaton function takes a sprite and runs through all its frames, then starts over at frame 0
void animateSprite(T& structObj, uint8_t T::*cframe, uint8_t T::*framec, int T::*counter, uint8_t T::*wait) {
  if (structObj.*counter % (FRAME(structObj.*wait)) == 0) {
    if (structObj.*cframe < structObj.*framec) {
      structObj.*cframe += 1;
    } else {
      structObj.*cframe = 0;
    }
  }
  structObj.*counter += 1;
}


template<typename T>
//This animation function takes a sprite and runs trough all its frames in incremental order, then when reaching the final frame, in decremental order back to zero, ad infinitum
void animateFWB(T& structObj, uint8_t T::*cframe, uint8_t T::*framec, int T::*counter, uint8_t T::*wait, bool T::*inc) {
  if (structObj.*counter % (FRAME(structObj.*wait)) == 0) {
    if (structObj.*cframe == structObj.*framec) {
      structObj.*inc = false;
    }
    if (structObj.*cframe == 0) {
      structObj.*inc = true;
    }
    if (structObj.*inc) {
      if (structObj.*cframe < structObj.*framec) {
        structObj.*cframe += 1;
      }
    } else {
      if (structObj.*cframe > 0) {
        structObj.*cframe -= 1;
      }
    }
  }
  structObj.*counter += 1;
}

void update() {

  switch (screen) {
    case Screen::Splash:
      if (startcounter != 0) {
        startcounter--;
      } else {
        screen = Screen::Game;
      }
      break;
    case Screen::Game:
      playerInput();
      updateNana();
      // Update A button animation
      if (arduboy.pressed(A_BUTTON)) {
        Abutton.currentframe = 1;
      } else {
        Abutton.currentframe = 0;
      }

      if (arduboy.pressed(B_BUTTON)) {
        Bbutton.currentframe = 0;
      } else {
        Bbutton.currentframe = 1;
      }

      if (displayTextTimer > 0) {
        displayTextTimer--;
        if (displayTextTimer <= 0) {
          nanacounter = 0;
          peeled = false;
          player.currentframe = 0;
        }
      }
      break;
  }
}


void render() {
  uint16_t currentPlane = arduboy.currentPlane();

  switch (screen) {

    case Screen::Splash:
      SpritesU::drawPlusMaskFX(0, 0, punklogooutlined, FRAME(0));
      break;


    case Screen::Game:

      if (currentPlane <= 1) {
#ifndef OLED_SH1106
        arduboy.fillScreen(WHITE);
#endif
      }
      SpritesU::fillRect_i8(Bbutton.x + 18, Bbutton.y, 100, 8, arduboy.color(BLACK));

      // Progress bar fill
      uint16_t barWidth = (nanacounter > 500 ? 500 : nanacounter) / 5;  // 500 = 100px
      SpritesU::fillRect_i8(Bbutton.x + 18, Bbutton.y, barWidth, 8, arduboy.color(DARK_GRAY));

      SpritesU::drawPlusMaskFX(player.x, player.y, player.sprite, FRAME(player.currentframe));
      SpritesU::drawPlusMaskFX(Abutton.x, Abutton.y, Abutton.sprite, FRAME(Abutton.currentframe));
      SpritesU::drawPlusMaskFX(Bbutton.x, Bbutton.y, Bbutton.sprite, FRAME(Bbutton.currentframe));
      // Draw peel counter
      arduboy.setCursor(0, 0);
      arduboy.print("Peels: ");
      arduboy.print(peelCount);
      //arduboy.setCursor(0, 0);
      //arduboy.print("peeling: ");
      //arduboy.print(nanacounter);

     
      // Draw completion text
      if (peeled && displayTextTimer > 0) {
        arduboy.setCursor(20, 20);
        arduboy.print("Nanner Peeled!!");
      }
      break;
  }
}