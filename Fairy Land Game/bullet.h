#ifndef BULLET__H
#define BULLET__H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Bullet : public Drawable {
public:
  Bullet( const std::string& );
  Bullet( const std::string&, const Vector2f&, const Vector2f& );
  Bullet( const Bullet& );
  Bullet& operator=( const Bullet& );

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return images[currentFrame]; }
  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  int getScaledWidth()  const { return getScale()*images[currentFrame]->getWidth(); }
  int getScaledHeight()  const { return getScale()*images[currentFrame]->getHeight(); }

  bool goneTooFar() const { return tooFar; }
  void reset();

private:
  std::vector<Image *> images;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  float distance;
  float maxDistance;
  bool tooFar;

  void advanceFrame(Uint32 ticks);
};
#endif
