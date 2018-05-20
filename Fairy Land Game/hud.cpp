#include "hud.h"
#include "ioMod.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "renderContext.h"
#include <sstream>

Hud& Hud::getInstance(Player* player){
    static Hud instance(player);
    return instance;
}

Hud::~Hud() {}

Hud::Hud(Player* player):
    player(player),
    sound(Gamedata::getInstance().getSoundInstance()),
    heartImage(RenderContext::getInstance()->getImage("hudHeart")),
    flowerImage(RenderContext::getInstance()->getImage("hudFlower")),
    deadGreenEnemy(RenderContext::getInstance()->getImages("hudDeadGreenEnemy")),
    deadRedEnemy(RenderContext::getInstance()->getImages("hudDeadRedEnemy")),
    width(Gamedata::getInstance().getXmlInt("hud/width")),
    height(Gamedata::getInstance().getXmlInt("hud/height")),
    hudx(Gamedata::getInstance().getXmlInt("hud/start/x")),
    hudy(Gamedata::getInstance().getXmlInt("hud/start/y")),
    hudRect({hudx,hudy,width,height}),
    renderer( RenderContext::getInstance()->getRenderer() ),
    io( IOmod::getInstance() ),
    textFontColor({255,255,255,255})
{}

void Hud::drawHealthBar(int x, int y, int w, int h, float percent) const {
  if ( player->isInvincible() ) {
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
    SDL_Rect fgrect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &fgrect);
    IOmod::getInstance().writeText("GOD MODE", hudx+120, hudy+17, textFontColor, 14);
  } else {
    SDL_Rect bgrect = { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgrect);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200);
    int pw =  (int)((float)w * (percent/100));
    SDL_Rect fgrect = { x, y, pw, h };
    SDL_RenderFillRect(renderer, &fgrect);
  }
}

void Hud::drawMusicStatus(int x, int y) const {
    SDL_Color tColor = textFontColor;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if (sound->isBackgroundMusicOn()) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        tColor = {0, 0, 0, 255};
    }
    SDL_Rect fgrect = { x, y, 50, 20 };
    SDL_RenderFillRect(renderer, &fgrect);
    IOmod::getInstance().writeText("MUSIC", x+7, y+2, tColor, 14);
}

void Hud::drawSFXStatus(int x, int y) const {
    SDL_Color tColor = textFontColor;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if (sound->areSoundEffectsOn()) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        tColor = {0, 0, 0, 255};
    }
    SDL_Rect fgrect = { x, y, 40, 20 };
    SDL_RenderFillRect(renderer, &fgrect);
    IOmod::getInstance().writeText("SFX", x+10, y+2, tColor, 14); 
}

void Hud::draw(const Vector2f& position) const {
    std::stringstream strm;
   
    heartImage->draw(position[0]+640, position[1]+10, 0.25);
    if (player->isInvincible()) {
      strm << "G";
    } else {
      strm << player->getRemainingLives();
    }
    IOmod::getInstance().writeText(strm.str(), 630, 15, textFontColor, 20);

    drawHealthBar(670, 15, 150, 20, player->getCurrentHealth()*100/player->getTotalHealth());

     SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
     SDL_SetRenderDrawColor( renderer, 100, 100, 100, 200 );
     SDL_RenderFillRect( renderer, &hudRect );
     SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
     SDL_RenderDrawRect( renderer, &hudRect );

    flowerImage->draw(position[0]+20, position[1]+hudy+5, 1);
    strm.clear();
    strm.str("");
    strm << player->getNumFlowersCollected();
    IOmod::getInstance().writeText(strm.str(), hudx+10, hudy+15, textFontColor, 20);

    deadGreenEnemy[0]->draw(position[0]+80, position[1]+hudy+10, 0.8);
    deadRedEnemy[0]->draw(position[0]+130, position[1]+hudy+5, 0.8);
    strm.clear();
    strm.str("");
    strm << player->getNumEnemiesDestroyed();
    IOmod::getInstance().writeText(strm.str(), 70, hudy+15, textFontColor, 20);

  
    IOmod::getInstance().writeText("Hameeda Taj", hudx+640, hudy+15, textFontColor, 18);
}
