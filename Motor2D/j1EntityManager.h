#ifndef _j1EntityManager_
#define _j1EntityManager_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateUnit(UNIT_TYPE u_type, fPoint pos);
	Entity* CreatBuilding(BUILDING_TYPE b_type, fPoint pos);

	void DeleteEntity(Entity* ptr); // will call other delete methods
	void DeleteUnit(Entity* ptr);
	void DeleteBuilding(Entity* ptr);
	void GetUnitsPath(iPoint destintation);

	std::list<Entity*> entity_list;

private:

	//Textura provisional para sprites por si no se cargan en animation
	SDL_Texture* sprites;

	//lista para "almacenar" unidades, puede ser cambiada a array etc.
};
#endif //_j1EntityManager_

