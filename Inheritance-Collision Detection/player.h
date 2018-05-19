#ifndef PLAYER__H
#define PLAYER__H

#include "multisprite.h"
#include <string>
#include <list>
#include <cmath>

// In this example the player is derived from MultiSprite.
// However, there are many options.
class SmartSprite;
class Player : public MultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);
  void attach(SmartSprite* o){ observers.push_back(o);}
  void detach(SmartSprite* o);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();
private:
  bool collision;
  Vector2f initialVelocity;
  std::list<SmartSprite*> observers;
};
#endif
