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

	Cube CreateCube(const vec3 size, const vec3 position, bool phys = true, float angle=0.0f, const vec3 axis=vec3(0,0,0));
	void MoveElevators();
	
	//Level1
	//void MoveCubeX();
	void MoveCube(PhysBody3D& body, Cube cube, float motion, float velocity, bool right);

	void CreatePolePosition(const float road_width, const float road_height, const float positionY, const float positionZ);
	void CreatePendulum(const float road_width, const float road_height, const float positionY, const float positionZ);
	void RenderStaticPieces();
	void MotionPendulum(const float positionY, const float positionZ);
	PhysBody3D* PutBlock(const float positionY, const float positionZ);
	void MovementBlock(PhysBody3D* body, float posZ);
	void CreateRoad(const float road_width, const float positionY, const float positionZ);
	
	

public:
	//<--List Primitives Objects statics
	p2List<Cube*> cube_list;
	p2List<Cylinder*> cylinders;
	p2List<Sphere*> spheres;

	//<--List Primitives and pbodies with mass
	p2List<Cylinder*> cylinders_move;
	p2List<Sphere*> spheres_move;
	p2List<PhysBody3D*> pbpieces_cylinders_move;
	p2List<PhysBody3D*> pbpieces_spheres_move;

	//?
	p2List<PhysBody3D*> pbpieces;
	p2List<Cube*> cube_list_move;

	//<-- List of blocks that have motion
	p2List<Cube*> block_list_move;
	

	
	//<----CAR------------>
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
	float road_width = 10.0f;
	float road_height = 2.0f;
	float high_map;
	vec3 road_size;

	//Floor Sensor
	PhysBody3D* floor_sensor;
	Cube floor;

	//Level1
	vec3 cube_size;
	Cube elevator;
	Cube c_left1, c_right1, c_left2, c_right2;
	PhysBody3D* pc_left1;
	PhysBody3D* pc_right1;
	PhysBody3D* pc_left2;
	PhysBody3D* pc_right2;
	PhysBody3D* p_elevator;

	//variables for blocks x axis motion
	//float pl1, pl2, pr1, pr2, tl1, tl2, tr1, tr2;

	//variables elevator
	float pY_elevator, tY_elevator;

	//Level2
	Cube elevator2;
	PhysBody3D* p_elevator2;
	float pY_elevator2, tY_elevator2;
	Cube cube_checkpoint_lvl2;
	PhysBody3D* sensor_checkpoint_lvl2;
	//Level3
	float posX_bob, tempb;
	float posY_block, temp;
	

	//LVL5
	PhysBody3D* block1;
	PhysBody3D* block2;



};














/*
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
*/