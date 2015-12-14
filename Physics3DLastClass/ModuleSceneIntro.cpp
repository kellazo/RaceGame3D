#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	temp = 0.1f;
	posY_block = 7.0f;
	psupport = prod = pbob = NULL;
	posX_bob = 0.0f;
	tempb = 0.1f;
	pl1 = pl2 = 12.0f;
	pr1 = pr2 = -12.0f;
	tl1 = tl2 = tr1 = tr2 = t1 = t2 = tw1 = tw2 = two1 = two2 = 0.1f;
	pc_left1 = pc_left2 = pc_right1 = pc_right2 = NULL;
	circuit_x = 50.0f;
	py1 = py2 = -5.0f;
	pxw1 = 12.0f;
	pxw2 = 1.0f;
	pxwo1 = 6.0f;
	pxwo2 = -6.0f;
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
	//float circuit_x = 50.0f;

	//pole position & first block
	road_start.size.Set(road_width, road_height, circuit_x);
	road_start.SetPos(0, road_height / 2, circuit_x / 2);
	App->physics->AddBody(road_start, 0)->collision_listeners.add(this);

	vec3 vec(0, 0, 1);
	pole_left.height = 10.0f;
	pole_left.SetRotation(90, vec);
	pole_left.radius = 1.0f;
	pole_left.SetPos(-9, 7, circuit_x / 2);
	App->physics->AddBody(pole_left, 0);


	pole_right.height = 10.0f;
	pole_right.SetRotation(90, vec);
	pole_right.radius = 1.0f;
	pole_right.SetPos(9, 7, circuit_x / 2);
	App->physics->AddBody(pole_right, 0);

	poster.size.Set(road_width, 3, 2);
	poster.SetPos(0, 13.5, circuit_x / 2);
	App->physics->AddBody(poster, 0)->collision_listeners.add(this);

	road_path1.size.Set(road_width, road_height, circuit_x);
	road_path1.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x);
	App->physics->AddBody(road_path1, 0)->collision_listeners.add(this);

	block.size.Set(5, 5, 5);
	block.SetPos(0, 20, circuit_x);
	pbblock = App->physics->AddBody(block, 0);

	// Simple Pendulum

	support.size.Set(10.0f, 2.0f, 4.0f);
	support.SetPos(0, 20, circuit_x + (circuit_x / 2));
	psupport = App->physics->AddBody(support, 0);

	rod.height = 12.0f;
	rod.SetRotation(90, vec);
	rod.radius = 1.0f;
	rod.SetPos(0, 20, ((circuit_x + circuit_x / 2)));
	prod = App->physics->AddBody(rod, 1);

	vec3 anchorA(0, -1.5, 1);
	vec3 anchorB(6, 0, 0);

	vec3 axisA(0, 0, 1);
	vec3 axisB(0, 0, 1);

	//App->physics->AddConstraintP2P(*(psupport),*(prod), anchorA,anchorB);
	App->physics->AddConstraintHinge(*(psupport), *(prod), anchorA, anchorB, axisA, axisB);

	bob.radius = 2.0f;
	bob.SetPos(-10, 5, ((circuit_x + circuit_x / 2)));
	pbob = App->physics->AddBody(bob, 1);

	vec3 anchorAr(-7, 0, 0);
	vec3 anchorBb(0, -1, 0);
	App->physics->AddConstraintHinge(*(prod), *(pbob), anchorAr, anchorBb, axisA, axisB);

	//Road blocks motion on X axis
	road_path2.size.Set(road_width, road_height, circuit_x);
	road_path2.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x + circuit_x);
	App->physics->AddBody(road_path2, 0)->collision_listeners.add(this);

	c_left1.size.Set(3, 2, 3);
	c_left1.SetPos(12, 3, circuit_x / 2 + circuit_x + 10);
	App->physics->AddBody(c_left1, 0);

	c_left2.size.Set(3, 2, 3);
	c_left2.SetPos(12, 3, circuit_x / 2 + circuit_x + 20);
	App->physics->AddBody(c_left2, 0);

	c_right1.size.Set(3, 2, 3);
	c_right1.SetPos(-12, 3, circuit_x / 2 + circuit_x + 15);
	App->physics->AddBody(c_right1, 0);

	c_right2.size.Set(3, 2, 3);
	c_right2.SetPos(-12, 3, circuit_x / 2 + circuit_x + 25);
	App->physics->AddBody(c_right2, 0);

	pc_left1 = App->physics->AddBody(c_left1, 0);
	pc_left2 = App->physics->AddBody(c_left2, 0);
	pc_right1 = App->physics->AddBody(c_right1, 0);
	pc_right2 = App->physics->AddBody(c_right2, 0);

	//more roads

	road1.size.Set(road_width, road_height, circuit_x);
	road1.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 2);
	App->physics->AddBody(road1, 0)->collision_listeners.add(this);

	road2.size.Set(road_width, road_height, circuit_x);
	road2.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 3);
	App->physics->AddBody(road2, 0)->collision_listeners.add(this);

	road3.size.Set(road_width, road_height, circuit_x);
	road3.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 4);
	App->physics->AddBody(road3, 0)->collision_listeners.add(this);

	road4.size.Set(road_width, road_height, circuit_x);
	road4.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 5);
	App->physics->AddBody(road4, 0)->collision_listeners.add(this);

	road5.size.Set(road_width, road_height, circuit_x);
	road5.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 6);
	App->physics->AddBody(road5, 0)->collision_listeners.add(this);

	road6.size.Set(road_width, road_height, circuit_x);
	road6.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 7);
	App->physics->AddBody(road6, 0)->collision_listeners.add(this);

	road7.size.Set(road_width, road_height, circuit_x);
	road7.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 8);
	App->physics->AddBody(road7, 0)->collision_listeners.add(this);

	road8.size.Set(road_width, road_height, circuit_x);
	road8.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 9);
	App->physics->AddBody(road8, 0)->collision_listeners.add(this);

	road9.size.Set(road_width, road_height, circuit_x);
	road9.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 10);
	App->physics->AddBody(road9, 0)->collision_listeners.add(this);

	road10.size.Set(road_width, road_height, circuit_x);
	road10.SetPos(0, road_height / 2, circuit_x / 2 + circuit_x * 11);
	App->physics->AddBody(road10, 0)->collision_listeners.add(this);

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

	road_start.Render();
	road_path1.Render();
	pole_left.Render();
	pole_right.Render();
	poster.Render();


	//velocity  y  block
	if (posY_block >= 20)
		temp = temp * -1;
	if (posY_block <= 6)
		temp = temp * -1;
	posY_block += temp;
	block.SetPos(0, posY_block, 50.0f);
	pbblock->SetPos(0, posY_block, 50.0f);
	block.Render();

	//Render Simple Pendulum
	support.Render();
	prod->GetTransform(&rod.transform);
	rod.Render();
	//velocity on axisX of Bob
	pbob->GetTransform(&bob.transform);
	/*if (posX_bob >= 7)
	tempb = tempb * -1;
	if (posX_bob <= -7)
	tempb = tempb * -1;
	posX_bob += tempb;
	bob.SetPos(posX_bob, 5, ((50 + 50 / 2)));
	pbob->SetPos(posX_bob, 5, ((50 + 50 / 2)));*/

	bob.Render();

	road_path2.Render();

	//Motion of Cube blocks on X axis
	pc_left1->GetTransform(&c_left1.transform);
	pc_left2->GetTransform(&c_left2.transform);
	pc_right1->GetTransform(&c_right1.transform);
	pc_right2->GetTransform(&c_right2.transform);

	if (pl1 >= 12)
		tl1 = tl1 * -1;
	if (pl1 <= -12)
		tl1 = tl1 * -1;
	pl1 += tl1;

	c_left1.SetPos(pl1, 3, circuit_x / 2 + circuit_x + 10);
	pc_left1->SetPos(pl1, 3, circuit_x / 2 + circuit_x + 10);

	if (pl2 >= 12)
		tl2 = tl2 * -1;
	if (pl2 <= -12)
		tl2 = tl2 * -1;
	pl2 += tl2;

	c_left2.SetPos(pl2, 3, circuit_x / 2 + circuit_x + 20);
	pc_left2->SetPos(pl2, 3, circuit_x / 2 + circuit_x + 20);

	if (pr1 >= -12)
		tr1 = tr1 * -1;
	if (pr1 <= 12)
		tr1 = tr1 * -1;
	pr1 += tr1;

	c_right1.SetPos(pr1, 3, circuit_x / 2 + circuit_x + 15);
	pc_right1->SetPos(pr1, 3, circuit_x / 2 + circuit_x + 15);

	if (pr2 >= -12)
		tr2 = tr2 * -1;
	if (pr2 <= 12)
		tr2 = tr2 * -1;
	pr2 += tr2;

	c_right2.SetPos(pr2, 3, circuit_x / 2 + circuit_x + 25);
	pc_right2->SetPos(pr2, 3, circuit_x / 2 + circuit_x + 25);


	c_left1.Render();
	c_left2.Render();
	c_right1.Render();
	c_right2.Render();


	//more roads
	road1.Render();
	road2.Render();
	road3.Render();
	road4.Render();
	road5.Render();
	road6.Render();
	road7.Render();
	road8.Render();
	road9.Render();
	road10.Render();


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
	*/pwall_mid1 = App->physics->AddBody(wall_mid1, 0);

	wall_mid1.Render();


	/*if (pxw2 >= 1)
	tw2 = tw2 * -1;
	if (pxw2 <= -10)
	tw2 = tw2 * -1;
	pxw2 += tw2;
	wall_mid2.SetPos(pxw2, 6, circuit_x / 2 + circuit_x * 2 + 25);
	pwall_mid2 = App->physics->AddBody(wall_mid2, 0);*/
	wall_mid2.Render();

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


	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");

	//App->player->vehicle->SetPos(0, 3, 10);
	App->player->vehicle->SetTransform(&App->player->ini_trans);
	App->player->break_start = true;
		
}

