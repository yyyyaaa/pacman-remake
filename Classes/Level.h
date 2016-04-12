#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "Definitions.h"
#include <string>

class Level: public cocos2d::Object
{
public:
	//FIELDS
	cocos2d::TMXTiledMap* _tilemap;
	cocos2d::TMXLayer* _background;
	cocos2d::TMXLayer* _path;
	cocos2d::Point _spawningPoint;

	//METHODS
	void initOptions();
	void loadMap(const std::string& filename);

	//get spawning point position
	cocos2d::Point getSpawnPoint();
	cocos2d::TMXTiledMap* getMap();
	cocos2d::TMXLayer* getBackground();
	cocos2d::TMXLayer* getPath();

	//convert from tile coords to global
	cocos2d::Point tileCoordinateToPosition(cocos2d::Size s, cocos2d::Point point);
	//reverse
	cocos2d::Point positionToTileCoordinate(cocos2d::Size s, cocos2d::Point point);

	//check if path tile
	bool isValidPath(Point mapcoord);

	//constructor, destructor
	Level();
	virtual ~Level();
};

#endif // __SPLASH_SCENE_H__
