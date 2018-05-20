#ifndef HUD_H
#define HUD_H
#include <SDL.h>
#include<string>
#include<stdint.h>
#include "drawable.h"
#include "clock.h"
#include "ioMod.h"

class Hud{
public:
  static Hud& getInstance();
  ~Hud();
  void displayHud() const;
  void drawPool(SDL_Renderer * const, unsigned int, unsigned int);
private:
  const int width;
  const int height;
  int x;
  int y;
  SDL_Rect Rectangle;
  SDL_Renderer* renderer;
  Clock& clock;
  Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
};
#endif
