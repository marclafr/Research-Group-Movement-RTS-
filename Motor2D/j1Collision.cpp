#include "j1Collision.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"

#include "j1Map.h"

j1Collision::j1Collision()
{
	name.assign("collision");
}

j1Collision::~j1Collision() {}

bool j1Collision::Update(float dt)
{
	bool ret = true;

	//Check colisions between units
	for (std::list<Entity*>::iterator unit1 = App->entity_manager->entity_list.begin(); unit1 != App->entity_manager->entity_list.end(); unit1++)
	{
		for (std::list<Entity*>::iterator unit2 = App->entity_manager->entity_list.begin(); unit2 != App->entity_manager->entity_list.end(); unit2++)
		{
			if (unit1 != unit2) 
			{
				if (DoUnitsIntersect((Unit*)unit1._Ptr->_Myval, (Unit*)unit2._Ptr->_Myval) == true)
				{
					//Collision detected
					Unit* unit_1 = (Unit*)unit1._Ptr->_Myval;
					Unit* unit_2 = (Unit*)unit2._Ptr->_Myval;
					if (unit_1->id > unit_2->id)
					{
						//iPoint tile = FindClosestWalkable(unit1._Ptr->_Myval->GetX(), unit1._Ptr->_Myval->GetY());
						//TEST: iPoint map_tile = App->map->WorldToMap(tile.x, tile.y);
						//unit1._Ptr->_Myval->SetPosition(tile.x, tile.y);
						SplitUnits((Unit*)unit1._Ptr->_Myval, (Unit*)unit2._Ptr->_Myval);
					}
				}
			}
		}
	}

	return ret;
}

bool j1Collision::DoUnitsIntersect(Unit* unit1, Unit* unit2)
{
	float distance_x = unit1->GetX() - unit2->GetX();
	float distance_y = unit1->GetY() - unit2->GetY();
	return (sqrt(distance_x * distance_x + distance_y * distance_y) < unit1->unit_radius + unit2->unit_radius);
}

iPoint j1Collision::FindClosestWalkable(int x, int y)
{
	bool found = false;
	int lenght = 3;

	iPoint tile = { x + lenght / 2, y - lenght / 2 - 1 };

	while (!found && lenght < 20)
	{
		while (tile.y < y + lenght / 2 && !found)
		{
			tile.y++;
			if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
				found = true;
		}

		while (tile.x > x - lenght / 2 && !found)
		{
			tile.x--;
			if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
				found = true;
		}

		while (tile.y > y - lenght / 2 && !found)
		{
			tile.y--;
			if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
				found = true;

		}

		while (tile.x < x + lenght / 2 && !found)
		{
			tile.x++;
			if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
				found = true;
		}
		lenght += 2;
	}
	return tile;
}

void j1Collision::SplitUnits(Unit * unit1, Unit * unit2)
{
	//axis X
	if (unit1->GetX() > unit2->GetX())
	{
		unit1->SetPosition(unit1->GetX() + 1, unit1->GetY());
	}
	if (unit1->GetX() < unit2->GetX())
	{
		unit1->SetPosition(unit1->GetX() - 1, unit1->GetY());
	}
	//axis Y
	if (unit1->GetY() > unit2->GetY())
	{
		unit1->SetPosition(unit1->GetX(), unit1->GetY() + 1);
	}
	if (unit1->GetY() < unit2->GetY())
	{
		unit1->SetPosition(unit1->GetX(), unit1->GetY() - 1);
	}

}
