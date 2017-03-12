#include "j1Collision.h"
#include "j1App.h"
#include "j1EntityManager.h"

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