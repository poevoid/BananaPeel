#pragma once
#include "Vector.h"

#define FRAME(x) x * 3 + arduboy.currentPlane()
#define MAX_FPS 10
#define SPLASH_TIME 75

constexpr uint16_t EEPROM_BASE = 0x03E0; // 992 decimal
constexpr uint16_t PEEL_MAGIC = 0x4250;  // "BP" identifier

int startcounter = SPLASH_TIME;
uint16_t currentPlane;
bool peeling = false;
bool peeled = false;
int nanacounter = 0;
int peelCount = 0;          // Add this line
int displayTextTimer = 0;   // Add this line

struct Sprite {
  uint8_t x, y;
  uint24_t sprite;
  uint8_t framecount;
  uint8_t currentframe;
  uint8_t framewait;
  int counter;
  bool inc;
};
struct Paddle {
  int x, y;
  uint24_t sprite;
  uint8_t framecount;
  uint8_t currentframe;
  uint8_t framewait;
  int counter;
  uint8_t height;
  uint8_t width;
  uint8_t score;
};

enum class Screen : uint8_t {
  Splash,
  Game,
};



Paddle player = { 128-77, 0, peel, 2, 0, 3, 0, 22, 32, 0 };
Sprite Abutton = { 4, 32, abutton, 3, 0, 6, 0 };
Sprite Bbutton = { 4, 48, bbutton, 3, 0, 6, 0 };
Screen screen = { Screen::Splash };