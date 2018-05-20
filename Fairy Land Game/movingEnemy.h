#ifndef MOVINGENEMY_H
#define MOVINGENEMY_H

#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class MovingEnemy : public Drawable {
public:
	MovingEnemy(const std::string&, const Vector2f&, int, int);
	MovingEnemy(const MovingEnemy&);
	MovingEnemy& operator=(const MovingEnemy&);
	~MovingEnemy();

	virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return images[currentFrame]; }
  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  int getScaledWidth()  const { return getScale()*images[currentFrame]->getWidth(); }
  int getScaledHeight()  const { return getScale()*images[currentFrame]->getHeight(); }

	bool explosionDone();
  void explode();
	void reset();
	void removeFromScreen();
	void restartGame();
	void setPlayerPos(const Vector2f& p) { playerPos = p; }
	void moveToInitialPosition() { setPosition(initialPosition); }
	void gotShot() { ++bulletsHit; }
	bool isDead() { return bulletsToDie == bulletsHit; }
	void setRespawn(bool value) { respawn = value; }

private:
	enum MODE {NORMAL, PURSUE};
	std::vector<Image *> images;
  ExplodingSprite* explosion;
	unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
	Vector2f playerPos;
	int playerWidth;
	int playerHeight;
	MODE currentMode;
	float safeDistance;
	int viewWidth;
	int viewHeight;
	int worldWidth;
	int worldHeight;
	int enemyWidth;
	int enemyRange;
	unsigned int bulletsToDie;
	unsigned int bulletsHit;
	Vector2f initialPosition;
	Vector2f initialVelocity;
	bool respawn;

	void goUp(Uint32 ticks);
	void goDown(Uint32 ticks);
	void advanceFrame(Uint32 ticks);
};
#endif
