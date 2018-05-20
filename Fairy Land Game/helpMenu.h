#ifndef HELPMENU__H
#define HELPMENU__H

#include <string>
#include <SDL.h>
#include <stdint.h>
#include "clock.h"
#include "ioMod.h"
#include "player.h"

class HelpMenu {
public:
  static HelpMenu& getInstance();
  ~HelpMenu();
  void draw() const;

private:
  const int width;
  const int height;
  int x;
  int y;
  SDL_Rect rect;
  SDL_Renderer* renderer;
  HelpMenu();
  HelpMenu(const HelpMenu&);
  HelpMenu& operator=(const HelpMenu&);
  const IOmod& io;
  SDL_Color textFontColor;
};
#endif
