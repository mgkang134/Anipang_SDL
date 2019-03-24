#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <ctime>
#include <map>
#include <Item.h>
#include <string>
using namespace std;


extern map<string, map<int, SDL_Texture*>>* imageLibrary;
extern SDL_Renderer* gRenderer;
extern SDL_Texture* gBackgroundTexture;
extern const int MAP_POSITION_X;
extern const int MAP_POSITION_Y;

Item::Item(int xPos, int yPos)
{
	exist = true;
	blacked = false;
	x = xPos;
	y = yPos;
	x_velocity, y_velocity = 0;
	x_before, y_before = 0;
	effectCounter = 0;
	iconType = rand() % 7 + 1;															//1~7까지의 랜덤한 정수
	texture = imageLibrary->find("icon_Library")->second.find(iconType)->second;		//생성시에는 Normal한 이미지를 대입
	rect.x = x;																			//blit을 위한 rect객체 초기화
	rect.y = y;
	rect.w = 70;
	rect.h = 70;
}

void Item::pangEffect() {
	if (effectCounter == 0) {
		imageReverse();
		effectCounter++;
	}
	else {
		imageGreen();
		effectCounter = 0;
	}
}

int Item::getX()
{
	return x;
}

int Item::getY()
{
	return y;
}

void Item::setX(int x) {
	this->x = x;
}

void Item::setY(int y) {
	this->y = y;
}

int Item::getIconType() {
	return iconType;
}

bool Item::getBlacked() {
	return blacked;
}

void Item::setExist(bool tf) {
	exist = tf;
}

void Item::setBlacked(bool tf) {
	blacked = tf;
}

void Item::randomIcon()
{
	iconType = rand() % 7 + 1;
	texture = imageLibrary->find("icon_Library")->second.find(iconType)->second;
}

bool Item::isExist()
{
	if (exist)
		return true;
	else
		return false;
}

void Item::getVelocity(int frame)
{
	x_velocity = float(x - x_before) / frame;
	y_velocity = float(y - y_before) / frame;
}

void Item::setXBefore()
{
	x_before = x;
}

void Item::setYBefore()
{
	y_before = y;
}

void Item::updateRect() {
	rect.x = x + MAP_POSITION_X;
	rect.y = y + MAP_POSITION_Y;
		
}

void Item::drawItem()
{
	updateRect();
	SDL_RenderCopy(gRenderer, texture, NULL, &rect);
}

void Item::removeItem()
{
	blacked = true;
}

void Item::newItem(int xPos, int yPos)		//인자로 받은 x, y값을 대입하고 아이콘 종류를 바꿈
{
	exist = true;
	blacked = false;
	x = xPos;
	y = yPos;
	x_velocity, y_velocity = 0;
	x_before, y_before = 0;
	effectCounter = 0;
	iconType = rand() % 7 + 1;
	texture = imageLibrary->find("icon_Library")->second.find(iconType)->second;

}

void Item::moveItem()
{
	x_before += x_velocity;
	y_before += y_velocity;
	x = x_before;
	y = y_before;
}

void Item::imageFocus()
{
	texture = imageLibrary->find("icon_focused_Library")->second.find(iconType)->second;
}

void Item::imageNormal()
{
	texture = imageLibrary->find("icon_Library")->second.find(iconType)->second;
}

void Item::imageReverse()
{
	texture = imageLibrary->find("icon_reverse_Library")->second.find(iconType)->second;
}

void Item::imageGreen()
{
	texture = imageLibrary->find("icon_green_Library")->second.find(iconType)->second;
}

void Item::image_0()
{
	texture = imageLibrary->find("icon_0_Library")->second.find(iconType)->second;
}

void Item::image_20()
{
	texture = imageLibrary->find("icon_20_Library")->second.find(iconType)->second;
}

void Item::image_40()
{
	texture = imageLibrary->find("icon_40_Library")->second.find(iconType)->second;
}

void Item::image_60()
{
	texture = imageLibrary->find("icon_60_Library")->second.find(iconType)->second;
}

void Item::image_80()
{
	texture = imageLibrary->find("icon_80_Library")->second.find(iconType)->second;
}

void Item::image_100()
{
	texture = imageLibrary->find("icon_100_Library")->second.find(iconType)->second;
}


