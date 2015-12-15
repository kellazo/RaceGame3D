#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	//------CIRCUIT

	//long circuit
	float circuit_x;

	//pole & block
	PhysBody3D* pbblock;
	Cube road_start, road_path1, poster, block;
	Cylinder pole_left, pole_right;
	float posY_block, temp;

	//Simple Pendulum
	PhysBody3D* psupport;
	PhysBody3D* prod;
	PhysBody3D* pbob;
	Cube support;
	Cylinder rod;
	Sphere bob;
	float posX_bob, tempb;

	//road with blocks x axis motion
	Cube c_left1, c_left2, c_right1, c_right2, road_path2;
	float pl1, pl2, pr1, pr2, tl1, tl2, tr1, tr2;
	PhysBody3D* pc_left1;
	PhysBody3D* pc_left2;
	PhysBody3D* pc_right1;
	PhysBody3D* pc_right2;

	//more road path
	Cube road1, road2, road3, road4, road5, road6, road7, road8, road9, road10;

	//blocks from ground and up
	Cube blockground1, blockground2;
	PhysBody3D* pblockground1;
	PhysBody3D* pblockground2;
	float py1, py2, t1, t2;

	//wall on middle
	Cube wall_mid1, wall_mid2;
	PhysBody3D* pwall_mid1;
	PhysBody3D* pwall_mid2;
	float pxw1, pxw2, tw1, tw2;

	//wall out
	Cube wall_outl;
	PhysBody3D* pwall_outl;
	Cube wall_outr;
	PhysBody3D* pwall_outr;
	float pxwo1, pxwo2, two1, two2;

	//flippers wall

	//hole road


	p2List<PhysBody3D*> pbpieces;
	p2List<Primitive> pieces;


	//Floor Sensor
	PhysBody3D* floor_sensor;
	Cube floor;



};