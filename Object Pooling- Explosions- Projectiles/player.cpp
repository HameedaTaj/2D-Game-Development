#include "player.h"
#include "smartSprite.h"


Player::Player( const std::string& name) :
  MultiSprite(name),
  collision(false),
  initialVelocity(getVelocity()),
  observers(),
  facing(RIGHT)
{ }

Player::Player(const Player& s) :
  MultiSprite(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  observers(s.observers),
  facing(s.facing)
  { }

Player& Player::operator=(const Player& s) {
  MultiSprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  facing = s.facing;
  return *this;
}

void Player::stop() { 
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    facing = RIGHT;
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    facing = LEFT;
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
void Player::update(Uint32 ticks) {
  if ( !collision ) MultiSprite::update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

  stop();
}

