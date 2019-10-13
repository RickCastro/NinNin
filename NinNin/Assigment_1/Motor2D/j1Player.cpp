#include <iostream>
#include <math.h>
#include "p2Defs.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Input.h"
//#include "ModuleParticles.h"
#include "j1Render.h"
#include "j1Window.h"
//#include "ModuleCollision.h"
//#include "ModuleFadeToBlack.h"
#include "j1Player.h"
#include "j1Audio.h"
//#include "ModuleFirstScene.h"

using namespace std;


j1Player::j1Player():j1Module()
{
	name.create("player");
	graphics = NULL;
	current_animation = NULL;

	//idle animation Right
	idle_Stop.PushBack({ 64,111,176,331 });
	idle_Stop.PushBack({ 310,111,176,331 });
	idle_Stop.PushBack({ 575,111,176,331 });
	idle_Stop.PushBack({ 845,111,176,331 });
	idle_Stop.PushBack({ 1138,111,176,331 });
	idle_Stop.PushBack({ 1450,111,176,331 });
	idle_Stop.PushBack({ 1735,111,176,331 });
	idle_Stop.PushBack({ 2019,111,176,331 });
	idle_Stop.PushBack({ 2284,111,176,331 });
	idle_Stop.PushBack({ 2559,111,176,331 });

	idle_Stop.loop = true;
	idle_Stop.speed = 0.05f;
	
	// Move Right
	right.PushBack({ 64,472,263,332 });
	right.PushBack({ 327,472,263,332 });
	right.PushBack({ 590,472,263,332 });
	right.PushBack({ 853,472,263,332 });
	right.PushBack({ 1137,472,263,332 });
	right.PushBack({ 1449,472,263,332 });
	right.PushBack({ 1734,472,263,332 });
	right.PushBack({ 2019,472,263,332 });
	right.PushBack({ 2286,472,263,332 });
	right.PushBack({ 2560,472,263,332 });

	right.loop = true;
	right.speed = 0.1f;

	//Move Right back
	right_back.PushBack({ 0,0,50,50 });

	right_back.loop = true;
	right_back.speed = 0.09f;

	// Move Left
	left.PushBack({ 64,823,263,332 });
	left.PushBack({ 327,823,263,332 });
	left.PushBack({ 590,823,263,332 });
	left.PushBack({ 853,823,263,332 });
	left.PushBack({ 1137,823,263,332 });
	left.PushBack({ 1449,823,263,332 });
	left.PushBack({ 1734,823,263,332 });
	left.PushBack({ 2019,823,263,332 });
	left.PushBack({ 2286,823,263,332 });
	left.PushBack({ 2560,823,263,332 });

	left.loop = true;
	left.speed = 0.1f;

	//Move Left Back
	left_back.PushBack({ 0,0,50,50 });

	left_back.loop = true;
	left_back.speed = 0.09f;

	//DIE
	die.PushBack({ 331,190,29,26 });
	die.PushBack({ 361,190,29,26 });
	die.PushBack({ 391,190,29,26 });
	die.loop = false;
	die.speed = 0.05f;

	//JUMP
	jump.PushBack({ 64,2103,263,332 });
	jump.PushBack({ 327,2103,263,332 });
	jump.PushBack({ 590,2103,263,332 });
	jump.PushBack({ 853,2103,263,332 });
	jump.PushBack({ 1137,2103,263,332 });
	jump.PushBack({ 1449,2103,263,332 });
	jump.PushBack({ 1734,2103,263,332 });
	jump.PushBack({ 2019,2103,263,332 });
	jump.PushBack({ 2286,2103,263,332 });
	jump.PushBack({ 2560,2103,263,332 });

	jump.loop = true;
	jump.speed = 0.05f;

}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{

	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;
	win_scale = App->win->GetScale();

	LOG("Loading Player Sheet");

	graphics = App->tex->Load("assets/character/character.png");

	LOG("Loading Player Collider");
	//Player_Coll = App->collision->AddCollider({ position.x, position.y, 46, 70 }, COLLIDER_PLAYER, this);
	//font_score = App->fonts->Load("fonts/Lletres_1.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ./\ ", 2);

	position.x = win_width/2;
	position.y = 215;

	stop = false;
	anim = false;
	water = false;
	grenade = false;
	god = false;
	counter = 0;
	only = true;
	fall = false;

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player Sheet");
	App->tex->UnLoad(graphics);
	//App->fonts->UnLoad(font_score);
	//App->textures->Unload(textures);

	LOG("Destroying Player Collider");
	//if (Player_Coll != nullptr)
		//Player_Coll->to_delete = true;

	return true;
}
// Update: draw background
bool j1Player::Update(float dt)
{
	SDL_Event e;
	speed = 1;
	SDL_Rect Potato = { 128,717,49,56 };
	SDL_Rect Potato_2 = { ((win_width*win_scale) / 2) - (23 * win_scale),position.y*win_scale - 35 * win_scale,100,100 };

	//LEFT
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;

		//The direccion changes with the position of the mouse
		
		if (current_animation != &left)
		{
			left.Reset();
			current_animation = &left;
			player_last_direction = RIGHT;
		}

	}
	//RIGHT
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		position.x += speed;

		if (current_animation != &right)
		{
			right.Reset();
			current_animation = &right;
			player_last_direction = RIGHT;
		}

	}

	//JUMP
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && Jump == false)
	{
		Jump = true;
		//position.y -= speed;

		if (current_animation != &jump)
		{
			jump.Reset();
			current_animation = &jump;
			player_last_direction = RIGHT;
		}
	}
	//DOWN
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		/*if (position.y <SCREEN_HEIGHT - 20) {
		position.y += speed;
		}
		if (current_animation != &down)
		{
		down.Reset();
		current_animation = &down;
		}
		player_last_direction = DOWN;*/
	}
	//UP
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		/*if (position.y > -1864 + SCREEN_HEIGHT) {
		position.y -= speed;
		}
		if (current_animation != &up)
		{
		up.Reset();
		current_animation = &up;
		}
		player_last_direction = UP;*/
	}

	//SHOT 

