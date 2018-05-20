#ifndef FLOWER__H
#define FLOWER__H

#include <string>
#include "drawable.h"

class Flower : public Drawable {
public:
  Flower(const std::string&);
  Flower(const Flower&);
  virtual ~Flower() { }
  Flower& operator=(const Flower&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { return image->getSurface(); }
  int getScaledWidth()  const { return getScale()*image->getWidth();  }
  int getScaledHeight() const { return getScale()*image->getHeight(); }

  void removeFromScreen() { setPosition(Vector2f(-500, -500)); }
  void restartGame();

private:
  const Image * image;
  int worldWidth;
  int worldHeight;
  Vector2f initialPosition;
};
#endif
