#include <iostream>
#include <cmath>
#include "bullet.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "renderContext.h"

Bullet::Bullet( const std::string& name ) :
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
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  distance(0),
  maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
  tooFar(false)
{}

Bullet::Bullet( const std::string& name, const Vector2f& pos, const Vector2f& vel ) :
  Drawable(name, pos, vel),
  images(RenderContext::getInstance()->getImages(name)),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  distance(0),
  maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
  tooFar(false)
{}

Bullet::Bullet( const Bullet& b ) :
  Drawable(b),
  images(b.images),
  currentFrame(b.currentFrame),
  numberOfFrames(b.numberOfFrames),
  frameInterval(b.frameInterval),
  timeSinceLastFrame(b.timeSinceLastFrame),
  worldWidth(b.worldWidth),
  worldHeight(b.worldHeight),
  distance(b.distance),
  maxDistance(b.maxDistance),
  tooFar(b.tooFar)
{}

Bullet& Bullet::operator=( const Bullet& b ) {
  Drawable::operator=(b);
  images = b.images;
  currentFrame = b.currentFrame;
  numberOfFrames = b.numberOfFrames;
  frameInterval = b.frameInterval;
  timeSinceLastFrame = b.timeSinceLastFrame;
  worldWidth = b.worldWidth;
  worldHeight = b.worldHeight;
  distance = b.distance;
  maxDistance = b.maxDistance;
  tooFar = b.tooFar;
  return *this;
}

void Bullet::draw() const {
	images[currentFrame]->draw(getX(), getY(), getScale());
}

void Bullet::reset() {
  tooFar = false;
  distance = 0;
}

void Bullet::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

void Bullet::update(Uint32 ticks) {
  Vector2f pos = getPosition();
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( getY() < 0 || getY() + getScaledHeight() > worldHeight ) {
    tooFar = true;
  }
  if ( getX() < 0 || getX() > worldWidth ) {
    tooFar = true;
  }
  distance += ( hypot( getX()-pos[0], getY()-pos[1]) );
  if ( distance > maxDistance ) {
    tooFar = true;
  }
}
