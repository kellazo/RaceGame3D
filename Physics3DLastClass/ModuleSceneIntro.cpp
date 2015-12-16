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
	circuit_x2 = 50.0f;
	road_width = 20.0f;
	road_height = 2.0f;
	high_map = 7.0f;
	//Level1
	pl1 = pl2 = 12.0f;
	pr1 = pr2 = -12.0f;
	tl1 = tl2 = tr1 = tr2 = 0.2f;
	pY_elevator = 7.0f;
	tY_elevator = 0.02f;
	//Level2
	pY_elevator2 = 14.0f;
	tY_elevator2 = 0.02f;

	road_size = vec3(road_width, road_height, circuit_x);
	cube_size = vec3(3, 2, 3);

	//Lvl3
	posX_bob = 0.0f;
	tempb = 0.1f;


	/*
	temp = 0.1f;
	posY_block = 7.0f;
	psupport = prod = pbob = NULL;
	posX_bob = 0.0f;
	tempb = 0.1f;
	pc_left1 = pc_left2 = pc_right1 = pc_right2 = NULL;
	tl1 = tl2 = tr1 = tr2 = t1 = t2 = tw1 = tw2 = two1 = two2 = 0.1f;
	py1 = py2 = -5.0f;
	pxw1 = 12.0f;
	pxw2 = 1.0f;
	pxwo1 = 6.0f;
	pxwo2 = -6.0f;
	*/
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

	//Level1

	CreatePolePosition(road_width, road_height, 0.0f, 50.0f);

	CreateCube(road_size, vec3(0, road_height / 2, circuit_x / 2));
	CreateCube(road_size, vec3(0, road_height / 2, circuit_x / 2 + circuit_x));
	CreateCube(vec3(1, 4, 1), vec3(9, 3, 45));
	
	elevator = CreateCube(road_size, vec3(0, 0, circuit_x / 2 + circuit_x * 2), false);
	p_elevator = App->physics->AddBody(elevator, 0);
	//Blocks motion on X axis
	c_left1 = CreateCube(cube_size, vec3(12, 3, 55), false);
	pc_left1 = App->physics->AddBody(c_left1, 0);

	c_right1 = CreateCube(cube_size, vec3(-12, 3, 65), false);
	pc_right1 = App->physics->AddBody(c_right1, 0);

	c_left2 = CreateCube(cube_size, vec3(12, 3, 85), false);
	pc_left2 = App->physics->AddBody(c_left2, 0);

	c_right2 = CreateCube(cube_size, vec3(12, 3, 75), false);
	pc_right2 = App->physics->AddBody(c_right2, 0);

	//Level2
	CreateCube(road_size, vec3(0, high_map, circuit_x / 2 + circuit_x * 3));
	CreateCube(vec3(1, 4, 1), vec3(9, 10, 45+50*3));
	CreateCube(vec3(1, 4, 1), vec3(7, 10, 45+50*3));
	CreateCube(vec3(road_width / 3, road_height, circuit_x / 3), vec3(-7, high_map, 209));
	CreateCube(vec3(road_width / 4, road_height, circuit_x/3+4.5f), vec3(0, high_map, 214+road_width/3), true, 90.0f, vec3(0, 1, 0));
	CreateCube(vec3(road_width / 5, road_height, circuit_x / 3+8.5f), vec3(8.5f, high_map, 220 + road_width / 3 + road_width / 2));
	
	elevator2 = CreateCube(road_size, vec3(0, high_map, circuit_x / 2 + circuit_x * 5), false);
	p_elevator2 = App->physics->AddBody(elevator, 0);

	//Level3
	CreateCube(road_size, vec3(0, high_map*2, circuit_x / 2 + circuit_x * 6));

	CreatePolePosition(road_width, high_map, high_map, circuit_x / 2 + circuit_x * 5.8);
	CreatePolePosition(road_width, high_map, high_map, circuit_x / 2 + circuit_x * 6);
	CreatePolePosition(road_width, high_map, high_map, circuit_x / 2 + circuit_x * 6.2);
	CreatePolePosition(road_width, high_map, high_map, circuit_x / 2 + circuit_x * 6.4);

	//Level4

	CreateRoad(road_width, high_map * 2.5, 425.0f);
	CreatePendulum(road_width, road_height, high_map * 3.5, 400.0f);
	CreatePendulum(road_width, road_height, high_map * 3.5, 425.0f);
	CreatePendulum(road_width, road_height, high_map * 3.5, 450.0f);



	//Level5
	CreateRoad(road_width, high_map * 3.5, 550.0f);
	block1 = PutBlock(high_map * 4.3, 530.0f);
	block2 = PutBlock(high_map * 4.3, 560.0f);


	//Level6
	CreateRoad(road_width, high_map * 4.5, 675.0f);
	CreatePolePosition(road_width, high_map*2.3, high_map*2.3, 675.0f);

	
	

	floor.size.Set(300.0f, 1.0f, 1500.0f);
	floor.SetPos(0, -1, 0);
	//floor.color = Black;
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

	//Level1
	//Motion of Cube blocks on X axis
	MoveCubeX();
	MoveElevators();

	//Level2


	//Level4
	MotionPendulum(high_map * 3.5, 400.0f);
	MotionPendulum(high_map * 3.5, 425.0f);
	MotionPendulum(high_map * 3.5, 450.0f);

	//Level5
	MovementBlock(block1, 530.0f);
	MovementBlock(block2, 560.0f);
	
	
	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");

	//App->player->vehicle->SetPos(0, 3, 10);
	App->player->vehicle->SetTransform(&App->player->ini_trans);
	App->player->break_start = true;
		
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

