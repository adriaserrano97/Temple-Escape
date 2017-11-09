#include "j1App.h"
#include "Bat.h"
#include "j1Collider.h"
#include "j1Map.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
//#include "Path.h"

Bat::Bat(int x, int y) : Enemy(x, y)
{
	bat_IA = 1;
	bat_going_right = true;
	moving = player_in_radar = false;
	have_to_chill = false; //check this

	fly_r.PushBack({ 76, 35, 16, 12 });
	fly_r.PushBack({ 92, 35, 16, 12 });
	fly_r.PushBack({ 109, 35, 16, 12 });
	fly_r.speed = 0.15f;
	fly_r.loop = true;

	fly_l.PushBack({ 47, 182, 16, 12 });
	fly_l.PushBack({ 31, 182, 16, 12 });
	fly_l.PushBack({ 14, 182, 16, 12 });
	fly_l.speed = 0.15f;
	fly_l.loop = true;

	animation = &fly_r;

	original_pos = App->map->WorldToMap(x, y);

	collider = App->collider->AddCollider({ 0, 0, 16, 12 }, COLLIDER_TYPE::COLLIDER_ENEMY_BAT, (j1Module*)App->enemies);

	SetRadar();
}

void Bat::Move()
{
	iPoint bat_pos = App->map->WorldToMap(position.x, position.y);

	if (have_to_chill && !player_in_radar && !moving) {

		App->pathfinding->CreatePath(iPoint(bat_pos.x, bat_pos.y), original_pos);
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		if (path->Count() > 0) {
			movementGoal = iPoint(path->At(0)->x, path->At(0)->y);
			//TODO FOLLOW THIS..
			//ill do this working around NO DIAGONALS so this will need an update
			if (path->Count() == 1)
				movementGoal.y--;

			if (movementGoal.x < bat_pos.x) {
				movementSpeed = { -0.5f,0.0f };
				animation = &fly_l;
			}
			else if (movementGoal.x > bat_pos.x) {
				movementSpeed = { 0.5f,0.0f };
				animation = &fly_r;
			}
			else if (movementGoal.y < bat_pos.y) {
				movementSpeed = { 0.0f,-0.5f };
			}
			else if (movementGoal.y > bat_pos.y) {
				movementSpeed = { 0.0f, 0.5f };
			}
			moving = true;
		}
		else {
			bat_IA = 1;
			have_to_chill = false;
		}
			
	}else if (player_in_radar && !moving) {
		App->pathfinding->CreatePath(iPoint(bat_pos.x, bat_pos.y), playerGoal);
		LOG("x : %i y : %i", bat_pos.x, bat_pos.y);
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		movementGoal = iPoint(path->At(0)->x, path->At(0)->y);
		LOG("goal x : %i goal y : %i", movementGoal.x, movementGoal.y);
		//TODO FOLLOW THIS..
		//ill do this working around NO DIAGONALS so this will need an update

		if (movementGoal.x < bat_pos.x) {
			movementSpeed = { -0.5f,0.0f };
			animation = &fly_l;
		}
		else if (movementGoal.x > bat_pos.x) {
			movementSpeed = { 0.5f,0.0f };
			animation = &fly_r;
		}
		else if (movementGoal.y < bat_pos.y) {
			movementSpeed = { 0.0f,-0.5f };
		}
		else if (movementGoal.y > bat_pos.y) {
			movementSpeed = { 0.0f, 0.5f };
		}
		moving = true;
	}
	else if (bat_going_right && !moving) {

		iPoint goal = App->map->WorldToMap(position.x, position.y);

		goal.x += 1;
		movementGoal = goal;
		moving = true;
		bat_IA++;
		movementSpeed = { 0.5f,0.0f };
		animation = &fly_r;

	}
	else if (!bat_going_right && !moving) {

		iPoint goal = App->map->WorldToMap(position.x, position.y);
		goal.x -= 1;
		movementGoal = goal;
		moving = true;
		bat_IA--;
		movementSpeed = { -0.5f,0.0f };
		animation = &fly_l;

	}
	else {
		if (moving) {
			position = position + movementSpeed;
		}

		if (bat_pos == movementGoal) {
			moving = false;
			if (bat_IA == 3 || bat_IA == 0)
				bat_going_right = !bat_going_right;
			
			if(!App->player->isDead)
				player_in_radar = CheckForPlayer();

			if (!have_to_chill && player_in_radar)
				have_to_chill = true;
		}
	}
	LOG("BAT POS x : %i y : %i", bat_pos.x, bat_pos.y);
	LOG("MOV GOAL x : %i goal y : %i", movementGoal.x, movementGoal.y);
	LOG("ORIGINAL POS X: %i | ORIGINAL POS Y: %i", original_pos.x, original_pos.y);
	
}

void Bat::SetRadar() {
	uint counter = 0;
	
	for (int i = -3; i < 4; i++)
	{
		for (int k = -3; k < 4; k++)
		{
			iPoint tmp_radar = App->map->WorldToMap(position.x, position.y);
			//LOG("x : %i y : %i",tmp_radar.x,tmp_radar.y);
			tmp_radar.x += i;
			tmp_radar.y += k;
			tile_radar[counter++] = tmp_radar;
		}
	}
}

bool Bat::CheckForPlayer() {
	iPoint tmp_player = App->map->WorldToMap(App->player->position.x, App->player->position.y);

	for (uint i = 0; i < TILE_RADAR; i++)
	{
		if (tile_radar[i] == tmp_player) {
			LOG("NANANANANANANA BAAAAT RADAAAAR ~ PLAYER IS IN THE BAT RADAR!!!");
			playerGoal = tile_radar[i];
			if (playerGoal == iPoint(App->map->WorldToMap(position.x, position.y)))
				return false;
			return true;
		}
	}
	return false;
}

void Bat::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_TYPE::COLLIDER_PLAYER) {
		player_in_radar = false;
		have_to_chill = true;
		//App->particles->AddParticle(App->particles->playerShotCollison, (collider->rect.x - (((collider->rect.w)) / 2)), (collider->rect.y - (((collider->rect.h)))));
		//animation = &bee_white;
	}
}