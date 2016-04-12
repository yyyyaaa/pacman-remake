#include "Level.h"

USING_NS_CC;
Level::Level()
{

}

Level::~Level()
{
	_tilemap->release();
}

void Level::initOptions()
{
	_tilemap->setScale(SCALE_FACTOR);
	this->getPath()->setVisible(true);
}
void Level::loadMap(const std::string& filename)
{
	_tilemap = cocos2d::TMXTiledMap::create(filename);
	_tilemap->retain();
}

cocos2d::TMXTiledMap* Level::getMap()
{
	return this->_tilemap;
}
cocos2d::TMXLayer* Level::getBackground()
{
	return this->_tilemap->getLayer("background"); //hard-coded
}
cocos2d::TMXLayer* Level::getPath()
{
	return this->_tilemap->getLayer("path"); //hard-coded
}
cocos2d::Point Level::getSpawnPoint()
{
	cocos2d::TMXObjectGroup* objects = _tilemap->getObjectGroup("spawnpoint"); //hard-coded
	CCASSERT(NULL != objects, "No object layer named spawnpoint");
	auto playerSpawnPoint = objects->getObject("SpawningPoint");
	CCASSERT(!playerSpawnPoint.empty(), "No object named SpawnPoint");
	//calculate SpawnPoint Vec2
	float x = playerSpawnPoint["x"].asFloat();
	float y = playerSpawnPoint["y"].asFloat();

	//return cocos2d::Point(x + _tilemap->getTileSize().width / 2, y + _tilemap->getTileSize().height / 2);
	return cocos2d::Point(x, y);
}

//convert from screen position to map coords
cocos2d::Point Level::tileCoordinateToPosition(cocos2d::Size s, cocos2d::Point point)
{
	float x = floor(s.width / 2 * SCALE_FACTOR + point.x * _tilemap->getTileSize().width * SCALE_FACTOR);
	float y = floor(s.height / 2 * SCALE_FACTOR + point.y * _tilemap->getTileSize().height * SCALE_FACTOR);
	return Point(x, y);
}
cocos2d::Point Level::positionToTileCoordinate(cocos2d::Size s, cocos2d::Point point)
{
	float x = floor((point.x - s.width / 2 * SCALE_FACTOR) / (_tilemap->getTileSize().width * SCALE_FACTOR));
	float y = floor((point.y - s.height / 2 * SCALE_FACTOR) / (_tilemap->getTileSize().height * SCALE_FACTOR));
	return Point(x, y);
}
bool Level::isValidPath(Point mapcoord)
{
	int tileGID = _path->getTileGIDAt(mapcoord);
	if (tileGID)
	{
		auto properties = _tilemap->getPropertiesForGID(tileGID).asValueMap();
		if (!properties.empty())
		{
			auto ispath = properties["path"].asString();
			if ("true" == ispath)
				return true;
		}
	}
	return false;
}