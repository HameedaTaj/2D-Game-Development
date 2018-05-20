#include "movingEnemy.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

MovingEnemy::~MovingEnemy() {
  if (explosion) delete explosion;
}

MovingEnemy::MovingEnemy( const std::string& name, const Vector2f& pos, int w, int h ) :
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
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance")),
  viewWidth(Gamedata::getInstance().getXmlFloat("view/width")),
  viewHeight(Gamedata::getInstance().getXmlFloat("view/height")),
  worldWidth(Gamedata::getInstance().getXmlFloat("world/width")),
  worldHeight(Gamedata::getInstance().getXmlFloat("world/height")),
  enemyWidth(Gamedata::getInstance().getXmlFloat(name+"/imageWidth")),
  enemyRange(Gamedata::getInstance().getXmlFloat("view/enemyRange")),
  bulletsToDie(Gamedata::getInstance().getXmlFloat(name+"/bulletsToDie")),
  bulletsHit(0),
  initialPosition(Vector2f(rand()%(worldWidth-viewWidth)+viewWidth,
                           rand()%(viewHeight-enemyRange)+1)),
  initialVelocity(getVelocity()),
  respawn(true)
{
  setPosition(initialPosition);
}

MovingEnemy::MovingEnemy(const MovingEnemy& s) :
  Drawable(s),
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance),
  viewWidth(s.viewWidth),
  viewHeight(s.viewHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  enemyWidth(s.enemyWidth),
  enemyRange(s.enemyRange),
  bulletsToDie(s.bulletsToDie),
  bulletsHit(s.bulletsHit),
  initialPosition(s.initialPosition),
  initialVelocity(s.initialVelocity),
  respawn(s.respawn)
{}

MovingEnemy& MovingEnemy::operator=(const MovingEnemy& s) {
  Drawable::operator=(s);
  images = s.images;
	explosion = s.explosion;
  currentFrame = s.currentFrame;
  numberOfFrames = s.numberOfFrames;
  frameInterval = s.frameInterval;
  timeSinceLastFrame = s.timeSinceLastFrame;
  playerPos = s.playerPos;
  playerWidth = s.playerWidth;
  playerHeight = s.playerHeight;
  currentMode = s.currentMode;
  safeDistance = s.safeDistance;
  viewWidth = s.viewWidth;
  viewHeight = s.viewHeight;
  worldWidth = s.worldWidth;
  worldHeight = s.worldHeight;
  enemyWidth = s.enemyWidth;
  enemyRange = s.enemyRange;
  bulletsToDie = s.bulletsToDie;
  bulletsHit = s.bulletsHit;
  initialPosition = s.initialPosition;
  initialVelocity = s.initialVelocity;
  respawn = s.respawn;
  return *this;
}

void MovingEnemy::restartGame() {
  explosion = NULL;
  currentMode = NORMAL;
  bulletsHit = 0;
  setPosition(Vector2f(rand()%(worldWidth-viewWidth)+viewWidth, rand()%(viewHeight-enemyRange)+1));
  setVelocity(initialVelocity);
}

void MovingEnemy::removeFromScreen() {
  setPosition(Vector2f(worldWidth + enemyWidth + 1000, 1000));
  setVelocity(Vector2f(0, 0));
}

void MovingEnemy::draw() const {
	if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void MovingEnemy::goUp(Uint32 ticks) {
  float incr = getVelocityY() * static_cast<float>(ticks) * 0.001;
  setY( getY()-incr );
}

void MovingEnemy::goDown(Uint32 ticks) {
  float incr = getVelocityY() * static_cast<float>(ticks) * 0.001;
  setY( getY()+incr );
}

void MovingEnemy::explode() {
  bulletsHit = bulletsToDie;
  if ( !explosion ) {
		Vector2f velocity(100, 100);
    Sprite sprite(getName(), getPosition(), velocity, images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

bool MovingEnemy::explosionDone() {
	return explosion && explosion->chunkCount() == 0;
}

void MovingEnemy::reset() {
  removeFromScreen();
  if ( explosionDone() ) {
    delete explosion;
    explosion = NULL;
    bulletsHit = 0;
    if ( respawn ) {
      setX(worldWidth + enemyWidth + rand()%(500));
      setY(rand()%(viewHeight-enemyRange+1));
      setVelocity(initialVelocity);
    }
  }
}

void MovingEnemy::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

void MovingEnemy::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
		this->reset();
    return;
  }
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  Vector2f currentPos = getPosition();
  setX(currentPos[0]+incr[0]);

  if ( getX() < -(enemyWidth) ) {
    setX(worldWidth + enemyWidth + 500);
    setY(rand()%(viewHeight-enemyRange+1));
  }

  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float px= playerPos[0]+playerWidth/2;
  float py= playerPos[1]+playerHeight/2;
  float distanceToEnemy = std::abs(x-px);

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = PURSUE;
  }
  else if  ( currentMode == PURSUE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else if (x > 0 && y > 0 && x < worldWidth && y < worldHeight) {
      if ( y < py ) goDown(ticks);
      if ( y > py ) goUp(ticks);
    }
  }
}
