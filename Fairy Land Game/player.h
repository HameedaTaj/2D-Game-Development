#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <list>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "movingEnemy.h"
#include "devil.h"
#include "bulletPool.h"

class ExplodingSprite;

class Player : public Drawable {
public:
  Player(const std::string&, const std::string&);
  Player(const Player&);
  Player& operator=(const Player&);
  ~Player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return images[currentFrame]; }
  int getScaledWidth()  const { return getScale()*images[currentFrame]->getWidth(); }
  int getScaledHeight()  const { return getScale()*images[currentFrame]->getHeight(); }
  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  void setImages(std::vector<Image *> newImages) { images = newImages; }

  void addFlower() { flowersCollected++; }
  void incrementEnemiesDestroyed() { enemiesDestroyed++; }
  void toggleGodMode() { godMode = !godMode; }
  bool isInvincible() { return godMode; }
  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }
  int getRemainingLives() const { return currentLives; }
  int getTotalLives() const { return initialLives;; }
  int getNumFlowersCollected() const { return flowersCollected; }
  int getNumEnemiesDestroyed() const { return enemiesDestroyed; }
  int getCurrentHealth() const { return currentHealth; }
  int getTotalHealth() const { return initialHealth; }
  void reset();
  void restartGame();
  bool explosionDone();
  bool explode();
  void stop();
  void right();
  void left();
  void up();
  void down();
  void shoot();
  void attach( MovingEnemy* o ) { observers.push_back(o); }
  void detach( MovingEnemy* o );
  bool destroyIfShot(MovingEnemy*);
  bool destroyIfShot(Devil*);
  bool hasReachedTree() { return reachedTree; }

private:
  enum Facing { LEFT, RIGHT };
  std::vector<Image *> images;
  ExplodingSprite* explosion;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  std::vector<Image *> rightsprite;
  std::vector<Image *> leftsprite;
  Facing facing;
  Vector2f initialVelocity;
  Vector2f initialPosition;
  std::list<MovingEnemy*> observers;
  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  Vector2f rightOffset;
  Vector2f leftOffset;
  BulletPool bullets;
  int flowersCollected;
  int enemiesDestroyed;
  int initialLives;
  int initialHealth;
  int currentLives;
  int currentHealth;
  int enemyCollisionHealthLoss;
  bool godMode;
  bool reachedTree;

  void advanceFrame(Uint32 ticks);
};
#endif
