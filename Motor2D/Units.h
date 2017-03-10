#ifndef _UNITS
#define _UNITS

#include "Entity.h"

#define XY_TILES_RELATION 2

struct PathList;
struct PathNode;

enum UNIT_TYPE
{
	NO_UNIT = 0,
	VILLAGER,
	TWOHANDEDSWORDMAN,
	CAVALRYARCHER,
	SIEGERAM
};

enum UNIT_CLASS
{
	NO_CLASS = 0,
	INFANTRY,
	ARCHER,
	CAVALRY,
	SIEGE,
};

enum ACTION_TYPE
{
	NO_ACTION = 0,
	ATTACK,
	DIE,
	DISAPPEAR,
	IDLE,
	WALK
};

enum DIRECTION
{
	NO_DIRECTION = 0,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

class Unit : public Entity
{
private:
	enum UNIT_TYPE unit_type;
	enum ACTION_TYPE action_type;
	enum DIRECTION direction;

	int attack;
	int range;
	float speed;
	float rate_of_fire;
	iPoint destination;
	bool moving = false;
	UNIT_CLASS unit_class;
	std::list<iPoint> path_list;

public:

	Unit(UNIT_TYPE u_type, fPoint pos);

	void Update(); // defines order

	virtual void Move();
	virtual void AI();
	virtual void Draw();
	

	const DIRECTION GetDir() const;
	const UNIT_TYPE GetUnitType() const;
	const ACTION_TYPE GetActionType() const;
	int GetPath(iPoint dest);
};

#endif