//	shot();
//	angle();

	while (SDL_PollEvent(&e))
	{
		switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				shooting = true;
			}
			break;
		}
	}

	//GRAVITY SIMULATOR
	/*if (Jump == false)
	{
		position.y += 5;
	}

	/*
		//JUMP METHODE
	if (Jump == true && fall == false)
	{
		position.y -= 10;
		counter++;
		if (counter == 10)
		{
			counter = 0;
			fall = true;
		}
	}
	if (Jump == true && fall == true)
	{
		position.y += 10;
	}*/

	/*if (anim == true)
	{
	current_animation = &die;
	if (current_animation->Finished() == true)
	{
	current_animation->Reset();
	anim = false;
	alive = true;
	only = true;
	}
	}
	else
	{
	anim = false;
	Disable();
	}


	*/

	// Special attack

	/*if (App->input->keyboard[SDL_SCANCODE_P] == KEY_STATE::KEY_DOWN ||
	App->input->controll[5] == KEY_STATE::KEY_DOWN) {   //Mes maco que lo de dalt no? (lo de dalt es pot borrar ja que ja no funciona )
	if (App->elements1->num_grenades > 0){
	current_animation = &grenadeThrow;
	if (grenade == false)
	App->particles->AddParticle(App->particles->grenade, position.x + 11, position.y + 3, COLLIDER_NONE);
	}
	grenade = true;
	}
	if (grenade) {
	if (current_animation->Finished() == true) {
	current_animation->Reset();
	grenade = false;
	}
	}*/

	//IDLE ANIMATIONS
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		switch (player_last_direction)
		{
		case j1Player::UP:
			current_animation = &idle_Stop;
			break;
		case j1Player::DOWN:
			current_animation = &idle_Stop;
			break;
		case j1Player::LEFT:
			current_animation = &idle_Stop;
			break;
		case j1Player::RIGHT:
			current_animation = &idle_Stop;
			break;
		default:
			break;
		}
	}

	// Player Colliders Position
	//Player_Coll->SetPos(position.x, position.y);

	//Rotate texture
	//SDL_RenderCopyEx(App->render->renderer, graphics, &Potato, &Potato_2, angle(), NULL, SDL_FLIP_NONE);
	// Draw everything --------------------------------------
	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),1.0f);

	return true;
}

