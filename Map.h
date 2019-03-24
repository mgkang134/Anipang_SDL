#pragma once
#include <vector>
#include <Item.h>

using namespace std;

class Map
{
private:
	vector<vector<Item*>> itemList;		//7x7 vector array
	vector<vector<Item*>> reversedList;
	SDL_Rect rectForDigit;
	bool swapMiss;
	int reSwapDirection;
	int hintIndexRaw;
	int hintIndexCol;
	int hintIndexRaw2;
	int hintIndexCol2;
	int hintCounter;
	bool hint;
	int comboGauge;					
	int comboTimer;					//0 ~ 100 progress percent
	int comboThreshold;	
	int comboLevel;

	//각 동물들의 터뜨린 갯수를 저장하는 변수들
	int animalCounter[7];



public:
	Map();
	~Map();
	void resetComboLevel();
	void incleaseComboLevel();
	void drawComboLevel();
	void setComboTimer(unsigned int);
	void hintImageToNormal();
	void saveItemPosition();
	void drawAnimalCounterIcon();
	void drawComboTimer();
	void renderScreen();
	void moveItems(int);
	void getItemVelocity(int);
	void stack();
	bool canPang();
	void pangCombo();
	void pang();
	void drawText();
	void pangRaw();
	void pangColumn();
	void remove();
	void adjustCurrentPosition();
	SDL_Texture* selectTextureByNumber(int);
	void showAnimalCounter();
	void drawMap();
	Item* getItemByPosition(int, int);
	bool swapItemsPosition(int, int, int);
	void addAnimalCounter();
	void pangEffect(int);
	bool getHintPosition();
	void showHint();
};