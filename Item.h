#pragma once
#include <SDL.h>
#include <string>
#include <map>

class Item 
{
private:
	bool exist;								//pang 수행 이후에 해당 블럭이 남아있는가 여부
	bool blacked;							//remove 수행 이후 검은 빈칸이 되어 있는가 여부
	bool hintOn;							//힌트가 가능한지 여부
	bool hintCounter;						//힌트가 나타나기까지의 시간을 세기 위한 카운터
	float x, y, x_before, y_before;			//아이템의 x,y 좌표
	float x_velocity, y_velocity;				//아이템의 속도
	int effectCounter;						//이펙트의 순번을 나타내기 위한 카운터
	int iconType;							//현재 아이템의 아이콘 타입(1~7)
	SDL_Texture* texture;
	SDL_Rect rect;							// rect객체 선언
	
public:
	Item(int = 0, int = 0);
	void pangEffect();
	int getX();
	int getY();
	void setX(int);
	void setY(int);
	int getIconType();
	bool getBlacked();
	void setExist(bool);
	void setBlacked(bool);
	void randomIcon();
	bool isExist();
	void getVelocity(int);
	void drawItem();
	void removeItem();
	void newItem(int, int);
	void moveItem();
	void imageFocus();
	void imageNormal();
	void imageReverse();
	void imageGreen();
	void image_0();
	void image_20();
	void image_40();
	void image_60();
	void image_80();
	void image_100();
	void setXBefore();
	void setYBefore();

	void updateRect();


};