// Load / Save
bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();

	return true;

}
bool j1Player::Save(pugi::xml_node& data) const 
{
	pugi::xml_node cam = data.append_child("player");

	cam.append_attribute("x") = position.x;
	cam.append_attribute("y") = position.y;

	return true;
}


/*void j1Player::OnCollision(Collider* c1, Collider* c2) {
	if (c2->type == COLLIDER_WALL)
	{
		if (c1 == feetcoll && (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT
			|| App->input->controll[0] == KEY_STATE::KEY_REPEAT))
		{
			position.y += 1;
		}
		if (c1 == feetcoll && (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT
			|| App->input->controll[1] == KEY_STATE::KEY_REPEAT))
		{
			position.y -= 1;
		}
		if (c1 == feetcoll && (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT
			|| App->input->controll[3] == KEY_STATE::KEY_REPEAT))
		{
			position.x += 1;
		}
		if (c1 == feetcoll && (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT
			|| App->input->controll[2] == KEY_STATE::KEY_REPEAT))
		{
			position.x -= 1;
		}
	}

	if (c2->type == COLLIDER_ENEMY_SHOT && god == false)
	{
		if (alive) {
			alive = false;
			App->elements1->num_lives_play_1--;
			anim = true;
		}
	}

	if (c2->type == COLLIDER_ENEMY && god == false)
	{
		if (alive) {
			alive = false;
			App->elements1->num_lives_play_1--;
			anim = true;
		}
	}

	//Jump methode
	if (c2->type == COLLIDER_FLOOR)
	{
		position.y -= 5;
		Jump = false;
		fall = false;
	}
}*/

/*void j1Player::shot() {
	int x, y;

	// Taking the Position of the mouse
	SDL_GetMouseState(&x, &y);

	//Changing the focus of the mouse (0,0) by default
	x = x - ((SCREEN_WIDTH*SCREEN_SIZE) / 2) - (23 * SCREEN_SIZE);
	y = y - position.y*SCREEN_SIZE - 35 * SCREEN_SIZE;
	LOG("La X=%d i la Y=%d", x, y);

	//The direction of the bullet is the module of the new mouse position
	App->particles->bullet.speed.y = y / sqrt(x*x + y*y) * 5;
	App->particles->bullet.speed.x = x / sqrt(x*x + y*y) * 5;

	//Creates the Particle
	if (shots_fired < SHOTS_PER_BURST && shooting == true && App->ui->mana>0) {
		LOG("ShOOTTOODAAA!!!!");
		if (shot_current_delay < SHOT_DELAY)
			shot_current_delay++;
		else {
			App->particles->AddParticle(App->particles->bullet, position.x + 23, position.y + 35, COLLIDER_PLAYER_SHOT);
			shots_fired++;
			shot_current_delay = 0;
			App->ui->mana--;
		}
	}
	else {
		shots_fired = 0;
		shooting = false;
	}
}*/

float j1Player::angle()
{
	int x, y;
	float div;
	float angle_rad;
	float angle;

	// Taking the Position of the mouse
	SDL_GetMouseState(&x, &y);

	//Changing the focus of the mouse (0,0) by default
	x = x - ((win_width*win_scale) / 2) - (23 * win_scale);
	y = y - position.y*win_scale - 35 * win_scale;
//	LOG("La X=%d i la Y=%d", x, y);


	//Obtaining the angle
	if (x >0 && y>0) {
		div = ((float)y / (float)x);
		angle_rad = atan(div);
		angle = angle_rad * 57.2957795;
	}
	if (x <0 && y>0) {
		div = ((float)x / (float)y);
		angle_rad = atan(div);
		angle = (angle_rad * 57.2957795*-1) + 90;
	}
	if (x <0 && y<0) {
		div = ((float)y / (float)x);
		angle_rad = atan(div);
		angle = (angle_rad * 57.2957795) + 180;
	}
	if (x >0 && y<0) {
		div = ((float)x / (float)y);
		angle_rad = atan(div);
		angle = (angle_rad * 57.2957795*-1) + 270;
	}
	if (x == 0 && y < 0) { angle = 270; }
	if (x == 0 && y > 0) { angle = 90; }
	if (x < 0 && y == 0) { angle = 180; }
	if (x > 0 && y == 0) { angle = 0; }

	//LOG("El angulo es %f", angle);

	return angle;
}
