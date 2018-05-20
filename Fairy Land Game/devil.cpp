#include "devil.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"
#include "viewport.h"

Devil::~Devil() {
  if (explosion) delete explosion;
}

Devil::Devil( const std::string& name, const std::string& bullet ) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  viewWidth(Gamedata::getInstance().getXmlFloat("view/width")),
  viewHeight(Gamedata::getInstance().getXmlFloat("view/height")),
  worldWidth(Gamedata::getInstance().getXmlFloat("world/width")),
  worldHeight(Gamedata::getInstance().getXmlFloat("world/height")),
  enemyWidth(Gamedata::getInstance().getXmlFloat(name+"/imageWidth")),
  enemyRange(Gamedata::getInstance().getXmlFloat("view/enemyRange")),
  bulletsToDie(Gamedata::getInstance().getXmlFloat(name+"/bulletsToDie")),
  bulletsHit(0),
  initialPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                           Gamedata::getInstance().getXmlInt(name+"/startLoc/y"))),
  bulletName(bullet),
  bulletInterval(Gamedata::getInstance().getXmlInt(bullet+"/bulletInterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bullet+"/minBulletSpeed")),
  leftOffset(Vector2f(
   Gamedata::getInstance().getXmlInt(bullet+"/leftOffset/x"),
   Gamedata::getInstance().getXmlInt(bullet+"/leftOffset/y")
  )),
  bullets(BulletPool(bullet))
{
  setPosition(initialPosition);
}

Devil::Devil(const Devil& s) :
  Drawable(s),
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  viewWidth(s.viewWidth),
  viewHeight(s.viewHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  enemyWidth(s.enemyWidth),
  enemyRange(s.enemyRange),
  bulletsToDie(s.bulletsToDie),
  bulletsHit(s.bulletsHit),
  initialPosition(s.initialPosition),
  bulletName(s.bulletName),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  minBulletSpeed(s.minBulletSpeed),
  leftOffset(s.leftOffset),
  bullets(s.bullets)
{}

Devil& Devil::operator=(const Devil& s) {
  Drawable::operator=(s);
  images = s.images;
	explosion = s.explosion;
  currentFrame = s.currentFrame;
  numberOfFrames = s.numberOfFrames;
  frameInterval = s.frameInterval;
  timeSinceLastFrame = s.timeSinceLastFrame;
  viewWidth = s.viewWidth;
  viewHeight = s.viewHeight;
  worldWidth = s.worldWidth;
  worldHeight = s.worldHeight;
  enemyWidth = s.enemyWidth;
  enemyRange = s.enemyRange;
  bulletsToDie = s.bulletsToDie;
  bulletsHit = s.bulletsHit;
  initialPosition = s.initialPosition;
  bulletName = s.bulletName;
  bulletInterval = s.bulletInterval;
  timeSinceLastBullet = s.timeSinceLastBullet;
  minBulletSpeed = s.minBulletSpeed;
  leftOffset = s.leftOffset;
  bullets = s.bullets;
  return *this;
}

void Devil::restartGame() {
  explosion = NULL;
  bulletsHit = 0;
  setPosition(initialPosition);
}

void Devil::removeFromScreen() {
  setPosition(Vector2f(worldWidth + enemyWidth + 1000, 1000));
}

void Devil::draw() const {
	if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
  bullets.draw();
}

void Devil::explode() {
  bulletsHit = bulletsToDie;
  if ( !explosion ) {
		Vector2f velocity(100, 100);
    Sprite sprite(getName(), getPosition(), velocity, images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void Devil::reset() {
  removeFromScreen();
  if ( explosionDone() ) {
    delete explosion;
    explosion = NULL;
  }
}

bool Devil::explosionDone() {
	return explosion && explosion->chunkCount() == 0;
}

void Devil::shoot() {
  if (timeSinceLastBullet > bulletInterval) {
    Vector2f bulletPosition = getPosition();
    Vector2f bulletVelocity = Vector2f(getVelocity()[0], 0);
    bulletPosition[0] += leftOffset[0];
    bulletPosition[1] += leftOffset[1];
    bulletVelocity[0] = -(minBulletSpeed + bulletVelocity[0]);
    bullets.shoot(bulletPosition, bulletVelocity);
    if (Viewport::getInstance().getX() + viewWidth > getX()) {
      Gamedata::getInstance().getSoundInstance()->playSoundEffect("BOSS_ENEMY_SHOOT");
    }
    timeSinceLastBullet = 0;
  }
}

void Devil::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

void Devil::update(Uint32 ticks) {
  bullets.update(ticks);
  timeSinceLastBullet += ticks;
  if ( explosion ) {
    explosion->update(ticks);
		this->reset();
    return;
  }
  shoot();
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }
}
