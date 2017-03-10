#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("Units");
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
	Entity* new_entity = (Entity*) new Unit(u_type, pos);
	entity_list.push_back(new_entity);
	return new_entity;
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