#include <Map.h>
#include <Item.h>
#include <main.h>
#include <algorithm>
using namespace std;

extern map<string, map<int, SDL_Texture*>>* imageLibrary;
extern SDL_Texture* gBackgroundTexture;
extern SDL_Renderer* gRenderer;
extern SDL_Window* gWindow;
extern SDL_Rect* backgroundRect;
extern SDL_Texture* gScreenBackgroundTexture;


extern SDL_Texture* gComboTimerBar;
extern SDL_Texture* gComboTimerBarEmpty;
extern SDL_Texture* gComboText;
extern SDL_Texture* gComboTimerText;
extern SDL_Texture* gComboIcon;

extern SDL_Event e;

//digit 텍스쳐

extern SDL_Texture* gDigit0;
extern SDL_Texture* gDigit1;
extern SDL_Texture* gDigit2;
extern SDL_Texture* gDigit3;
extern SDL_Texture* gDigit4;
extern SDL_Texture* gDigit5;
extern SDL_Texture* gDigit6;
extern SDL_Texture* gDigit7;
extern SDL_Texture* gDigit8;
extern SDL_Texture* gDigit9;


extern const int ITEM_WIDTH;
extern const int ITEM_HEIGHT;
extern const int SCREEN_TICKS_PER_FRAME;
extern const int SCREEN_FPS;
extern const int MAP_SIZE;
extern unsigned int COMBO_DELAY;

extern void close();

Map::Map()
{
	SDL_QueryTexture(gDigit0, NULL, NULL, &rectForDigit.w, &rectForDigit.h);
	swapMiss = false;
	reSwapDirection = 0;
	hintIndexRaw = 0;
	hintIndexCol = 0;
	hintIndexRaw2 = 0;
	hintIndexCol2 = 0;
	hintCounter = 0;
	comboGauge = 0;
	comboTimer = 0;
	comboThreshold = 0;
	comboLevel = 0;


	for (int i = 0; i < MAP_SIZE; i++) {			//동물 카운터 배열 0으로 초기화
		animalCounter[i] = 0;
	}



	for (int i = 0; i < MAP_SIZE;i++) {
		vector<Item*> subVector;
		for (int j = 0;j < MAP_SIZE;j++) {
			Item* item = new Item();
			subVector.push_back(item);
		}
		itemList.push_back(subVector);
	}

	for (int i = 0;i < MAP_SIZE;i++) {
		vector<Item*> subVector;
		for (int j = 0;j < MAP_SIZE;j++) {
			subVector.push_back(itemList[j][i]);
		}
		reversedList.push_back(subVector);
	}

	//초기화를 위해 현재 가능한 타일들은 모두 터뜨림
	while (canPang()) {
		pang();
		remove();
		stack();
		adjustCurrentPosition();
	}

}

Map::~Map()
{
	for (auto &i : itemList) {
		for (auto &j : i) {
			delete j;
		}
	}
}

void Map::resetComboLevel()
{
	comboLevel = 0;
	COMBO_DELAY = 2000;
}

void Map::incleaseComboLevel()
{
	
	if (comboLevel < 5) {			//콤보레벨은 최대 5이다.
		comboLevel++;
		switch (comboLevel) {
		case 3:
			COMBO_DELAY -= 500;
			break;
		case 4:
			COMBO_DELAY -= 500;
		}
		COMBO_DELAY;
	}
}

void Map::drawComboLevel() {
	SDL_Rect rect;
	int yStep = 0;
	rect.w = 70;
	rect.h = 70;

	for (int i = 0; i < comboLevel; i++) {
		rect.x = MAP_POSITION_X - 80;
		rect.y = MAP_POSITION_Y + yStep;
		SDL_RenderCopy(gRenderer, gComboIcon, NULL, &rect);

		yStep += 90;
	}
}

void Map::setComboTimer(unsigned int time)		//time -> 0 ~ COMBO_DELAY(2000) ms
{
		comboTimer = (float)time / COMBO_DELAY * 100;					//comboTimer값은 퍼센트 수치이다.
}

