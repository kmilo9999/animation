#pragma once

#include "Shader.h"
#include "Camera.h"
#include <vector>
#include "Skinnedmesh.h"

class Renderer
{
public:
	Renderer();
	Renderer( float screenWidth, float ScreenHeight);
	~Renderer();
	void Prepare();
	
	void RenderModel(Shader shader);
	void RenderBones(Shader shader);
	bool InitMesh();
	void InitWorld();

	

	void UpdateEntities(Shader shader,float dt);
	void SetCamera(Camera* );
	Camera* GetCamera();

	float GetScreenWidth();
	void SetScreenWidth(float);

	float GetScreenHeight();
	void SetScreenHeight(float);
	void HandleKeyInput(bool* keys);
	
	unsigned int GetSelectedMesh();
	void SetSelectedMesh(unsigned int);

	Mesh* GetMesh();

private:
	float FieldOfVIew;
	float NearPlane;
	float FarPLane;
	float ScreenWidth;
	float ScreenHeight;
	Camera* camera;
	//SkinnedMesh m_mesh;
	std::vector<Mesh*> m_mesh;
	Transformation WorldTranformation;
	unsigned int selectedMesh;

};
