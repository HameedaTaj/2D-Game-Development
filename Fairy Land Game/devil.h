#ifndef DEVIL_H
#define DEVIL_H

#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "bulletPool.h"

class ExplodingSprite;

class Devil : public Drawable {
public:
	Devil(const std::string&, const std::string&);
	Devil(const Devil&);
	Devil& operator=(const Devil&);
  ~Devil();

	virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const { return images[currentFrame]; }
  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  int getScaledWidth()  const { return getScale()*images[currentFrame]->getWidth(); }
  int getScaledHeight()  const { return getScale()*images[currentFrame]->getHeight(); }

	void reset();
	void removeFromScreen();
	void restartGame();
	bool explosionDone();
  void explode();
	void setOriginalState() {
		setPosition(initialPosition);
		bulletsHit = 0;
	}
	void gotShot() { ++bulletsHit; }
	bool isDead() { return bulletsToDie == bulletsHit; }
	bool hasShot(Drawable* d) { return bullets.collided(d); };

private:
	std::vector<Image *> images;
  ExplodingSprite* explosion;
	unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
	int viewWidth;
	int viewHeight;
	int worldWidth;
	int worldHeight;
	int enemyWidth;
	int enemyRange;
	unsigned int bulletsToDie;
	unsigned int bulletsHit;
	Vector2f initialPosition;
	std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  Vector2f leftOffset;
  BulletPool bullets;

	void advanceFrame(Uint32 ticks);
	void shoot();
};
#endif
