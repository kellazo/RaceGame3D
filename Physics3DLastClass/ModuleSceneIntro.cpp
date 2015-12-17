#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

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

	App->camera->Move(vec3(0.0f, 13.0f, -10.0f));
	App->camera->LookAt(vec3(0, 5, 6));
	
	//Floor
	
	CreateCube(vec3(150.0f, 1.0f, 150.0f), vec3(0, 0, 0));
	//floor.color.Set(255, 255, 0, 0.5f);

	CreateSphere(vec3(0.0f, 3.0f, 20.0f), 2);
	CreateSphere(vec3(0.0f, 3.0f, 25.0f), 2);
	CreateSphere(vec3(0.0f, 3.0f, 30.0f), 2);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (p2List_item<Cube*>* tmp = cube_list.getFirst(); tmp != NULL; tmp = tmp->next){
		delete tmp->data;
	}
	
	for (p2List_item<Sphere*>* tmp = spheres.getFirst(); tmp != NULL; tmp = tmp->next){
		delete tmp->data;
	}
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	
	RenderStaticPieces();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	p2List_item<Sphere*>* tmp1 = spheres.getFirst();
	p2List_item<PhysBody3D*>* tmp2 = spheres_body.getFirst();
	int i = 0;
	for (tmp2; tmp2 != NULL; tmp2 = tmp2->next){
		
		if (i == spheres_body.find(body1)){
			spheres.del(tmp1);
			spheres_body.del(tmp2);
			break;
		}
		tmp1=tmp1->next;
	}
}

Cube ModuleSceneIntro::CreateCube(const vec3 size, const vec3 position, bool phys, float angle, const vec3 axis){
	Cube* new_cube = new Cube();
	new_cube->size.Set(size.x,size.y,size.z);
	if (angle != 0.0f)
		new_cube->SetRotation(angle, axis);

	new_cube->SetPos(position.x, position.y, position.z);
	
	App->physics->AddBody(*new_cube, 0);
	cube_list.add(new_cube);

	return *new_cube;
}

//Iterate for objects statics and render them
void ModuleSceneIntro::RenderStaticPieces()
{

	for (p2List_item<Cube*>* tmp = cube_list.getFirst(); tmp != NULL; tmp = tmp->next){

		tmp->data->Render();
	}

	p2List_item<Sphere*>* tmp1 = spheres.getFirst();
	p2List_item<PhysBody3D*>* tmp2 = spheres_body.getFirst();
	for (tmp2; tmp2 != NULL; tmp2 = tmp2->next){
		tmp2->data->GetTransform(&tmp1->data->transform);
		tmp1->data->Render();
	}
	
}

void ModuleSceneIntro::CreateSphere(const vec3& position, float radius)
{
	Sphere* new_sphere = new Sphere();
	new_sphere->radius = radius;
	new_sphere->SetPos(position.x, position.y, position.z);
	spheres.add(new_sphere);

	PhysBody3D* new_sphere_body;
	new_sphere_body = App->physics->AddBody(*new_sphere, 0);
	new_sphere_body->SetAsSensor(true);
	new_sphere_body->collision_listeners.add(this);
	spheres_body.add(new_sphere_body);

	
}














