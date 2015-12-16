#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circuit_x = 50.0f;
	road_width = 20.0f;
	road_height = 2.0f;
	high_map = 7.0f;
	

	road_size = vec3(road_width, road_height, circuit_x);
	cube_size = vec3(3, 2, 3);

	


	
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

	
	
	
	//Floor lvl1
	floor = CreateCube(vec3(300.0f, 1.0f, 1500.0f), vec3(0, -1, 0), false);
	

	floor_sensor = App->physics->AddBody(floor, 0.0f);
	floor_sensor->SetAsSensor(true);
	floor_sensor->collision_listeners.add(this);
	
	//CheckPoint
	/*cube_checkpoint_lvl2 = CreateCube(vec3(road_width, 1, 1), vec3(0.0f, 10.0f, circuit_x*3), false);

	sensor_checkpoint_lvl2 = App->physics->AddBody(cube_checkpoint_lvl2, 0.0f);
	sensor_checkpoint_lvl2->SetAsSensor(true);
	sensor_checkpoint_lvl2->collision_listeners.add(this);*/




	CreateRoad(road_width, 0.f, 25.0f);
	CreateRoad(road_width, 0.f, 75.0f);
	CreateRoad(road_width, 0.f, 75.0f + 50.0f);
	CreateRoad(road_width, 0.f, 75.0f + 50.0f*2);
	CreateRoad(road_width, 0.f, 75.0f + 50.0f * 3);


	ramp1 = CreateCube(vec3(road_width, road_height, circuit_x), vec3(0.0f, 2.5f, 70.0f + 50.0f * 4 - 10.f), true, -10.0f, vec3(1,0,0));
	ramp2 = CreateCube(vec3(road_width, road_height, circuit_x), vec3(0.0f, 2.5f + 12.0f, 70.0f + 50.0f * 5 - 12.f), true, -20.0f, vec3(1, 0, 0));
	//ramp3 = CreateCube(vec3(road_width, road_height, circuit_x), vec3(0.0f, 30.0f, 70.0f + 50.0f * 6), true, -30.0f, vec3(1, 0, 0));
	//ramp4 = CreateCube(vec3(road_width, road_height, circuit_x), vec3(0.0f, 50.0f, 70.0f + 50.0f * 7), true, -40.0f, vec3(1, 0, 0));

	




	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (p2List_item<Cube*>* tmp = cube_list.getFirst(); tmp != NULL; tmp = tmp->next){
		delete tmp->data;
	}
	for (p2List_item<Cube*>* tmp = cube_list_move.getFirst(); tmp != NULL; tmp = tmp->next){
		delete tmp->data;
	}
	for (p2List_item<Cylinder*>* tmp = cylinders.getFirst(); tmp != NULL; tmp = tmp->next){
		delete tmp->data;
	}
	for (p2List_item<Sphere*>* tmp = spheres.getFirst(); tmp != NULL; tmp = tmp->next){
		delete tmp->data;
	}
	for (p2List_item<PhysBody3D*>* tmp = pbpieces.getFirst(); tmp != NULL; tmp = tmp->next){
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
	
	//floor sensor
	floor_sensor->GetTransform(&floor.transform);
	floor.Render();

	
	//sensor_checkpoint_lvl2->GetTransform(&floor.transform);
	//cube_checkpoint_lvl2.Render();

	
	
	
	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");
	if (body1 == floor_sensor){
		App->player->vehicle->ResetVelocity();
		App->player->vehicle->SetTransform(&App->player->ini_trans);
	}

	/*if (body1 == sensor_checkpoint_lvl2){
		App->player->checkpoint_lvl2 = true;
		App->player->vehicle->GetTransform(&App->player->lvl2_trans);
	}*/
}

Cube ModuleSceneIntro::CreateCube(const vec3 size, const vec3 position, bool phys, float angle, const vec3 axis){
	Cube* new_cube = new Cube();
	new_cube->size.Set(size.x,size.y,size.z);

	if (angle != 0.0f)
		new_cube->SetRotation(angle, axis);

	new_cube->SetPos(position.x, position.y, position.z);
	
	if (phys==false){
		cube_list_move.add(new_cube);
	}
	else{
		App->physics->AddBody(*new_cube, 0);
		cube_list.add(new_cube);
	}
	return *new_cube;
}

void ModuleSceneIntro::MoveCube(PhysBody3D& body, Cube cube, float motion, float velocity, bool right){
	static int i = 1;
	if (i==1){
	body.velocity = velocity;
	if (right){
		body.motion_position = -motion;
	}
	else{
		body.motion_position = motion;
	}
	i++;
	}
	body.GetTransform(&cube.transform);
	if (right == false){
		
		if (body.motion_position >= motion)
			body.velocity = body.velocity * -1;
		if (body.motion_position <= -motion)
			body.velocity = body.velocity * -1;
	}
	else{
		if (body.motion_position >= -motion)
			body.velocity = body.velocity * -1;
		if (body.motion_position <= motion)
			body.velocity = body.velocity * -1;
	}
	body.motion_position += body.velocity;
	body.SetPos(body.motion_position, body.GetPos().y, body.GetPos().z);

	cube.Render();

}