void ModuleSceneIntro::MoveCubeX(){
	pc_left1->GetTransform(&c_left1.transform);
	pc_right1->GetTransform(&c_right1.transform);
	pc_left2->GetTransform(&c_left2.transform);
	pc_right2->GetTransform(&c_right2.transform);

	if (pl1 >= 12)
		tl1 = tl1 * -1;
	if (pl1 <= -12)
		tl1 = tl1 * -1;
	pl1 += tl1;
	pc_left1->SetPos(pl1, 3, 65);

	if (pr1 >= -12)
		tr1 = tr1 * -1;
	if (pr1 <= 12)
		tr1 = tr1 * -1;
	pr1 += tr1;

	pc_right1->SetPos(pr1, 3, 55);

	if (pl2 >= 12)
		tl2 = tl2 * -1;
	if (pl2 <= -12)
		tl2 = tl2 * -1;
	pl2 += tl2;
	pc_left2->SetPos(pl2, 3, 85);

	if (pr2 >= -12)
		tr2 = tr2 * -1;
	if (pr2 <= 12)
		tr2 = tr2 * -1;
	pr2 += tr2;
	pc_right2->SetPos(pr2, 3, 75);

	c_left1.Render();
	c_right1.Render();
	c_left2.Render();
	c_right2.Render();
}

