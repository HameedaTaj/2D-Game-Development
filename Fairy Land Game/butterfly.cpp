#include <iostream>
#include <cmath>
#include "butterfly.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "renderContext.h"
#include "viewport.h"

Butterfly::Butterfly( const std::string& name, float fact ) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame(0),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  prevViewX(0),
  factor(fact)
{
  setPosition(Vector2f(rand()%(worldWidth-1000)+500, rand()%(worldHeight-400)+300));
}

Butterfly::Butterfly( const Butterfly& b ) :
  Drawable(b),
  images(b.images),
  currentFrame(b.currentFrame),
  numberOfFrames(b.numberOfFrames),
  frameInterval(b.frameInterval),
  timeSinceLastFrame(b.timeSinceLastFrame),
  worldWidth(b.worldWidth),
  worldHeight(b.worldHeight),
  prevViewX(b.prevViewX),
  factor(b.factor)
{}

Butterfly& Butterfly::operator=( const Butterfly& b ) {
  Drawable::operator=(b);
  images = b.images;
  currentFrame = b.currentFrame;
  numberOfFrames = b.numberOfFrames;
  frameInterval = b.frameInterval;
  timeSinceLastFrame = b.timeSinceLastFrame;
  worldWidth = b.worldWidth;
  worldHeight = b.worldHeight;
  prevViewX = b.prevViewX;
  factor = b.factor;
  return *this;
}

void Butterfly::restartGame() {
  setPosition(Vector2f(rand()%(worldWidth-1000)+500, rand()%(worldHeight-400)+300));
}

void Butterfly::draw() const {
	images[currentFrame]->draw(getX(), getY(), getScale());
}

void Butterfly::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

void Butterfly::update(Uint32 ticks) {
  Viewport& view = Viewport::getInstance();
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  Vector2f position = getPosition() + incr;
  if (prevViewX < view.getX()) {
    position[0] += factor;
  } else if (prevViewX > view.getX()) {
    position[0] -= factor;
  }
  prevViewX = view.getX();
  setPosition(position);
  if ( getX() < -(images[0]->getWidth()) ) {
    setX(worldWidth + images[0]->getWidth());
    setY(rand()%(worldHeight-400)+300);
  }
}
