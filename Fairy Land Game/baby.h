#ifndef BABY__H
#define BABY__H

#include <string>
#include "drawable.h"

class Baby : public Drawable {
public:
  Baby(const std::string&);
  Baby(const Baby&);
  virtual ~Baby() { }
  Baby& operator=(const Baby&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { return image->getSurface(); }
  int getScaledWidth()  const { return getScale()*image->getWidth();  }
  int getScaledHeight() const { return getScale()*image->getHeight(); }

private:
  const Image * image;
};
#endif
