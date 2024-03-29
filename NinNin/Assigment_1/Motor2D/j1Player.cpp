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
#include "j1Colliders.h"
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
	{
		idle_right.PushBack({ 64,111,176,331 });
		idle_right.PushBack({ 310,111,176,331 });
		idle_right.PushBack({ 575,111,176,331 });
		idle_right.PushBack({ 845,111,176,331 });
		idle_right.PushBack({ 1138,111,176,331 });
		idle_right.PushBack({ 1450,111,176,331 });
		idle_right.PushBack({ 1735,111,176,331 });
		idle_right.PushBack({ 2019,111,176,331 });
		idle_right.PushBack({ 2284,111,176,331 });
		idle_right.PushBack({ 2559,111,176,331 });

		idle_right.loop = true;
		idle_right.speed = 0.5f;
	}

	//idle animation Left
	{
		idle_left.PushBack({ 64,1159,176,329 });
		idle_left.PushBack({ 311,1159,176,329 });
		idle_left.PushBack({ 577,1159,176,329 });
		idle_left.PushBack({ 848,1159,176,329 });
		idle_left.PushBack({ 1142,1159,176,329 });
		idle_left.PushBack({ 1455,1159,176,329 });
		idle_left.PushBack({ 1739,1159,176,329 });
		idle_left.PushBack({ 2022,1159,176,329 });
		idle_left.PushBack({ 2286,1159,176,329 });
		idle_left.PushBack({ 2560,1159,176,329 });

		idle_left.loop = true;
		//idle_left.speed = 0.001f;
		idle_left.speed = 0.5f;
	}

	
	// Move Right
	{
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
		right.speed = 0.5f;
	}

	//Move Right back
	right_back.PushBack({ 0,0,50,50 });

	right_back.loop = true;
	right_back.speed = 0.5f;

	// Move Left
	{
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
		left.speed = 0.5;
	}

	//Move Left Back
	left_back.PushBack({ 0,0,50,50 });

	left_back.loop = true;
	left_back.speed = 0.5f;

	//DIE
	die.PushBack({ 331,190,29,26 });
	die.PushBack({ 361,190,29,26 });
	die.PushBack({ 391,190,29,26 });
	die.loop =true;
	die.speed = 0.5f;

	//JUMP_RIGHT
	{
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

		jump.loop = false;
		jump.speed = 0.5f;
	}


}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Init Player config");
	bool ret = true;

	return ret;
}
// Load assets
bool j1Player::Start()
{
	LOG("Loading Player Sheet");

	graphics = App->tex->Load("assets/character/character.png");

	LOG("Loading Player Collider");
	Player_Collider = App->colliders->AddCollider({ position.x, position.y, 46, 250 }, COLLIDER_PLAYER, this);
	//font_score = App->fonts->Load("fonts/Lletres_1.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ./\ ", 2);

	//Init Screen vars
	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;
	win_scale = App->win->GetScale();

	//Init position vars
	position.x =win_width/2;
	position.y = 215;

	//Init Jump vars
	jump_height = 300;
	jump_vel = 10;
	gravity = 10;

	//acceleraation vars
	acceleration = 0;
	accel_counter = 0;

	//Init bools
	fall = false;
	Jump = false;

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
	if (Player_Collider != nullptr)
		//Player_Coll->to_delete = true;

	return true;
}
// Update: draw background
bool j1Player::Update(float dt)
{
	SDL_Event e;
	speed = 4;
	
	//LEFT
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (position.x >= speed)
		{
			position.x -= (speed + acceleration);
			Acceleration_Method();
		}

		

		if (current_animation != &left && !Jump)
		{
			left.Reset();
			current_animation = &left;
			player_last_direction = LEFT;
		}

	}
	//RIGHT
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		/*if (position.x < (int)win_width +400)
		{*/
			position.x += speed + acceleration;
			Acceleration_Method();
		/*}*/


		if (current_animation != &right && !Jump)
		{
			right.Reset();
			current_animation = &right;
			player_last_direction = RIGHT;
		}

	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		acceleration = 0;
		accel_counter = 0;
		speed = 8;
	}

	//JUMP
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !fall)
	{
		if (!Jump)
		{
			Pos_jump = position.y - jump_height;
			Jump = true;
		}

		if (current_animation != &jump)
		{
			jump.Reset();
			current_animation = &jump;
			player_last_direction = RIGHT;
		}
		
	}
	
	//Function that makes the Jump
	Jump_Method();

	//IDLE ANIMATIONS
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		switch (player_last_direction)
		{
		case j1Player::UP:
			current_animation = &idle_right;
			break;
		case j1Player::DOWN:
			current_animation = &idle_right;
			break;
		case j1Player::LEFT:
			current_animation = &idle_left;
			break;
		case j1Player::RIGHT:
			current_animation = &idle_right;
			break;
		default:
			break;
		}
	}

	//Player Colliders Position
	Player_Collider->SetPos(position.x, position.y);

	// Draw everything --------------------------------------
	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),0.5f);

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

	data.append_child("player").append_attribute("x") = position.x;
	data.append_child("player").append_attribute("y") = position.y;

	return true;
}


void j1Player::OnCollision(Collider* c1, Collider* c2) {
	
	//Jump methode
	if (c2->type == COLLIDER_FLOOR)
	{
		position.y -= gravity;
		Jump = false;
		fall = false;
	}
}

void j1Player::Jump_Method()
{
	if (!Jump) {
		position.y += gravity;
	}

	if (Jump == true && position.y != Pos_jump)
	{
		position.y -= jump_vel;
		if (position.y == Pos_jump)
		{
			Jump = false;
			fall = true;
		}
	}
}

void j1Player::Acceleration_Method()
{
	accel_counter += 1;
	if (accel_counter % 100 == 0)
	{
		acceleration += 2;
	}
}

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
