#pragma once
#include <SDL.h>
#include <string>
#include <map>

class Item 
{
private:
	bool exist;								//pang ���� ���Ŀ� �ش� ���� �����ִ°� ����
	bool blacked;							//remove ���� ���� ���� ��ĭ�� �Ǿ� �ִ°� ����
	bool hintOn;							//��Ʈ�� �������� ����
	bool hintCounter;						//��Ʈ�� ��Ÿ��������� �ð��� ���� ���� ī����
	float x, y, x_before, y_before;			//�������� x,y ��ǥ
	float x_velocity, y_velocity;				//�������� �ӵ�
	int effectCounter;						//����Ʈ�� ������ ��Ÿ���� ���� ī����
	int iconType;							//���� �������� ������ Ÿ��(1~7)
	SDL_Texture* texture;
	SDL_Rect rect;							// rect��ü ����
	
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