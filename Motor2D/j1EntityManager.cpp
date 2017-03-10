#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("Units");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::Awake() { return true; }

bool j1EntityManager::CleanUp() { // not done

	if(sprites != nullptr)
	App->tex->UnLoad(sprites);

	std::list<Entity*>::iterator item = entity_list.begin();
	while (item != entity_list.end()) {
		RELEASE(item._Ptr->_Myval);
		item++;
	}
	entity_list.clear();

	//Borrar enemigos
	//for(uint i = 0;             ; ++i)
		//if(unit_list[i])
	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Unit(u_type, pos);
	entity_list.push_back(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreatBuilding(BUILDING_TYPE b_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Building(b_type, pos);
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

		case BUILDING:
			DeleteBuilding(ptr);
			break;
	}
}

void j1EntityManager::DeleteUnit(Entity * ptr)
{
}

void j1EntityManager::DeleteBuilding(Entity * ptr)
{
}


bool j1EntityManager::Update(float dt)
{

	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); ++it)
	{
		it._Ptr->_Myval->Update();
	}



	//a través de la lista donde 
	//tenemos los enemigos ejecutamos los siguientes pasos

	//funcion de movimiento de cada enemigo

	//Draw de los enemigos

	//si han muerto y han acabado su animacion de muerte hacer delete
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

void j1EntityManager::GetUnitsPath(iPoint destination) {

	std::list<Entity*>::iterator prove = entity_list.begin();

	while (prove != entity_list.end())
		//if (prove._Ptr->_Myval->GetEntityStatus() == E_SELECTED)
			prove++;


}