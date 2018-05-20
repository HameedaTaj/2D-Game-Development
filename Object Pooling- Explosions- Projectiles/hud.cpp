#include "hud.h"
#include<iostream>
#include<string>
#include<iomanip>
#include "gamedata.h"
#include "renderContext.h"
#include <sstream>

Hud& Hud::getInstance(){
 	static Hud instance;
	return instance;
}

Hud::Hud():
	width(Gamedata::getInstance().getXmlInt("hud/width")),
	height(Gamedata::getInstance().getXmlInt("hud/height")),
 	x(Gamedata::getInstance().getXmlInt("hud/start/x")),
	y(Gamedata::getInstance().getXmlInt("hud/start/y")),
	Rectangle({x,y,width,height}),
	renderer(RenderContext::getInstance()->getRenderer()),
	clock( Clock::getInstance() )

{}

Hud::~Hud()
{}

void Hud::displayHud() const{

	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer,255,255,255,255/2);
	SDL_RenderFillRect(renderer,&Rectangle);
	SDL_SetRenderDrawColor(renderer,0,0,255,255);
	SDL_RenderDrawRect(renderer,&Rectangle);
	SDL_Color Textcolor = {255,0,0,0};
	IoMod::getInstance().writeText("  Player Controls:",x+15,y+10,Textcolor);
	IoMod::getInstance().writeText("		a:   <",x+15,y+40,Textcolor);
	IoMod::getInstance().writeText("		d:   >",x+15,y+70,Textcolor);
	IoMod::getInstance().writeText("		w:   ^",x+15,y+100,Textcolor);
	IoMod::getInstance().writeText("		s:    v",x+15,y+130,Textcolor);
}
void Hud::drawPool(SDL_Renderer * const renderer, unsigned int bulletlistSize, unsigned int freelistSize){
	 SDL_Rect box;
  box.x = Gamedata::getInstance().getXmlInt("hud/Pool/posX");
  box.y = Gamedata::getInstance().getXmlInt("hud/Pool/posY");
  box.w = Gamedata::getInstance().getXmlInt("hud/Pool/width");
  box.h = Gamedata::getInstance().getXmlInt("hud/Pool/height");
  int borderThickness = Gamedata::getInstance().getXmlInt("hud/border/thickness");
  SDL_Color borderColor;
  borderColor.r = Gamedata::getInstance().getXmlInt("hud/border/Color/red");
  borderColor.g = Gamedata::getInstance().getXmlInt("hud/border/Color/green");
  borderColor.b = Gamedata::getInstance().getXmlInt("hud/border/Color/blue");
  borderColor.a = Gamedata::getInstance().getXmlInt("hud/border/Color/alpha");
  //SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
  for(int i =1; i<=borderThickness; i++){
    SDL_RenderDrawRect(renderer, &box);
    box.x += 1;
    box.y += 1;
    box.w -= 2;
    box.h -= 2;
  }
  SDL_Color background;
  background.r = Gamedata::getInstance().getXmlInt("hud/hudColor/red");
  background.g = Gamedata::getInstance().getXmlInt("hud/hudColor/green");
  background.b = Gamedata::getInstance().getXmlInt("hud/hudColor/blue");
  background.a = Gamedata::getInstance().getXmlInt("hud/hudColor/alpha");

  SDL_Color textColor;
  textColor.r =Gamedata::getInstance().getXmlInt("hud/textColor/red");
  textColor.g = Gamedata::getInstance().getXmlInt("hud/textColor/green");
  textColor.b = Gamedata::getInstance().getXmlInt("hud/textColor/blue");
  textColor.a = Gamedata::getInstance().getXmlInt("hud/textColor/alpha");

  //Gamedata::getInstance()SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
  SDL_RenderFillRect(renderer, &box);
	std::stringstream free;
	free<<"FreeList:\t"<<freelistSize;
	IoMod::getInstance().writeText(free.str(),box.x+=5,box.y+=5,textColor);
	std::stringstream bullets;
	bullets<<"BulletList:\t"<<bulletlistSize;
	IoMod::getInstance().writeText(bullets.str(),box.x,box.y+=30,textColor);
}
