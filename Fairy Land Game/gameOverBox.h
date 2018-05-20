#ifndef GAMEOVERBOX__H
#define GAMEOVERBOX__H

#include <string>
#include <SDL.h>
#include <stdint.h>
#include "clock.h"
#include "ioMod.h"
#include "player.h"

class GameOverBox {
public:
  static GameOverBox& getInstance(Player*);
  ~GameOverBox();
  void draw(const Vector2f& pos, const bool) const;

private:
  Player* player;
  Image* heartImage;
  Image* flowerImage;
  std::vector<Image *> deadGreenEnemy;
  std::vector<Image *> deadRedEnemy;
  const int width;
  const int height;
  int x;
  int y;
  SDL_Rect rect;
  SDL_Renderer* renderer;
  GameOverBox(Player*);
  GameOverBox(const GameOverBox&);
  GameOverBox& operator=(const GameOverBox&);
  const IOmod& io;
  SDL_Color textFontColor;
  void drawHealthBar(int x, int y, int w, int h, float percent) const;
};
#endif
