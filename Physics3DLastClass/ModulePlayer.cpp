#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	start = false;
	win = false;
	lose = false;

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 01.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 3, 10);
	vehicle->GetTransform(&ini_trans);
	xpos = vec3(0.0f, 0.0f, 1.0f);



	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		start = true;
	}


		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			start = false;
			win = false;
			lose = false;
			App->scene_intro->time = TIME;
			App->player->vehicle->ResetVelocity();
			App->player->vehicle->SetTransform(&App->player->ini_trans);


			p2List_item<PhysBody3D*>* tmp = App->scene_intro->spheres_body.getFirst();
			
			for (tmp; tmp != NULL; tmp = tmp->next){
					tmp->data->SetPos(tmp->data->GetPos().x, tmp->data->GetPos().y - 6, tmp->data->GetPos().z);
					
			}
			App->scene_intro->spheres_body.clear();
			App->scene_intro->CreateRandSpheres(NUM_BALLS);
		}
	if (start){
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			acceleration = MAX_ACCELERATION * 4;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			acceleration = -MAX_ACCELERATION * 4;
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_REPEAT)
		{
			brake = BRAKE_POWER;
		}
	}

	if (App->scene_intro->spheres_body.count() == 0){
		win = true;
	}
	if (App->scene_intro->time <= 0.0f){
		App->scene_intro->time == 0.0f;
		lose = true;
	}
	if (win==false && lose==false){
		char title[80];
		sprintf_s(title, "Time left: %f, Balls left: %d/%d, Best Time: %f", App->scene_intro->time, App->scene_intro->spheres_body.count(), NUM_BALLS, App->scene_intro->best_time);
		App->window->SetTitle(title);
	}
	if (win){
		start = false;
		brake = BRAKE_POWER;
		App->scene_intro->best_time = MAX(App->scene_intro->best_time, App->scene_intro->time);
		char title[80];
		sprintf_s(title, "YOU WIN! Time left: %f, Balls left: %d/%d, Best Time: %f", App->scene_intro->time, App->scene_intro->spheres_body.count(), NUM_BALLS, App->scene_intro->best_time);
		App->window->SetTitle(title);
	}
	if (lose){
		start = false;
		brake = BRAKE_POWER;
		char title[80];
		sprintf_s(title, "YOU LOSE! Time left: %f, Balls left: %d/%d, Best Time: %f", App->scene_intro->time, App->scene_intro->spheres_body.count(), NUM_BALLS, App->scene_intro->best_time);
		App->window->SetTitle(title);
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();
	return UPDATE_CONTINUE;
}



