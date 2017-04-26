////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "MeshObject.h"
#include "BoxTreeObject.h"
#include "SphereObject.h"
#include "PlaneObject.h"
#include "InstanceObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp" 
#include "glm/gtc/random.hpp"
#include <time.h>
#include <ctime>
#include <iostream>
#include <string>

#define PI 3.14159265359

void project1();
void spheres();
void project2();

////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
	time_t t;
	srand((unsigned)time(&t));

	//project1();
	//spheres();

	project2();

	std::cout << "Press ENTER to quit";
	std::string ept;
	std::getline(std::cin, ept);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void project1() {
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));

	// Create boxes
	MeshObject box1; 
	box1.MakeBox(5.0f,0.1f,5.0f);
	//scn.AddObject(box1);

	BoxTreeObject tree1;
	tree1.Construct(box1);
	scn.AddObject(tree1);

	MeshObject box2;
	box2.MakeBox(1.0f,1.0f,1.0f);

	BoxTreeObject tree2;
	tree2.Construct(box2);

	InstanceObject inst1(tree2);
	glm::mat4x4 mtx=glm::rotate(glm::mat4x4(),0.5f,glm::vec3(1,0,0));
	mtx[3][1]=1.0f;
	inst1.SetMatrix(mtx); 
	scn.AddObject(inst1);


	// half box
	InstanceObject inst2(tree2);
	mtx=glm::rotate(glm::mat4x4(),1.0f,glm::vec3(0,1,0));
	mtx[3] = glm::vec4(-1, 0, 1, 1);
	inst2.SetMatrix(mtx);
	scn.AddObject(inst2);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(0.5f);
	sunlgt.SetDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(2.0f);
	redlgt.SetPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	scn.AddLight(redlgt);

	// Create camera
	Camera cam;
	cam.LookAt(glm::vec3(2.0f,2.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0,1,0));
	cam.SetResolution(800,600);
	cam.SetFOV(40.0f);
	cam.SetAspect(1.33f);

	// Render image
	cam.Render(scn, 0);
	cam.SaveBitmap("project1.bmp");

	cam.Render(scn, 1);
	cam.SaveBitmap("project1_shade.bmp");
}

void spheres() {
	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));

	// Create ground plane
	PlaneObject ground;
	scn.AddObject(ground);

	// Create spheres
	for (int i = 0; i<20; i++) {
		SphereObject *sphere = new SphereObject;
		float rad = glm::linearRand(0.25f, 0.5f);
		vec pos(glm::linearRand(-5.0f, 5.0f), rad, glm::linearRand(-5.0f, 5.0f));
		sphere->SetRadius(rad);
		sphere->SetCenter(pos);
		scn.AddObject(*sphere);
	}

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(1.0f);
	sunlgt.SetDirection(vec(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);

	// Create camera
	Camera cam;
	cam.LookAt(vec(-3.75f, 0.25f, 6.0f), vec(0.0f, 0.5f, 0.0f), vec(0, 1, 0));
	cam.SetFOV(40.0f);
	cam.SetAspect(1.33f);
	cam.SetResolution(800, 600);

	// Render image
	cam.Render(scn, 0);
	cam.SaveBitmap("spheres.bmp");
	cam.Render(scn, 1);
	cam.SaveBitmap("spheres_shade.bmp");
}

void project2() {

	// Create scene
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));

	// Create ground
	MeshObject ground;
	ground.MakeBox(5.0f, 0.1f, 5.0f);
	scn.AddObject(ground);

	/*BoxTreeObject treeg;
	treeg.Construct(ground);
	scn.AddObject(treeg);*/

	std::cout << "LoadPLY Timer started." << std::endl;
	std::clock_t timer = std::clock();

	// Create dragon
	MeshObject dragon;
	dragon.LoadPLY("dragon.ply", 0);
	dragon.Smooth();

	double timeused = (std::clock() - timer) / (double)CLOCKS_PER_SEC;
	std::cout << "LoadPLY Time Used: " << timeused << std::endl;


	std::cout << "Construct BoxTree Timer started." << std::endl;
	timer = std::clock();

	BoxTreeObject tree;
	tree.Construct(dragon);
	scn.AddObject(tree);

	timeused = (std::clock() - timer) / (double)CLOCKS_PER_SEC;
	std::cout << "Construct BoxTree Time Used: " << timeused << std::endl;

	// Create instance
	InstanceObject inst(tree);
	glm::mat4x4 mtx = glm::eulerAngleY(PI);
	mtx[3] = glm::vec4(-0.05f, 0.0f, -0.1f, 1.0f);
	inst.SetMatrix(mtx);
	scn.AddObject(inst);

	// Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
	sunlgt.SetIntensity(1.0f);
	sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(0.02f);
	redlgt.SetPosition(glm::vec3(-0.2f, 0.2f, 0.2f));
	scn.AddLight(redlgt);

	PointLight bluelgt;
	bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f));
	bluelgt.SetIntensity(0.02f);
	bluelgt.SetPosition(glm::vec3(0.1f, 0.1f, 0.3f));
	scn.AddLight(bluelgt);

	// Create camera
	Camera cam;
	cam.LookAt(glm::vec3(-0.1f, 0.1f, 0.2f), glm::vec3(-0.05f, 0.12f, 0.0f), glm::vec3(0, 1.0f, 0));
	cam.SetFOV(40.0f);
	cam.SetAspect(1.33f);
	cam.SetResolution(800, 600);

	// Render image
	std::cout << "Render Timer started." << std::endl;
	timer = std::clock();
	cam.Render(scn, 1);
	timeused = (std::clock() - timer) / (double)CLOCKS_PER_SEC;
	std::cout << "Render Time Used: " << timeused << std::endl;
	cam.SaveBitmap("project2.bmp");
}