void Map::hintImageToNormal()
{
	itemList[hintIndexRaw][hintIndexCol]->imageNormal();
	itemList[hintIndexRaw2][hintIndexCol2]->imageNormal();
}


void Map::saveItemPosition()
{
	for (int i = 0; i < MAP_SIZE;i++) {
		for (int j = 0;j < MAP_SIZE;j++) {
			itemList[i][j]->setXBefore();
			itemList[i][j]->setYBefore();
		}
	}
}

void Map::drawAnimalCounterIcon() {

	SDL_Texture* tempTexture = NULL;
	SDL_Rect rect;
	rect.w = ITEM_WIDTH;
	rect.h = ITEM_HEIGHT;
	int yStep = 0;

	for (int i = 0; i < MAP_SIZE; i++) {

		rect.x = MAP_WIDTH + MAP_POSITION_X + 200 + 30;
		rect.y = MAP_POSITION_Y + yStep;

		tempTexture = imageLibrary->find("icon_Library")->second.find(i + 1)->second;

		SDL_RenderCopy(gRenderer, tempTexture, NULL, &rect);
		yStep += 70;
	}


}

void Map::drawComboTimer() {
	SDL_Rect rect;
	rect.x = MAP_WIDTH + MAP_POSITION_X + 20;			//x, y값은 동일
	rect.y = MAP_POSITION_Y;
	rect.w = 49;
	rect.h = 490;
	SDL_RenderCopy(gRenderer, gComboTimerBar, NULL, &rect);
	rect.w = 49;
	rect.h = (490 * comboTimer / 100);
	SDL_RenderCopy(gRenderer, gComboTimerBarEmpty, NULL, &rect);
}

void Map::renderScreen() {

	SDL_RenderCopy(gRenderer, gScreenBackgroundTexture, NULL, NULL);
	SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, backgroundRect);
	drawMap();
	showAnimalCounter();
	drawAnimalCounterIcon();
	drawComboTimer();
	drawComboLevel();
	drawText();
	SDL_RenderPresent(gRenderer);
	SDL_Delay(SCREEN_TICKS_PER_FRAME);
}

void Map::moveItems(int	interval)
{
	for (int k = 0; k < interval; k++) {
		for (int i = 0; i < MAP_SIZE;i++) {
			for (int j = 0;j < MAP_SIZE;j++) {
				itemList[i][j]->moveItem();
			}
		}
		SDL_RenderClear(gRenderer);
		extern SDL_Texture* gDigit0;

		//render
		renderScreen();
	}
}

void Map::getItemVelocity(int frame)
{
	for (int i = 0; i < MAP_SIZE;i++) {
		for (int j = 0;j < MAP_SIZE;j++) {
			itemList[i][j]->getVelocity(frame);
		}
	}

}

void Map::stack()
{
	vector<Item*> changedItemList;				//새로 내려오는 블록들을 저장하는 벡터

	for (int raw = 0; raw < MAP_SIZE; raw++) {			//reverse된 이중벡터에서 스택을 실행.	
		vector<Item*>::iterator iter;
		Item* tempItem;
		int depth = 0;
		for (int col = 0; col < MAP_SIZE; col++) {
			if (!reversedList[raw][col]->isExist()) {
				changedItemList.push_back(reversedList[raw][col]);

				iter = reversedList[raw].begin();
				tempItem = reversedList[raw][col];
				tempItem->newItem(tempItem->getX(), -ITEM_WIDTH * (depth + 1));
				depth++;
				reversedList[raw].erase(iter + col);
				iter = reversedList[raw].begin();
				reversedList[raw].insert(iter, tempItem);
				changedItemList.push_back(tempItem);
			}
		}
	}

	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0;j < MAP_SIZE; j++) {
			itemList[i][j] = reversedList[j][i];
		}
	}

	while (!getHintPosition()) {			//맞출 수 있는 타일이 없을 경우에 계속 반복.
		printf("no hint!!!!\n");
		for (Item* item : changedItemList) {
			item->randomIcon();
		}
	}




}

