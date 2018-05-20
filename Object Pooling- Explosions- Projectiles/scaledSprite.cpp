#include<iostream>
#include<cmath>
#include<functional>
#include "scaledSprite.h"

ScaledSprite::ScaledSprite(const std::string& name,float sc1):
  MultiSprite(name,sc1),
  scale(sc1)
{}
inline namespace{
	constexpr float SCALE_EPSILON = 2e-7;
}

void ScaledSprite::draw() const{
	if(getScale()<SCALE_EPSILON) return;
	images[currentFrame]->draw(getX(),getY(),scale);
}
