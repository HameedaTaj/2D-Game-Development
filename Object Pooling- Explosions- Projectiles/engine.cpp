#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "twowaysprite.h"


const SDL_Color color = {164,48,50,255};
const SDL_Color color1 = {240,232,112,255};

class ScaledSpritesInstances
{
  public:
    bool operator()(const Drawable* lhs, const Drawable* rhs) const {
	return lhs->getScale()<rhs->getScale();
     }
};

Engine::~Engine() { 

  std::cout << "Terminating program" << std::endl;
  for(auto& sp: scaledsprites)
	delete sp;
  std::vector<SmartSprite*>::iterator it=sprites.begin();
  while(it!=sprites.end()){
    delete *it;
  ++it;
  }
  
  //delete player;
  for(CollisionStrategy* strategy:strategies){
	delete strategy;
  }
 
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  mountain("mountain",Gamedata::getInstance().getXmlInt("mountain/factor")),
  
  building("building", Gamedata::getInstance().getXmlInt("building/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Fairy")),
  scaledsprites(),
  sprites(),
  strategies(),
  currentStrategy(0),
  collision(false),
  currentSprite(0),
  showHud(false),
  makeVideo( false )
  {
  int noofsprites=Gamedata::getInstance().getXmlInt("pinkbird/noofobj");
  sprites.reserve(noofsprites);
  Vector2f pos = player->getPosition();
  int w=player->getScaledWidth();
  int h=player->getScaledHeight();
  
  //push scaled sprites
  unsigned int n = Gamedata::getInstance().getXmlInt("snowflake/noOfObjects/instances");
  static float scale = Gamedata::getInstance().getXmlFloat("snowflake/scale/min");
  for(unsigned int i=0;i<n;++i)
  {
	float diff = Gamedata::getInstance().getXmlFloat("snowflake/scale/max")-Gamedata::getInstance().getXmlFloat("snowflake/scale/min");
	float incr = diff/n;
	scale = scale+incr;
	auto* s = new ScaledSprite("snowflake",scale);
	s->setScale(scale);
	scaledsprites.push_back(s);
  }
  std::vector<Drawable*>::iterator ptr = scaledsprites.begin();
  sort(ptr,scaledsprites.end(),ScaledSpritesInstances());

  //push smartsprites and attach it to player
  for(int i=0;i<noofsprites;i++)
  {  sprites.push_back(new SmartSprite("pinkbird",pos,w,h)); 
     player->attach(sprites[i]);
   }
 
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
  switchSprite();
  std::cout << "Loading complete" << std::endl;
}
std::string IntToString(int n)
{
	std::stringstream sstream;
	sstream<<n;
	return sstream.str();
}
void Engine::draw() const {
  SDL_Color textColor2;
  sky.draw();
  for(int i=0;i<((Gamedata::getInstance().getXmlInt("snowflake/noOfObjects/instances"))/2);i++) scaledsprites[i]->draw();
  mountain.draw();
  for(int i=6;i<(Gamedata::getInstance().getXmlInt("snowflake/noOfObjects/instances"));i++) scaledsprites[i]->draw();
  building.draw();
  player->draw();
  if(showHud){
  Hud::getInstance().displayHud();}
  IoMod::getInstance().writeText("Press m to change strategy", 460, 70,color1);
  std::stringstream strm1,strm2;
  strm1 << sprites.size() << " Smart Sprites Remaining";
  IoMod::getInstance().writeText(strm1.str(), 460, 10,color1);
  strategies[currentStrategy]->draw();
  if ( collision ) {
    IoMod::getInstance().writeText("Oops: Collision", 460, 90,color1);
  }

  //textColor1.r=0;textColor1.g=0;textColor1.b=0;
  textColor2.r=255;textColor2.g=255;textColor2.b=255;
  strm2<<"Frames per sec:"<<clock.getFps();	
  io.writeText(strm2.str(),30,100,color1);
  io.writeText("Hameeda Taj",30,450,textColor2);
  for(auto* s: sprites) s->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}
void Engine::checkForCollisions() {
  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  for(auto* s:scaledsprites)s->update(ticks);
  for(auto* s:sprites) s->update(ticks);
  player->update(ticks);
  sky.update(); 
  mountain.update();
  building.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite=currentSprite% sprites.size();
  Viewport::getInstance().setObjectToTrack(player);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }

        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      if(keystate[SDL_SCANCODE_F1]){
	showHud=!showHud;
	}
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