bool Map::canPang() {


	for (auto &itemRaw : itemList) {
		for (int i = 0; i < MAP_SIZE - 2;i++) {
			if (itemRaw[i]->getIconType() == itemRaw[i + 1]->getIconType() &&
				itemRaw[i + 1]->getIconType() == itemRaw[i + 2]->getIconType()) {
				if (!itemRaw[i]->getBlacked() && !itemRaw[i + 1]->getBlacked() && !itemRaw[i + 2]->getBlacked()) {
					return true;
				}
			}
		}
	}

	for (auto &itemRaw : reversedList) {
		for (int i = 0; i < MAP_SIZE - 2;i++) {
			if (itemRaw[i]->getIconType() == itemRaw[i + 1]->getIconType() &&
				itemRaw[i + 1]->getIconType() == itemRaw[i + 2]->getIconType()) {
				if (!itemRaw[i]->getBlacked() && !itemRaw[i + 1]->getBlacked() && !itemRaw[i + 2]->getBlacked()) {
					return true;
				}
			}
		}
	}
	return false;
}

void Map::pangCombo() {															//콤보시스템이 추가된 pang

	vector<int> indexRawVector;
	vector<int> indexColVector;

	int counter = comboLevel - 2;

	for (int i = 0; i < counter; i++) {											//3레벨부터 루프에 들어감

		if (i == 2)																//5레벨인 경우 처음루프는 무시(2번만 수행)
			continue;

		for (int raw = 0; raw < MAP_SIZE; raw++) {
			for (int col = 0; col < MAP_SIZE; col++) {
				if (!itemList[raw][col]->isExist()) {							//exist가 black인 블럭에 한해서,
					indexRawVector.push_back(raw);								//인덱스값도 넣어둠
					indexColVector.push_back(col);

				}
			}
		}

		printf("%d", indexRawVector.size());

		for (int i = 0;i < indexRawVector.size(); i++) {
			int raw = indexRawVector[i];
			int col = indexColVector[i];
			if (col - 1 >= 0)
				itemList[raw][col - 1]->setExist(false);
			if (col + 1 < 7)
				itemList[raw][col + 1]->setExist(false);
			if (raw - 1 >= 0)
				itemList[raw - 1][col]->setExist(false);
			if (raw + 1 < 7)
				itemList[raw + 1][col]->setExist(false);
		}

		indexRawVector.clear();
		indexColVector.clear();
		if (comboLevel == 3 || comboLevel == 4)								//3, 4레벨의 경우 한번만 수행
			break;
		
	}
}

void Map::pang()
{
	pangRaw();
	pangColumn();
	pangCombo();
}

void Map::drawText() {

	SDL_Rect rect;
	SDL_QueryTexture(gComboText, NULL, NULL, &rect.w, &rect.h);
	rect.x = MAP_POSITION_X - 100;			//콤보 텍스트
	rect.y = MAP_POSITION_Y - 45;
	
	SDL_RenderCopy(gRenderer, gComboText, NULL, &rect);

	SDL_QueryTexture(gComboTimerText, NULL, NULL, &rect.w, &rect.h);
	rect.x = MAP_POSITION_X + MAP_WIDTH - 50;
	rect.y = MAP_POSITION_Y - 45;

	SDL_RenderCopy(gRenderer, gComboTimerText, NULL, &rect);
	
}

void Map::pangRaw()
{
	for (auto &itemRaw : itemList) {
		for (int i = 0; i < MAP_SIZE - 2;i++) {
			if (itemRaw[i]->getIconType() == itemRaw[i + 1]->getIconType() &&
				itemRaw[i + 1]->getIconType() == itemRaw[i + 2]->getIconType()) {
				if (!itemRaw[i]->getBlacked() && !itemRaw[i + 1]->getBlacked() && !itemRaw[i + 2]->getBlacked()) {
					itemRaw[i]->setExist(false);
					itemRaw[i + 1]->setExist(false);
					itemRaw[i + 2]->setExist(false);
				}
			}
		}
	}
}

