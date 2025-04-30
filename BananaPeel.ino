#define ABG_IMPLEMENTATION
#define ABG_SYNC_PARK_ROW
#define SPRITESU_IMPLEMENTATION
#define OLED_SH1106  //for arduboy mini, set ABG_REFRESH_HZ TO 95
#define SPRITESU_OVERWRITE
#define SPRITESU_PLUSMASK
#define SPRITESU_FX
#define SPRITESU_RECT
#include <ArduboyFX.h>
#include <EEPROM.h>
#include "src/ArduboyG.h"
#include "src/SpritesU.hpp"
#include "fxdata/fxdata.h"

ArduboyG_Config<ABG_Mode::L4_Triplane> arduboy;

#include "func.h"

void setup() {

  arduboy.boot();
  
  arduboy.startGray();
  FX::begin(FX_DATA_PAGE, FX_SAVE_PAGE);
  uint16_t magic;
  EEPROM.get(EEPROM_BASE, magic);
  if (magic != PEEL_MAGIC) {
    peelCount = 0;
    EEPROM.put(EEPROM_BASE, PEEL_MAGIC);      // Bytes 992-993: Magic
    EEPROM.put(EEPROM_BASE + 2, peelCount);   // Bytes 994-995: Counter
  } else {
    EEPROM.get(EEPROM_BASE + 2, peelCount);   // Load existing counter
  }
}

void loop() {

  
  
  arduboy.waitForNextPlane();
  FX::disableOLED();
  if (arduboy.needsUpdate()) {
    arduboy.pollButtons();
    update();
    
  }
  render();
  FX::enableOLED();
}