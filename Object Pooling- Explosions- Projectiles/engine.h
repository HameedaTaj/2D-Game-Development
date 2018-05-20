#include <vector>
#include "renderContext.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "collisionStrategy.h"
#include "smartSprite.h"
#include "hud.h"
#include "scaledSprite.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

  Engine(const Engine&)=delete;
  Engine& operator=(const Engine&)=delete;

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World sky;
  World mountain;
  World building;
  Viewport& viewport;
  Player* player;
  std::vector<Drawable*> scaledsprites;
  std::vector<SmartSprite*> sprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;
  
  int currentSprite;
  bool showHud;
  bool makeVideo;
   void draw() const;
  void update(Uint32);
  void printScales() const;
  void checkForCollisions();
};
