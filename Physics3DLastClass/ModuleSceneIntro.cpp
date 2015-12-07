#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//longitud circuit
	float road_width = 100.0f;
	float road_height = 5.0f;
	float circuit_x = 15.0f;
	float circuit_y = 80.0f;

	recta1.size.Set(road_width, road_height, circuit_x);
	recta1.SetPos(0, road_height / 2, circuit_x / 2);
	App->physics->AddBody(recta1, 0)->collision_listeners.add(this);

	vec3 hola(0, 0, 1);
	curva1.height = road_height;
	curva1.SetRotation(90, hola);
	curva1.radius = 10.0f;
	curva1.SetPos(-circuit_x, 0, 0);
	App->physics->AddBody(curva1, 0)->collision_listeners.add(this);;



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

	recta1.Render();
	curva1.Render();
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

