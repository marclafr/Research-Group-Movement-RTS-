#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1Map.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("entity_manager");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::Awake() { return true; }

bool j1EntityManager::CleanUp() 
{
	std::list<Entity*>::iterator item = entity_list.begin();
	while (item != entity_list.end())
	{
		RELEASE(item._Ptr->_Myval);
		item++;
	}
	entity_list.clear();

	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type, fPoint pos)
{
	id++;
	Entity* new_entity = (Entity*) new Unit(u_type, pos, id);
	entity_list.push_back(new_entity);
	return new_entity;
}

void j1EntityManager::SelectInQuad(const SDL_Rect& select_rect)
{
	int unit_selected_count = 0;
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		if (unit_selected_count >= MAX_UNIT_SELECTED)
			break;

		int unit_x = it._Ptr->_Myval->GetX();
		int unit_y = it._Ptr->_Myval->GetY();
		if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
		{
			it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			unit_selected_count++;
		}
		else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
		{
			it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			unit_selected_count++;
		}
		else if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
		{
			it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			unit_selected_count++;
		}
		else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
		{
			it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			unit_selected_count++;
		}		
	}
}

void j1EntityManager::UnselectEverything()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		it._Ptr->_Myval->SetEntityStatus(E_NON_SELECTED);
	}
}

void j1EntityManager::DeleteEntity(Entity * ptr)
{
	switch (ptr->GetEntityType())
	{
		case UNIT:
			DeleteUnit(ptr);
			break;
	}
}

void j1EntityManager::DeleteUnit(Entity * ptr)
{
}

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	for (std::list<Entity*>::iterator units = App->entity_manager->entity_list.begin(); units != App->entity_manager->entity_list.end(); units++)
	{
		if (units._Ptr->_Myval != unit)
		{
			if (tile == App->map->WorldToMap(units._Ptr->_Myval->GetX(), units._Ptr->_Myval->GetY()))
				return true;
		}
	}

	return false;
}

bool j1EntityManager::Update(float dt)
{

	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); ++it)
	{
		it._Ptr->_Myval->Update();
	}

	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); ++it)
	{
		if (it._Ptr->_Myval->ToDelete() == true)
		{
			DeleteEntity(it._Ptr->_Myval);
			entity_list.erase(it);
		}
	}
	return true;
}