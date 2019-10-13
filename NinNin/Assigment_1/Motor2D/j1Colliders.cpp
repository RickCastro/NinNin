#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Colliders.h"

j1Colliders::j1Colliders() : j1Module()
{
	name.create("colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_FEET] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FEET] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_GRENADE_EXPLOSION] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_FEET] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_MAP_LIMIT_FOR_ENEMIES] = true;


	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_FEET] = false;

	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_FEET] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_FEET][COLLIDER_FEET] = false;
	matrix[COLLIDER_FEET][COLLIDER_WALL] = true;
	matrix[COLLIDER_FEET][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_FEET][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_FEET][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_FEET][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_GRENADE_EXPLOSION][COLLIDER_FEET] = false;
	matrix[COLLIDER_GRENADE_EXPLOSION][COLLIDER_WALL] = false;
	matrix[COLLIDER_GRENADE_EXPLOSION][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GRENADE_EXPLOSION][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_GRENADE_EXPLOSION][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_GRENADE_EXPLOSION][COLLIDER_ENEMY_SHOT] = false;

	matrix[COLLIDER_FLOOR][COLLIDER_WALL] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_FEET] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
}

// Destructor
j1Colliders::~j1Colliders()
{

}

bool j1Colliders::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool j1Colliders::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Colliders::Update(float dt)
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (c1->to_delete == false && c2->to_delete != true) {
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);
					if (c1->to_delete == false) {
						if (matrix[c2->type][c1->type] && c2->callback)
							c2->callback->OnCollision(c2, c1);
					}
				}
			}
		}
	}

	DebugDraw();

	return true;
}

void j1Colliders::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		LOG("YOu can see the colliders");
		debug = !debug;
	}

	if (debug == false) {
		return;
	}
	else {

		Uint8 alpha = 80;
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == nullptr)
				continue;

			switch (colliders[i]->type)
			{
			case COLLIDER_NONE: // white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			case COLLIDER_WALL: // blue
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
				break;
			case COLLIDER_PLAYER: // green
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
				break;
			case COLLIDER_ENEMY: // red
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
				break;
			case COLLIDER_PLAYER_SHOT: // yellow
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
				break;
			case COLLIDER_ENEMY_SHOT: // magenta
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
				break;
			case COLLIDER_FEET: // pink
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
				break;
			case COLLIDER_GRENADE_EXPLOSION: // orange
				App->render->DrawQuad(colliders[i]->rect, 255, 170, 0, alpha);
				break;
			case COLLIDER_FLOOR: // light blue
				App->render->DrawQuad(colliders[i]->rect, 135, 206, 250, alpha);
				break;
			case COLLIDER_MAP_LIMIT_FOR_ENEMIES: //magenta
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
				break;
			}

		}
	}
}

bool j1Colliders::checkColisionList(Collider * enemCollider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr)
			continue;
		if (enemCollider->CheckCollision(colliders[i]->rect))
			return true;
	}
	return false;
}

// Called before quitting
bool j1Colliders::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Colliders::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool j1Colliders::EraseCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			return true;
		}
	}

	return false;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (rect.x <= r.x + r.w && rect.x + rect.w >= r.x)
	{
		if (rect.y <= r.y + r.h && rect.y + rect.h >= r.y)
		{
			return true;
		}
	}
	return false;
}




