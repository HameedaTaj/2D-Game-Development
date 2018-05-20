#ifndef SCALEDSPRITE_H
#define SCALEDSPRITE_H
#include<string>
#include<vector>
#include<cmath>
#include "multisprite.h"

class ScaledSprite: public MultiSprite
{
  public:
	ScaledSprite(const std::string&,float);
	virtual void draw() const;
	inline float getScale() const{return scale;}
	void setScale(float s){ scale=s;}
  private:
	float scale;
};
#endif