void Map::pangColumn()
{

	for (auto &itemRaw : reversedList) {
		for (int i = 0; i < MAP_SIZE - 2;i++) {
			if (itemRaw[i]->getIconType() == itemRaw[i + 1]->getIconType() &&
				itemRaw[i + 1]->getIconType() == itemRaw[i + 2]->getIconType()) {
				if (!itemRaw[i]->getBlacked() && !itemRaw[i + 1]->getBlacked() && !itemRaw[i + 2]->getBlacked()) {
					itemRaw[i]->setExist(false);
					itemRaw[i + 1]->setExist(false);
					itemRaw[i + 2]->setExist(false);
				}
			}
		}
	}

}

void Map::remove()
{
	for (int i = 0; i < MAP_SIZE;i++) {
		for (int j = 0;j < MAP_SIZE;j++) {
			if (!itemList[i][j]->isExist())
				itemList[i][j]->removeItem();
		}
	}
}

void Map::adjustCurrentPosition()				//배열상의 위치에 맞게 실제 아이템들의 x,y값을 바꾸어주는 함수.
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < MAP_SIZE;i++) {
		for (int j = 0;j < MAP_SIZE;j++) {
			itemList[i][j]->setX(x);
			itemList[i][j]->setY(y);
			x += ITEM_WIDTH;
		}
		x = 0;
		y += ITEM_HEIGHT;
	}
}

SDL_Texture* Map::selectTextureByNumber(int digit) {
	switch (digit) {
	case 0:
		return gDigit0;
		break;
	case 1:
		return gDigit1;
		break;
	case 2:
		return gDigit2;
		break;
	case 3:
		return gDigit3;
		break;
	case 4:
		return gDigit4;
		break;
	case 5:
		return gDigit5;
		break;
	case 6:
		return gDigit6;
		break;
	case 7:
		return gDigit7;
		break;
	case 8:
		return gDigit8;
		break;
	case 9:
		return gDigit9;
		break;
	}
}

void Map::showAnimalCounter() {

	SDL_Texture* selectedTexture = NULL;
	int digit;
	int xStep = 0;
	int yStep = 0;

	for (int i = 0; i < MAP_SIZE; i++) {

		string countString = to_string(animalCounter[i]);
		reverse(countString.begin(), countString.end());

		for (auto &i : countString) {
			digit = i - '0';
			selectedTexture = selectTextureByNumber(digit);

			rectForDigit.x = MAP_WIDTH + MAP_POSITION_X + 200 - xStep;
			rectForDigit.y = MAP_POSITION_Y + yStep + 15;

			SDL_RenderCopy(gRenderer, selectedTexture, NULL, &rectForDigit);
			xStep += 20;
		}
		xStep = 0;
		yStep += 70;
	}
}

void Map::drawMap()
{
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			itemList[i][j]->drawItem();
		}
	}
}


Item* Map::getItemByPosition(int x, int y)
{
	return itemList[y / ITEM_HEIGHT][x / ITEM_WIDTH];
}

bool Map::swapItemsPosition(int x1, int y1, int direction)
{
	int x2 = 0;
	int y2 = 0;

	switch (direction) {								//받은 방향에 따라 타겟 아이템의 인덱스를 변경
	case Direction::RIGHT:
		x2 = x1;
		y2 = y1 + 1;
		break;
	case Direction::LEFT:
		x2 = x1;
		y2 = y1 - 1;
		break;
	case Direction::UP:
		x2 = x1 - 1;
		y2 = y1;
		break;
	case Direction::DOWN:
		x2 = x1 + 1;
		y2 = y1;
		break;
	default:
		break;
	}

	if (x2 == -1 || x2 == MAP_SIZE)				//타겟 아이템 인덱스가 맵을 벗어나는 경우 false를 리턴
		return false;
	if (y2 == -1 || y2 == MAP_SIZE)
		return false;

	swap(itemList[x1][y1], itemList[x2][y2]);
	swap(reversedList[y1][x1], reversedList[y2][x2]);
	return true;
}

