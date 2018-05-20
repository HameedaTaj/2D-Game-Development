#ifndef BUTTERFLY__H
#define BUTTERFLY__H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Butterfly : public Drawable {
public:
  Butterfly( const std::string&, float );
  Butterfly( const Butterfly& );
  Butterfly& operator=( const Butterfly& );

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return images[currentFrame]; }
  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  int getScaledWidth()  const { return getScale()*images[currentFrame]->getWidth(); }
  int getScaledHeight()  const { return getScale()*images[currentFrame]->getHeight(); }

  void restartGame();

private:
  std::vector<Image *> images;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  float prevViewX;
  float factor;

  void advanceFrame(Uint32 ticks);
};
#endif
