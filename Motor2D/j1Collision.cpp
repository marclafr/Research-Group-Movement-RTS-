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

	for (std::list<Entity*>::iterator unit1 = App->entity_manager->entity_list.begin(); unit1 != App->entity_manager->entity_list.end(); unit1++)
	{
		iPoint pos = App->map->WorldToMap(unit1._Ptr->_Myval->GetX(), unit1._Ptr->_Myval->GetY());
		Unit* unit_1 = (Unit*)unit1._Ptr->_Myval;
		if (!App->pathfinding->IsWalkable(pos) && unit_1->moving == false)
		{
			iPoint tile = FindClosestWalkable(unit1._Ptr->_Myval->GetX(), unit1._Ptr->_Myval->GetY());
			unit1._Ptr->_Myval->SetPosition(tile.x, tile.y);
		}
		else
		{
			//Check colisions between units
			for (std::list<Entity*>::iterator unit2 = App->entity_manager->entity_list.begin(); unit2 != App->entity_manager->entity_list.end(); unit2++)
			{
				if (unit1 != unit2)
				{
					if (DoUnitsIntersect((Unit*)unit1._Ptr->_Myval, (Unit*)unit2._Ptr->_Myval) == true)
					{
						//Collision detected
						Unit* unit_2 = (Unit*)unit2._Ptr->_Myval;
						if (unit_1->id > unit_2->id && unit_1->moving == false)
						{
							SplitUnits((Unit*)unit1._Ptr->_Myval, (Unit*)unit2._Ptr->_Myval);
						}
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

	iPoint tile = App->map->WorldToMap(x, y);
	iPoint origin = App->map->WorldToMap(x, y);

	int dist = 1;

	while (!found)
	{
		tile.y += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.x += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
		{
			found = true;
			break;
		}

		tile.x -= dist;
		tile.x -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }))
		{
			found = true;
			break;
		}

		dist++;
	}

	return App->map->MapToWorld(tile.x, tile.y);

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