void Map::addAnimalCounter() {
	for (int raw = 0; raw < MAP_SIZE; raw++) {
		for (int col = 0; col < MAP_SIZE; col++) {
			if (!itemList[raw][col]->isExist()) {		//exist가 false인 아이템(터진 아이템)에 대해서 카운트를 증가시킴
				animalCounter[itemList[raw][col]->getIconType() - 1]++;
			}
		}
	}
}

void Map::pangEffect(int interval)
{
	for (int i = 0; i < interval; i++) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				close();
				exit(0);
			}
		}
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				if (!itemList[i][j]->isExist()) {
					itemList[i][j]->pangEffect();
				}
			}
		}
		renderScreen();
	}
}


bool Map::getHintPosition()
{
	for (int raw = 0; raw < MAP_SIZE; raw++) {
		for (int col = 0; col < MAP_SIZE; col++) {

			swapItemsPosition(raw, col, RIGHT);			//오른쪽으로 스왑해보고
			if (canPang()) {							//pang이 가능하면 힌트로 적용하고 빠져나감
				hintIndexRaw = raw;
				hintIndexCol = col;
				hintIndexRaw2 = raw;
				hintIndexCol2 = col + 1;

				swapItemsPosition(raw, col, RIGHT);
				return true;
			}
			else {										//Pang이 불가능하면 다시 되돌림
				swapItemsPosition(raw, col, RIGHT);
			}

			swapItemsPosition(raw, col, DOWN);			//아래쪽으로 스왑해보고
			if (canPang()) {							//pang이 가능하면 힌트로 적용하고 빠져나감
				hintIndexRaw = raw;
				hintIndexCol = col;
				hintIndexRaw2 = raw + 1;
				hintIndexCol2 = col;
				swapItemsPosition(raw, col, DOWN);
				return true;
			}
			else {										//Pang이 불가능하면 다시 되돌림
				swapItemsPosition(raw, col, DOWN);
			}
		}
	}
	return false;
}

void Map::showHint()
{
	static int hintDrawCounter = 0;
	Item* hintItem = itemList[hintIndexRaw][hintIndexCol];
	Item* hintItem2 = itemList[hintIndexRaw2][hintIndexCol2];

	if (SCREEN_FPS / 10 * 0 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 1) {
		hintItem->image_0();
		hintItem2->image_0();
	}
	if (SCREEN_FPS / 10 * 1 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 2) {
		hintItem->image_20();
		hintItem2->image_20();
	}
	if (SCREEN_FPS / 10 * 2 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 3) {
		hintItem->image_40();
		hintItem2->image_40();
	}
	if (SCREEN_FPS / 10 * 3 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 4) {
		hintItem->image_60();
		hintItem2->image_60();
	}
	if (SCREEN_FPS / 10 * 4 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 5) {
		hintItem->image_80();
		hintItem2->image_80();
	}
	if (SCREEN_FPS / 10 * 5 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 6) {
		hintItem->image_100();
		hintItem2->image_100();
	}
	if (SCREEN_FPS / 10 * 6 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 7) {
		hintItem->image_80();
		hintItem2->image_80();
	}
	if (SCREEN_FPS / 10 * 7 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 8) {
		hintItem->image_60();
		hintItem2->image_60();
	}
	if (SCREEN_FPS / 10 * 8 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 9) {
		hintItem->image_40();
		hintItem2->image_40();
	}
	if (SCREEN_FPS / 10 * 9 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 10) {
		hintItem->image_20();
		hintItem2->image_20();
	}

	hintDrawCounter++;
	if (hintDrawCounter == SCREEN_FPS)
		hintDrawCounter = 0;



}
