#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	vel,red,blue = 0;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(0.0f, 13.0f, -10.0f));
	App->camera->LookAt(vec3(0, 5, 6));
	//longitud circuit
	float road_width = 20.0f;
	float road_height = 2.0f;
	float circuit_x = 50.0f;
	recta1.size.Set(road_width, road_height, circuit_x);
	recta1.SetPos(0, road_height / 2, circuit_x / 2);
	App->physics->AddBody(recta1, 0)->collision_listeners.add(this);

	recta2.size.Set(road_width, road_height, 10);
	recta2.SetPos(0, road_height / 2, 56);
	App->physics->AddBody(recta2, 0)->collision_listeners.add(this);

	vec3 vec(0, 0, 1);
	poste1.height = 10.0f;
	poste1.SetRotation(90, vec);
	poste1.radius = 1.0f;
	poste1.SetPos(-9, 7, 56);
	App->physics->AddBody(poste1, 0);


	poste2.height = 10.0f;
	poste2.SetRotation(90, vec);
	poste2.radius = 1.0f;
	poste2.SetPos(9, 7, 56);
	App->physics->AddBody(poste2, 0);

	cartell.size.Set(road_width, 3, 2);
	cartell.SetPos(0, 13.5, 56);
	App->physics->AddBody(cartell, 0)->collision_listeners.add(this);

	
	
	curva2.height = 2.0f;
	curva2.SetRotation(90, vec);
	curva2.radius = 7.0f;
	curva2.SetPos(0, 1, 70);
	App->physics->AddBody(curva2, 0);

	block.size.Set(5, 5, 5);
	//block.SetPos(0, 13, 20);
	pbblock = App->physics->AddBody(block, 0);


	floor.size.Set(500.0f, 1.0f, 500.0f);
	floor.SetPos(0, -1, 0);
	floor.color = Black;

	//floor.size.Set(5, 3, 1);
	//floor.SetPos(0.0f, 4.5f, 20.0f);

	floor_sensor = App->physics->AddBody(floor, 0.0f);
	floor_sensor->SetAsSensor(true);
	floor_sensor->collision_listeners.add(this);



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	//floor sensor
	floor_sensor->GetTransform(&floor.transform);
	floor.Render();

	/*p2List_item<PhysBody3D*>* item = pbpieces.getFirst();
	p2List_item<Primitive>* item_primitive = pieces.getFirst();
	while (item_primitive != NULL)
	{
		item->data->GetTransform(&(item_primitive->data.transform));
		item_primitive->data.Render();
		

		item = item->next;
		item_primitive = item_primitive->next;
		
	}*/

	recta1.Render();
	recta2.Render();
	poste1.Render();
	poste2.Render();
	cartell.Render();
	curva2.Render();

	for (red = 0; red <= 20; red++)
	{
		
		pbblock->GetTransform(&(block.transform));
		//block.SetPos(0, red, 20);
		//pbblock->Push(red, 0, 0);
		block.Render();
		
	}
	
	//block.color.Set(100, 0, 50);
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");

	//App->player->vehicle->SetPos(0, 3, 10);
	App->player->vehicle->SetTransform(&App->player->ini_trans);
	App->player->break_start = true;
		
}

