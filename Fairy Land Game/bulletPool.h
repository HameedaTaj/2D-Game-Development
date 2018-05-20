#ifndef BULLETPOOL__H
#define BULLETPOOL__H

#include <list>
#include "bullet.h"

class CollisionStrategy;

class BulletPool {
public:
  BulletPool(const std::string&);
  BulletPool(const BulletPool&);
  BulletPool& operator=(const BulletPool&);
  ~BulletPool();

  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount() const { return freeList.size(); }
  bool shooting() const { return !bulletList.empty(); }
  bool collided(const Drawable*);

private:
  std::string name;
  Vector2f velocity;
  std::vector<Image*> bulletImages;
  std::list<Bullet> bulletList;
  std::list<Bullet> freeList;
  CollisionStrategy* strategy;
};
#endif
