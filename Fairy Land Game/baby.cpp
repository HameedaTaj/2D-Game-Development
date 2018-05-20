#include <cmath>
#include <random>
#include <functional>
#include "baby.h"
#include "gamedata.h"
#include "renderContext.h"

Baby::Baby(const std::string& name) :
  Drawable(name,
          Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                   Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
          Vector2f(0, 0)
          ),
  image( RenderContext::getInstance()->getImage(name) )
{}

Baby::Baby(const Baby& s) :
  Drawable(s),
  image(s.image)
{}

Baby& Baby::operator=(const Baby& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  return *this;
}

void Baby::draw() const {
  image->draw(getX(), getY(), 0.5);
}

void Baby::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
}
