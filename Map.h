#pragma once
#include <vector>
#include <Item.h>

using namespace std;

class Map
{
private:
	vector<vector<Item*>> itemList;		//7x7 vector array
	vector<vector<Item*>> reversedList;
	bool swapMiss;
	int reSwapDirection;
	int hintIndexRaw;
	int hintIndexCol;
	int hintIndexRaw2;
	int hintIndexCol2;
	int hintCounter;
	bool hint;

public:
	Map();
	~Map();
	void hintImageToNormal();
	void saveItemPosition();
	void moveItems(int);
	void getItemVelocity(int);
	void stack();
	bool canPang();
	void pang();
	void pangRaw();
	void pangColumn();
	void remove();
	void adjustCurrentPosition();
	void drawMap();
	Item* getItemByPosition(int, int);
	bool swapItemsPosition(int, int, int);
	void pangEffect(int);
	bool getHintPosition();
	void showHint();
};