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
