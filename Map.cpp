#include <Map.h>
#include <Item.h>
#include <main.h>

using namespace std;

extern SDL_Texture* gBackgroundTexture;
extern SDL_Renderer* gRenderer;
extern SDL_Surface* gScreenSurface;
extern SDL_Window* gWindow;

extern SDL_Event e;



extern const int ITEM_WIDTH;
extern const int ITEM_HEIGHT;
extern const int SCREEN_TICKS_PER_FRAME;
extern const int SCREEN_FPS;
extern const int MAP_SIZE;

extern void close();

Map::Map()
{
	swapMiss = false;
	reSwapDirection = 0;
	hintIndexRaw = 0;
	hintIndexCol = 0;
	hintIndexRaw2 = 0;
	hintIndexCol2 = 0;

	hintCounter = 0;
	hint = false;
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

void Map::moveItems(int	interval)
{
	for (int k = 0; k < interval; k++) {
		for (int i = 0; i < MAP_SIZE;i++) {
			for (int j = 0;j < MAP_SIZE;j++) {
				itemList[i][j]->moveItem();
			}
		}
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
		drawMap();
		SDL_RenderPresent(gRenderer);
		SDL_Delay(SCREEN_TICKS_PER_FRAME);
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

	for (int i=0; i < MAP_SIZE; i++) {
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

void Map::pang()
{
	pangRaw();
	pangColumn();
}

void Map::pangRaw()
{
	for (auto &itemRaw : itemList) {
		for (int i = 0; i < MAP_SIZE - 2;i++) {
			if (itemRaw[i]->getIconType() == itemRaw[i + 1]->getIconType() &&
				itemRaw[i+1]->getIconType() == itemRaw[i + 2]->getIconType()) {
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
				itemRaw[i+1]->getIconType() == itemRaw[i + 2]->getIconType()) {
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

void Map::adjustCurrentPosition()
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

	switch (direction){								//받은 방향에 따라 타겟 아이템의 인덱스를 변경
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

void Map::pangEffect(int interval)
{
	for (int i = 0; i < interval; i++) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				close();
				exit(0);
			}
		}
		for (int i = 0; i < MAP_SIZE; i++){
			for (int j = 0; j < MAP_SIZE; j++) {
				if (!itemList[i][j]->isExist()) {
					itemList[i][j]->pangEffect();
				}
			}
		}
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
		drawMap();
		SDL_RenderPresent(gRenderer);
		SDL_Delay(SCREEN_TICKS_PER_FRAME);
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
				hintIndexCol2 = col+1;

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
				hintIndexRaw2 = raw+1;
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
	if (SCREEN_FPS / 10 * 1 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 2){
		hintItem->image_20();
		hintItem2->image_20();
	}
	if (SCREEN_FPS / 10 * 2 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 3){
		hintItem->image_40();
		hintItem2->image_40();
	}
	if (SCREEN_FPS / 10 * 3 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 4){
		hintItem->image_60();
		hintItem2->image_60();
	}
	if (SCREEN_FPS / 10 * 4 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 5){
		hintItem->image_80();
		hintItem2->image_80();
	}
	if (SCREEN_FPS / 10 * 5 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 6){
		hintItem->image_100();
		hintItem2->image_100();
	}
	if (SCREEN_FPS / 10 * 6 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 7){
		hintItem->image_80();
		hintItem2->image_80();
	}
	if (SCREEN_FPS / 10 * 7 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 8){
		hintItem->image_60();
		hintItem2->image_60();
	}
	if (SCREEN_FPS / 10 * 8 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 9){
		hintItem->image_40();
		hintItem2->image_40();
	}
	if (SCREEN_FPS / 10 * 9 <= hintDrawCounter && hintDrawCounter < SCREEN_FPS / 10 * 10){
		hintItem->image_20();
		hintItem2->image_20();
	}

	hintDrawCounter++;
	if (hintDrawCounter == SCREEN_FPS)
		hintDrawCounter = 0;



}