//With this function you can put finish and start meta
void ModuleSceneIntro::CreatePolePosition(const float road_width, const float road_height, const float positionY, const float positionZ)
{
	
	Cylinder* pole_left = new Cylinder();
	vec3 vec(0, 0, 1);
	pole_left->height = 10.0f;
	pole_left->SetRotation(90, vec);
	pole_left->radius = 1.0f;
	pole_left->SetPos(road_width / 2 - 1, (pole_left->height / 2) + road_height + positionY, positionZ);
	App->physics->AddBody(*pole_left, 0);

	Cylinder* pole_right = new Cylinder();
	pole_right->height = pole_left->height;
	pole_right->SetRotation(90, vec);
	pole_right->radius = pole_left->radius;
	pole_right->SetPos(-((road_width / 2) - 1), (pole_right->height / 2) + road_height + positionY, positionZ);
	App->physics->AddBody(*pole_right, 0);

	Cube* poster = new Cube();
	poster->size.Set(road_width, 3, 2);
	poster->SetPos(0, (pole_right->height / 2)*2 + road_height + positionY, positionZ);
	App->physics->AddBody(*poster, 0)->collision_listeners.add(this);

	cube_list.add(poster);
	cylinders.add(pole_left);
	cylinders.add(pole_right);


}

//This method create a Pendelum
void ModuleSceneIntro::CreatePendulum(const float road_width, const float road_height, const float positionY, const float positionZ)
{

	PhysBody3D* psupport;
	PhysBody3D* prod;
	PhysBody3D* pbob;
	Cube* support = new Cube();
	Cylinder* rod = new Cylinder();
	Sphere* bob = new Sphere();
	
	vec3 vec(0, 0, 1);
	rod->height = 12.0f;
	rod->SetRotation(90, vec);
	rod->radius = 1.0f;
	rod->SetPos(0, (rod->height / 2 - 1.5)  + road_height + positionY, positionZ);
	prod = App->physics->AddBody(*rod, 1);

	support->size.Set(10.0f, 2.0f, 4.0f);
	
	support->SetPos(0, (rod->height / 2) * 2 + road_height + positionY, positionZ);
	psupport = App->physics->AddBody(*support, 0);

	vec3 anchorA(0, -1.5, 0);
	vec3 anchorB(6, 0, 0);

	vec3 axisA(0, 0, 1);
	vec3 axisB(0, 0, 1);

	App->physics->AddConstraintHinge(*(psupport), *(prod), anchorA, anchorB, axisA, axisB);

	bob->radius = 2.0f;
	bob->SetPos(0, (rod->height / 4)  - 4.5 + positionY, positionZ);
	pbob = App->physics->AddBody(*bob, 1);

	vec3 anchorAr(-7, 0, 0);
	vec3 anchorBb(0, -1, 0);
	App->physics->AddConstraintHinge(*(prod), *(pbob), anchorAr, anchorBb, axisA, axisB);


	cube_list.add(support);
	spheres_move.add(bob);
	cylinders_move.add(rod);

	pbpieces_cylinders_move.add(prod);
	pbpieces_spheres_move.add(pbob);

}

//Iterate for objects statics and render them
void ModuleSceneIntro::RenderStaticPieces()
{

	for (p2List_item<Cube*>* tmp = cube_list.getFirst(); tmp != NULL; tmp = tmp->next){

		tmp->data->Render();
	}

	p2List_item<Cylinder*>* tmp2 = cylinders.getFirst();

	for (tmp2; tmp2 != NULL; tmp2 = tmp2->next){

		tmp2->data->Render();
	}

	p2List_item<Sphere*>* tmp3 = spheres.getFirst();

	for (tmp3; tmp3 != NULL; tmp3 = tmp3->next){

		tmp3->data->Render();
	}

}

//This method apply velocity on X axis whatever pendulum thaht you have create before.  PD: PositionY and Z it's the same on method CreatePendulum
void ModuleSceneIntro::MotionPendulum(const float positionY, const float positionZ)
{


	p2List_item<PhysBody3D*>* tmp6 = pbpieces_cylinders_move.getFirst();
	p2List_item<Cylinder*>* tmp4 = cylinders_move.getFirst();
	p2List_item<PhysBody3D*>* tmp7 = pbpieces_spheres_move.getFirst();
	p2List_item<Sphere*>* tmp5 = spheres_move.getFirst();

	for (tmp4; tmp4 != NULL; tmp4 = tmp4->next){
		for (tmp6; tmp6 != NULL; tmp6 = tmp6->next){
			tmp6->data->GetTransform(&(tmp4->data->transform));
			tmp4->data->Render();
		}
	}

	for (tmp5; tmp5 != NULL; tmp5 = tmp5->next){
		for (tmp7; tmp7 != NULL; tmp7 = tmp7->next){
			tmp7->data->GetTransform(&(tmp5->data->transform));
			tmp5->data->Render();
		}
	}
}

//Generate a Block whatever you want
PhysBody3D* ModuleSceneIntro::PutBlock(const float positionY, const float positionZ)
{
	Cube* block = new Cube();
	block->size.Set(5, 10, 5);
	block->SetPos(0, positionY, positionZ);
	block_list_move.add(block);
	return App->physics->AddBody(*block, 0);

}

//Call this function on Update and you can define a velocity
void ModuleSceneIntro::MovementBlock(PhysBody3D* body, float posZ)
{
	
	p2List_item<Cube*>* tmp2 = block_list_move.getFirst();

	for (tmp2; tmp2 != NULL; tmp2 = tmp2->next){
		tmp2->data->Render();
	}

	
}

//Create anyone road on Z axis
void ModuleSceneIntro::CreateRoad(const float road_width, const float positionY, const float positionZ)
{
	Cube* road = new Cube();
	road->size.Set(road_width, road_height, circuit_x);
	road->SetPos(0, road_height / 2 + positionY, positionZ);
	App->physics->AddBody(*road, 0)->collision_listeners.add(this);
	cube_list.add(road);

}



















