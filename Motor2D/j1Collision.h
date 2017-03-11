#ifndef _j1Collision_
#define _j1Collision_

#include "j1Module.h"

class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	bool Update(float dt);

private:
};
#endif //_j1Collision_