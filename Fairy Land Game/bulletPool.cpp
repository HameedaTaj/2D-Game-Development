#include <iostream>
#include <cmath>
#include "ioMod.h"
#include "bulletPool.h"
#include "imageFactory.h"
#include "collisionStrategy.h"

BulletPool::~BulletPool() {
  delete strategy;
}

BulletPool::BulletPool(const std::string& n) :
  name(n),
  velocity(
    Gamedata::getInstance().getXmlInt(name+"/speedX"),
    Gamedata::getInstance().getXmlInt(name+"/speedY")
  ),
  bulletImages( ImageFactory::getInstance().getImages(name) ),
  bulletList(),
  freeList(),
  strategy(NULL)
{
  const string thisStrategy = Gamedata::getInstance().getXmlStr(name+"/collisionStrategy");
  if (thisStrategy == "PerPixel") {
    strategy = new PerPixelCollisionStrategy;
  }
  else if (thisStrategy == "Rectangular") {
    strategy = new RectangularCollisionStrategy;
  }
  else {
    strategy = new MidPointCollisionStrategy;
  }
}

BulletPool::BulletPool(const BulletPool& b) :
  name(b.name),
  velocity(b.velocity),
  bulletImages(b.bulletImages),
  bulletList(b.bulletList),
  freeList(b.freeList),
  strategy(b.strategy)
  {}

BulletPool& BulletPool::operator=(const BulletPool& b) {
  name = b.name;
  velocity = b.velocity;
  bulletImages = b.bulletImages;
  bulletList = b.bulletList;
  freeList = b.freeList;
  strategy = b.strategy;
  return *this;
}

void BulletPool::shoot(const Vector2f& pos, const Vector2f& objVel) {
  if ( freeList.empty() ) {
    Bullet b( name, pos, objVel );
    bulletList.push_back( b );
  }
  else {
    Bullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back( b );
  }
}

bool BulletPool::collided(const Drawable* obj) {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while ( ptr != bulletList.end() ) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void BulletPool::draw() const {
  for ( const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

void BulletPool::update(Uint32 ticks) {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while ( ptr != bulletList.end() ) {
    ptr->update( ticks );
    if ( ptr->goneTooFar() ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else {
      ++ptr;
    }
  }
}