void ModuleSceneIntro::MoveElevators(){
	p_elevator->GetTransform(&elevator.transform);
	if (pY_elevator >= high_map)
		tY_elevator = tY_elevator * -1;
	if (pY_elevator <= 0)
		tY_elevator = tY_elevator * -1;
	pY_elevator += tY_elevator;

	p_elevator->SetPos(0,pY_elevator, circuit_x / 2 + circuit_x * 2);
	elevator.Render();

	p_elevator2->GetTransform(&elevator2.transform);
	if (pY_elevator2 >= high_map*2)
		tY_elevator2 = tY_elevator2 * -1;
	if (pY_elevator2 <= high_map)
		tY_elevator2 = tY_elevator2 * -1;
	pY_elevator2 += tY_elevator2;

	p_elevator2->SetPos(0, pY_elevator2, circuit_x / 2 + circuit_x * 5);
	elevator2.Render();

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

			/*if (posX_bob >= 7)
				tempb = tempb * -1;
			if (posX_bob <= -7)
				tempb = tempb * -1;
			posX_bob += tempb;
			tmp5->data->SetPos(posX_bob, positionY, positionZ);
			tmp7->data->SetPos(posX_bob, positionY, positionZ);*/

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

		if (posY_block >= 20)
			temp = temp * -1;
		if (posY_block <= 6)
			temp = temp * -1;
		posY_block += temp;
		//body->GetTransform(&(tmp2->data->transform));
		//body->SetPos(0, posY_block, posZ);
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



















/*

//blocks from ground and up

blockground1.size.Set(5, 5, 5);
blockground1.SetPos(3, -4, circuit_x / 2 + circuit_x * 2);
pblockground1 = App->physics->AddBody(blockground1, 0);

blockground2.size.Set(5, 5, 5);
blockground2.SetPos(-3, -4, circuit_x / 2 + circuit_x * 2);
pblockground2 = App->physics->AddBody(blockground2, 0);

//2 wall on middle

wall_mid1.size.Set(2, 8, 10);
wall_mid1.SetPos(12, 6, circuit_x / 2 + circuit_x * 2 + 25);
pwall_mid1 = App->physics->AddBody(wall_mid1, 0);

wall_mid2.size.Set(2, 8, 10);
wall_mid2.SetPos(-12, 6, circuit_x / 2 + circuit_x * 2 + 25);
pwall_mid2 = App->physics->AddBody(wall_mid2, 0);

// 2 wall out

wall_outl.size.Set(2, 8, 8);
wall_outl.SetPos(12, 4, circuit_x / 2 + circuit_x * 3);
pwall_outl = App->physics->AddBody(wall_outl, 0);

wall_outr.size.Set(2, 8, 8);
wall_outr.SetPos(-12, 4, circuit_x / 2 + circuit_x * 3);
pwall_outr = App->physics->AddBody(wall_outr, 0);

//flippers wall

//hole road
*/







//blocks from ground and up

//TODO: fer forats al terra i fer que puguin els blocks  perque si hi ha un objecte fisic xoca i no travessa
/*pblockground1->GetTransform(&blockground1.transform);
pblockground2->GetTransform(&blockground2.transform);

if (py1 >= 10)
t1 = t1 * -1;
if (py1 <= -4)
t1 = t1 * -1;
py1 += t1;
blockground1.SetPos(3, py1, circuit_x / 2 + circuit_x * 2);
pblockground1->SetPos(-3, py1, circuit_x / 2 + circuit_x * 2);
blockground1.Render();

if (py2 >= 10)
t2 = t2 * -1;
if (py2 <= -4)
t2 = t2 * -1;
py2 += t2;
blockground2.SetPos(-3, py2, circuit_x / 2 + circuit_x * 2);
pblockground2->SetPos(-3, py2, circuit_x / 2 + circuit_x * 2);
blockground2.Render();*/


//2 wall on middle
//pwall_mid1->GetTransform(&wall_mid1.transform);
//pwall_mid2->GetTransform(&wall_mid2.transform);

/*if (pxw1 >= 12)
tw1 = tw1 * -1;
if (pxw1 <= -12)
tw1 = tw1 * -1;
pxw1 += tw1;

wall_mid1.SetPos(pxw1, 6, circuit_x / 2 + circuit_x * 2 + 25);
*/
/*
pwall_mid1 = App->physics->AddBody(wall_mid1, 0);

wall_mid1.Render();
*/

/*if (pxw2 >= 1)
tw2 = tw2 * -1;
if (pxw2 <= -10)
tw2 = tw2 * -1;
pxw2 += tw2;
wall_mid2.SetPos(pxw2, 6, circuit_x / 2 + circuit_x * 2 + 25);
pwall_mid2 = App->physics->AddBody(wall_mid2, 0);*/
/*
wall_mid2.Render();
*/
// 2 wall out
/*pwall_outl->GetTransform(&wall_outl.transform);
pwall_outr->GetTransform(&wall_outr.transform);

if (pxwo1 >= 10)
two1 = two1 * -1;
if (pxwo1 <= -4)
two1 = two1 * -1;
pxwo1 += two1;
wall_outl.SetPos(pxwo1, 3, circuit_x / 2 + circuit_x * 3);
pwall_outl = App->physics->AddBody(wall_outl, 0);
wall_outl.Render();

if (pxwo2 >= 10)
two2 = two2 * -1;
if (pxwo2 <= -4)
two2 = two2 * -1;
pxwo2 += two2;
wall_outr.SetPos(pxwo2, 3, circuit_x / 2 + circuit_x * 3);
pwall_outr = App->physics->AddBody(wall_outr, 0);
wall_outr.Render();*/

//flippers wall

//hole road