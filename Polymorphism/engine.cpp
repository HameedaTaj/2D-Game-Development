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

Engine::~Engine() { 

  std::cout << "Terminating program" << std::endl;
  std::vector<Drawable*>::iterator it=sprites.begin();
  while(it!=sprites.end()){
    delete *it;
  ++it;
  }
 
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  building("building", Gamedata::getInstance().getXmlInt("building/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),
  makeVideo( false )
{
  int noofsprites=Gamedata::getInstance().getXmlInt("pinkbird/noofobj");
  sprites.push_back(new MultiSprite("SpinningStar"));
  sprites.push_back(new TwoWaySprite("bird"));
  for(int i=0;i<noofsprites;i++)
  {  sprites.push_back(new Sprite("pinkbird"));  }
 
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
  SDL_Color textColor1,textColor2;Clock c;
  sky.draw();
  building.draw();
  textColor1.r=255;textColor1.g=255;textColor1.b=0;
  textColor2.r=255;textColor2.g=255;textColor2.b=255;
   std::stringstream strm;
  strm<<"fps:"<<clock.getFps();	
  io.writeText(strm.str(),70,70,textColor1);
  io.writeText("htaj",30,450,textColor2);
  for(auto* s: sprites) s->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {

  for(auto* s:sprites) s->update(ticks);
  sky.update();
  building.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite=currentSprite% sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
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
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
