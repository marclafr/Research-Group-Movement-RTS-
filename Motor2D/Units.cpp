
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1EntityManager.h"

Unit::Unit(UNIT_TYPE u_type, fPoint pos): Entity(UNIT, pos), unit_type(u_type), direction(WEST), action_type(IDLE)
{
	switch (u_type)
	{
		//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case TWOHANDEDSWORDMAN:
		SetHp(60);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 1;
		unit_class = INFANTRY;
		break;

	case CAVALRYARCHER:
		SetHp(50);
		attack = 6;
		SetArmor(1);
		speed = 1.4;
		rate_of_fire = 2;
		range = 4;
		unit_class = ARCHER;
		break;

	case SIEGERAM:
		SetHp(270);
		attack = 4;
		SetArmor(-5);
		speed = 0.6;
		rate_of_fire = 5;
		range = 1;
		unit_class = SIEGE;
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = NO_CLASS;
		break;
	}
}

void Unit::Update()
{
	AI();
	Move();
	Draw();
}

void Unit::Move()
{
	if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
	{
		App->input->GetMousePosition(destination.x, destination.y);
		destination.x -= App->render->camera.x;
		destination.y -= App->render->camera.y;
		if (this->GetEntityStatus() == E_SELECTED)
		{
			if (this->GetPath({ destination.x, destination.y }) != -1)
			{
				this->action_type = WALK;
				this->moving = true;
			}
			else
			{
				this->moving = false;
				this->action_type = IDLE;
			}
		}
	}

	if (this->moving == true)
	{
		iPoint dest_map = App->map->WorldToMap(destination.x, destination.y);
		iPoint unit_map = App->map->WorldToMap(this->GetX() , this->GetY());


		if (path_list.size() > 0 && unit_map == path_list.front())
		{
			path_list.pop_front();
			iPoint nxt = path_list.front();
			iPoint prv = App->map->WorldToMap(this->GetX(), this->GetY());

			if (nxt.x > prv.x && nxt.y < prv.y)
			{
				this->direction = EAST;
			}
			else if (nxt.x > prv.x && nxt.y > prv.y)
			{
				this->direction = SOUTH;
			}
			else if (nxt.x > prv.x && nxt.y == prv.y)
			{
				this->direction = SOUTH_EAST;
			}
			else if (nxt.x == prv.x && nxt.y > prv.y)
			{
				this->direction = SOUTH_WEST;
			}
			else if (nxt.x < prv.x && nxt.y > prv.y)
			{
				this->direction = WEST;
			}
			else if (nxt.x < prv.x && nxt.y < prv.y)
				this->direction = NORTH;

			else if (nxt.x == prv.x && nxt.y < prv.y)
				this->direction = NORTH_EAST;

			else if (nxt.x < prv.x && nxt.y == prv.y)
				this->direction = NORTH_WEST;
			

		}
		switch (this->direction)
		{
		case EAST: 
			this->SetPosition(this->GetX() + this->speed, this->GetY());
			break;
		case SOUTH:
			this->SetPosition(this->GetX(), this->GetY() + this->speed / XY_TILES_RELATION);
			break;
		case SOUTH_EAST:
			this->SetPosition(this->GetX() + this->speed, this->GetY() + this->speed / XY_TILES_RELATION);
			break;
		case SOUTH_WEST:
			this->SetPosition(this->GetX() - this->speed, this->GetY() + this->speed / XY_TILES_RELATION);
			break;
		case WEST:
			this->SetPosition(this->GetX() - this->speed, this->GetY());
			break;
		case NORTH:
			this->SetPosition(this->GetX(), this->GetY() - this->speed / XY_TILES_RELATION);
			break;
		case NORTH_EAST:
			this->SetPosition(this->GetX() + this->speed, this->GetY() - this->speed / XY_TILES_RELATION);
			break;
		case NORTH_WEST:
			this->SetPosition(this->GetX() - this->speed, this->GetY() - this->speed / XY_TILES_RELATION);
			break;
		default:
			this->SetPosition(this->GetX() + this->speed, this->GetY() + this->speed/ XY_TILES_RELATION);
			break;
		}

		if (unit_map == dest_map)
		{
			this->moving = false;
			this->action_type = IDLE;
		}
	}

}

void Unit::AI()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		action_type = ATTACK;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		action_type = IDLE;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		action_type = WALK;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		action_type = DISAPPEAR;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);
		
		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		action_type = DIE;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}

}

void Unit::Draw()
{
	SDL_Texture* tex = App->anim->GetTexture(unit_type);
	SDL_Rect rect;
	iPoint pivot;

	App->anim->GetAnimationFrame(rect, pivot, this);

	if (direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST)
		App->render->Blit(tex, GetX(), GetY(), &rect, SDL_FLIP_HORIZONTAL, pivot.x, pivot.y);
	else
		App->render->Blit(tex, GetX() - pivot.x, GetY() - pivot.y, &rect);
}

const DIRECTION Unit::GetDir() const
{
	return direction;
}

const UNIT_TYPE Unit::GetUnitType() const
{
	return unit_type;
}

const ACTION_TYPE Unit::GetActionType() const
{
	return action_type;
}

int Unit::GetPath(iPoint dest)
{
	iPoint ori = App->map->WorldToMap(GetX(), GetY());
	iPoint destinat = App->map->WorldToMap(dest.x, dest.y);
	return App->pathfinding->CreatePath(ori, destinat, path_list);
}