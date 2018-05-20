#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "helpMenu.h"
#include "gameOverBox.h"
#include "movingEnemy.h"
#include "devil.h"
#include "baby.h"
#include "sound.h"
#include <vector>

class Player;
class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  unsigned int helpMenuTime;
  World sky;
  World mountains;
  World building;
  Viewport& viewport;

  Player* player;
  Devil* devil;
  Baby* baby;
  std::vector<Drawable* > bigButterflies;
  std::vector<Drawable* > smallButterflies;
  std::vector<Drawable* > flowers;
  std::vector<Drawable* > enemies;
  std::vector<CollisionStrategy*> strategies;
  int currentSprite;
  int currentStrategy;
  bool showHelpMenu;

  Hud& hud;
  HelpMenu& helpMenu;
  GameOverBox& gameOverBox;

  SDLSound* sound;

  bool makeVideo;
  bool gameOverSoundPlayed;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
