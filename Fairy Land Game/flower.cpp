#include <cmath>
#include <random>
#include <functional>
#include "flower.h"
#include "gamedata.h"
#include "renderContext.h"

Flower::Flower(const std::string& name) :
  Drawable(name, Vector2f(0, 0), Vector2f(0, 0)),
  image( RenderContext::getInstance()->getImage(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  initialPosition(Vector2f(rand()%(worldWidth-200)+150, rand()%(worldHeight - 50)))
{
  setPosition(initialPosition);
}

Flower::Flower(const Flower& s) :
  Drawable(s),
  image(s.image),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  initialPosition(s.initialPosition)
{}

Flower& Flower::operator=(const Flower& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  initialPosition = rhs.initialPosition;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Flower::restartGame() {
  setPosition(Vector2f(rand()%(worldWidth + 200), rand()%(worldHeight)));
}

void Flower::draw() const {
  if(getScale() < SCALE_EPSILON) return;
  image->draw(getX(), getY(), getScale());
}

void Flower::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
}
