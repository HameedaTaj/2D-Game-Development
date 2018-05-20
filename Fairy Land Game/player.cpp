#include "player.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

Player::~Player() {
  if (explosion) delete explosion;
}

Player::Player( const std::string& player, const std::string& bullet ) :
  Drawable(player,
           Vector2f(Gamedata::getInstance().getXmlInt(player+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(player+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(player+"/speedX"),
                    Gamedata::getInstance().getXmlInt(player+"/speedY"))
          ),
  images( RenderContext::getInstance()->getImages(player+"/rightSprite")),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(player+"/frames")),
  frameInterval( Gamedata::getInstance().getXmlInt(player+"/frameInterval")),
  timeSinceLastFrame(0),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  rightsprite( RenderContext::getInstance()->getImages(player+"/rightSprite") ),
  leftsprite( RenderContext::getInstance()->getImages(player+"/leftSprite") ),
  facing(RIGHT),
  initialVelocity(getVelocity()),
  initialPosition(getPosition()),
  observers(),
  bulletName(bullet),
  bulletInterval(Gamedata::getInstance().getXmlInt(bullet+"/bulletInterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bullet+"/minBulletSpeed")),
  rightOffset(Vector2f(
    Gamedata::getInstance().getXmlInt(bullet+"/rightOffset/x"),
    Gamedata::getInstance().getXmlInt(bullet+"/rightOffset/y")
  )),
  leftOffset(Vector2f(
    Gamedata::getInstance().getXmlInt(bullet+"/leftOffset/x"),
    Gamedata::getInstance().getXmlInt(bullet+"/leftOffset/y")
  )),
  bullets(BulletPool(bullet)),
  flowersCollected(0),
  enemiesDestroyed(0),
  initialLives(Gamedata::getInstance().getXmlInt(player+"/lives")),
  initialHealth(Gamedata::getInstance().getXmlInt(player+"/health")),
  currentLives(Gamedata::getInstance().getXmlInt(player+"/lives")),
  currentHealth(Gamedata::getInstance().getXmlInt(player+"/health")),
  enemyCollisionHealthLoss(Gamedata::getInstance().getXmlInt(player+"/enemyCollisionHealthLoss")),
  godMode(false),
  reachedTree(false)
{}

Player::Player(const Player& s) :
  Drawable(s),
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  rightsprite(s.rightsprite),
  leftsprite(s.leftsprite),
  facing(s.facing),
  initialVelocity(s.initialVelocity),
  initialPosition(s.initialPosition),
  observers( s.observers ),
  bulletName( s.bulletName ),
  bulletInterval( s.bulletInterval ),
  timeSinceLastBullet( s.timeSinceLastBullet ),
  minBulletSpeed( s.minBulletSpeed ),
  rightOffset( s.rightOffset ),
  leftOffset( s.leftOffset ),
  bullets( s.bullets ),
  flowersCollected ( s.flowersCollected ),
  enemiesDestroyed ( s.enemiesDestroyed ),
  initialLives( s.initialLives ),
  initialHealth( s.initialHealth ),
  currentLives( s.currentLives ),
  currentHealth( s.currentHealth ),
  enemyCollisionHealthLoss( s.enemyCollisionHealthLoss ),
  godMode( s.godMode ),
  reachedTree( s.reachedTree )
{}

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = s.images;
  explosion = s.explosion;
  currentFrame = s.currentFrame;
  numberOfFrames = s.numberOfFrames;
  frameInterval = s.frameInterval;
  timeSinceLastFrame = s.timeSinceLastFrame;
  worldWidth = s.worldWidth;
  worldHeight = s.worldHeight;
  rightsprite = s.rightsprite;
  leftsprite = s.leftsprite;
  facing = s.facing;
  initialVelocity = s.initialVelocity;
  initialPosition = s.initialPosition;
  observers = s.observers;
  bulletName = s.bulletName;
  bulletInterval = s.bulletInterval;
  timeSinceLastBullet = s.timeSinceLastBullet;
  minBulletSpeed = s.minBulletSpeed;
  rightOffset = s.rightOffset;
  leftOffset = s.leftOffset;
  bullets = s.bullets;
  flowersCollected = s.flowersCollected;
  enemiesDestroyed = s.enemiesDestroyed;
  initialLives = s.initialLives;
  initialHealth = s.initialHealth;
  currentLives = s.currentLives;
  currentHealth = s.currentHealth;
  enemyCollisionHealthLoss = s.enemyCollisionHealthLoss;
  godMode = s.godMode;
  reachedTree = s.reachedTree;
  return *this;
}

void Player::stop() {
  setVelocity(Vector2f(0,0));
}

void Player::right() {
  facing = RIGHT;
  if ( getX() < worldWidth - getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  facing = LEFT;
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
}
void Player::down()  {
  if ( getY() < worldHeight - getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::restartGame() {
  explosion = NULL;
  facing = RIGHT;
  flowersCollected = 0;
  enemiesDestroyed = 0;
  currentLives = initialLives;
  currentHealth = initialHealth;
  setPosition(initialPosition);
  reachedTree = false;
}

void Player::draw() const {
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
  bullets.draw();
}

bool Player::destroyIfShot( MovingEnemy* enemy ) {
  if ( bullets.collided(enemy) ) {
    enemy->gotShot();
    if (enemy->isDead()) {
      enemiesDestroyed += 1;
      enemy->explode();
      return true;
    }
  }
  return false;
}

bool Player::destroyIfShot( Devil* enemy ) {
  if ( bullets.collided(enemy) ) {
    enemy->gotShot();
    if (enemy->isDead()) {
      enemiesDestroyed += 1;
      enemy->explode();
      std::list<MovingEnemy*>::iterator ptr = observers.begin();
      while ( ptr != observers.end() ) {
        (*ptr)->explode();
        (*ptr)->setRespawn(false);
        ++ptr;
      }
      return true;
    }
  }
  return false;
}

void Player::shoot() {
  if (timeSinceLastBullet > bulletInterval) {
    Vector2f bulletPosition = getPosition();
    Vector2f bulletVelocity = Vector2f(initialVelocity[0], 0);
    switch(facing) {
      case LEFT:
        bulletPosition[0] += leftOffset[0];
        bulletPosition[1] += leftOffset[1];
        bulletVelocity[0] = -(minBulletSpeed + bulletVelocity[0]);
        break;
      case RIGHT:
        bulletPosition[0] += rightOffset[0];
        bulletPosition[1] += rightOffset[1];
        bulletVelocity[0] += minBulletSpeed;
        break;
    }
    bullets.shoot(bulletPosition, bulletVelocity);
    timeSinceLastBullet = 0;
  }
}

void Player::reset() {
  setPosition(Vector2f(getX(), -1000));
  if ( explosionDone() ) {
    delete explosion;
    explosion = NULL;
    std::list<MovingEnemy*>::iterator ptr = observers.begin();
    while ( ptr != observers.end() ) {
      if (currentLives > 0) {
        (*ptr)->moveToInitialPosition();
      } else {
        (*ptr)->explode();
        (*ptr)->setRespawn(false);
      }
      ++ptr;
    }
    if (currentLives > 0) {
      currentHealth = initialHealth;
      setPosition(initialPosition);
    }
  }
}

bool Player::explode() {
  enemiesDestroyed += 1;
  currentHealth -= enemyCollisionHealthLoss;
  if ( currentHealth <= 0 && !explosion ) {
    currentLives -= 1;
    Vector2f velocity(100, 100);
    Sprite sprite(getName(), getPosition(), velocity, images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
    return true;
  }
  return false;
}

bool Player::explosionDone() {
  return explosion && explosion->chunkCount() == 0;
}

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

void Player::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    this->reset();
    return;
  }
  advanceFrame(ticks);
  if ( getX() >= worldWidth - getScaledWidth() ) {
    reachedTree = true;
  }
  if (currentLives > 0) {
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
    bullets.update(ticks);
    timeSinceLastBullet += ticks;
    switch(facing) {
      case LEFT: setImages(leftsprite); break;
      case RIGHT: setImages(rightsprite); break;
    }
    std::list<MovingEnemy*>::iterator ptr = observers.begin();
    while ( ptr != observers.end() ) {
      (*ptr)->setPlayerPos( getPosition() );
      ++ptr;
    }
  }
  stop();
}

void Player::detach( MovingEnemy* o ) {
  std::list<MovingEnemy*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
