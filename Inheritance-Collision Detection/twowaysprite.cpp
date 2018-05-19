#include "twowaysprite.h"
#include "gamedata.h"
#include "renderContext.h"


void TwoWaySprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwoWaySprite::TwoWaySprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesRight( ImageFactory::getInstance().getImages(name) ),
  imagesLeft(ImageFactory::getInstance().getImages(name+"left")),
  images(imagesRight),
   frameHeight(Gamedata::getInstance().getXmlInt("Fairy/imageHeight")),
   frameWidth(Gamedata::getInstance().getXmlInt("Fairy/imageWidth")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
 timeSinceLastFrame( 0 ),

  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

 worldWidth(Gamedata::getInstance().getXmlInt("world/width")){ }

TwoWaySprite::TwoWaySprite(const TwoWaySprite& s) :
  Drawable(s),
  imagesRight(s.imagesRight),
  imagesLeft(s.imagesLeft), 
  images(s.imagesRight), 
  frameHeight(s.getImage()->getHeight()),
  frameWidth(s.getImage()->getWidth()),
   currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldHeight( s.worldHeight ),  worldWidth( s.worldWidth )

  { }

TwoWaySprite& TwoWaySprite::operator=(const TwoWaySprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );  worldHeight = ( s.worldHeight );

  worldWidth = ( s.worldWidth );
  return *this;
}

void TwoWaySprite::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwoWaySprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images=imagesRight;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    images=imagesLeft;
  }  

}
