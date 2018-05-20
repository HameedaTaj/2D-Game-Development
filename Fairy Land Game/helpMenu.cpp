#include "helpMenu.h"
#include "ioMod.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "renderContext.h"
#include <sstream>

HelpMenu& HelpMenu::getInstance(){
    static HelpMenu instance;
    return instance;
}

HelpMenu::~HelpMenu() {}

HelpMenu::HelpMenu():
    width(Gamedata::getInstance().getXmlInt("helpMenu/width")),
    height(Gamedata::getInstance().getXmlInt("helpMenu/height")),
    x(Gamedata::getInstance().getXmlInt("helpMenu/start/x")),
    y(Gamedata::getInstance().getXmlInt("helpMenu/start/y")),
    rect({x,y,width,height}),
    renderer( RenderContext::getInstance()->getRenderer() ),
    io( IOmod::getInstance() ),
    textFontColor({0, 0, 0, 255})
{}

void HelpMenu::draw() const {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 247, 198, 250, 100 );
    SDL_RenderFillRect( renderer, &rect );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, &rect );
    IOmod::getInstance().writeText("Save the little princess to win!!!", x+15, y+30,textFontColor, 16);
    IOmod::getInstance().writeText("CONTROLS", x+90, y+10, textFontColor, 18);
    IOmod::getInstance().writeText("WASD .......Movement", x+15, y+50,textFontColor, 16);
    IOmod::getInstance().writeText("R ......... Restart Game", x+15, y+70,textFontColor, 16);
    IOmod::getInstance().writeText("G ......... God Mode", x+15, y+90,textFontColor, 16);
    IOmod::getInstance().writeText("ESC ....... Quit Game", x+15, y+110,textFontColor, 16);
}
