
#include "Renderer.h"
#include "Math/Math.h"


Renderer::Renderer() :ScreenWidth(640.0f), ScreenHeight(480.0f), FieldOfVIew(70.0f), NearPlane(0.1f), FarPLane(1000.0f)
{
	selectedMesh = 0;
}

Renderer::Renderer( float screenWidth, float screenHeight) : ScreenWidth(screenWidth), ScreenHeight(screenHeight),
															FieldOfVIew(70.0f), NearPlane(0.1f), FarPLane(1000.0f)
{
	selectedMesh = 0;
}

Renderer::~Renderer()
{
	auto size = m_mesh.size();
	for (int i = 0; i < (int)size; ++i)
	{
		m_mesh[i]->Clear();
		delete m_mesh[i];
	}
}

void Renderer::Prepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
	
}




void Renderer::UpdateEntities(Shader shader, float dt)
{
	shader.Use();
	std::vector<Matrix4>  Transforms;
	m_mesh[selectedMesh]->BoneTransform(dt, Transforms);

	auto size = Transforms.size();
	for (int i = 0; i < (int)size; i++) {
		shader.SetBoneTransform(i, Transforms[i]);
	}
	shader.Unuse();
}


void Renderer::RenderModel( Shader shader)
{
	shader.Use();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	GLuint textureSamplerID = glGetUniformLocation(shader.GetProgramID(), "textureSampler");
	glUniform1i(textureSamplerID, 0);
	
	Matrix4 ViewMatrix = camera->GetMatrix();
	
	Matrix4 ProjectionMatrix = Math::Perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	
	
	//m_mesh.transformation.Orientation = Math::AngleDegreesAxis(90.0f, Vector3(-1, 0, 0));
	
	Matrix4 ModelMatrix;
	
	ModelMatrix = Math::Translate(m_mesh[selectedMesh]->transformation.Position) *
		Math::QuaterinionToMatrix(m_mesh[selectedMesh]->transformation.Orientation) *
		Math::Scale(m_mesh[selectedMesh]->transformation.Scale);

	Matrix4 mvpMatrix = ViewMatrix * ModelMatrix;

	int loc = shader.GetUniformLocation("mpvMatrix");

	glUniformMatrix4fv(loc, 1, GL_FALSE, mvpMatrix[0].data);	
	shader.SetUniformLocation("phongSpecular", Vector3(1.0, 1.0, 1.0));
	shader.SetUniformLocation("phongShininess", 50);
	shader.SetUniformLocation("lightAmbient", Vector3(0.5f, 0.5f, 0.5f));
	shader.SetUniformLocation("lightValue", Vector3(0.8f, 0.8f, 0.8f));
	shader.SetUniformLocation("lightPos", Vector3(0, 100.0f, 100.0f));
	shader.SetUniformLocation("eyePos", camera->Transform.Position);
	shader.SetUniformLocation("Model", ModelMatrix);
	
	m_mesh[selectedMesh]->Render();

	shader.Unuse();
	
}

void Renderer::RenderBones(Shader shader)
{
	
	shader.Use();
	Matrix4 ModelMatrix;

	ModelMatrix = Math::Translate(m_mesh[selectedMesh]->transformation.Position) *
		Math::QuaterinionToMatrix(m_mesh[selectedMesh]->transformation.Orientation) *
		Math::Scale(m_mesh[selectedMesh]->transformation.Scale);
	shader.SetUniformLocation("model", ModelMatrix);
	// Pass them to the shaders
	shader.SetUniformLocation("camera", camera->GetMatrix());	

	m_mesh[selectedMesh]->RenderBones();
	shader.Unuse();

}

void Renderer::SetCamera(Camera* cam)
{
	camera = cam;
}

Camera* Renderer::GetCamera()
{
	return camera;
}

float Renderer::GetScreenWidth()
{
	return ScreenWidth;
}

void Renderer::SetScreenWidth(float width)
{
	ScreenWidth = width;
}

float Renderer::GetScreenHeight()
{
	return ScreenHeight;
}

void Renderer::SetScreenHeight(float height)
{
	ScreenHeight = height;
}

bool Renderer::InitMesh()
{

	Mesh* model1 =  new Mesh();
	if (!model1->LoadMesh("assets/yodan.x")) {

			printf("Mesh yodan load failed\n");
			return false;
	}
	
	Mesh* model2 = new Mesh();
	if (!model2->LoadMesh("ArmyPilot/ArmyPilot.x")) {

			printf("Mesh ArmyPilot load failed\n");
			return false;
	}

	m_mesh.push_back(model1);
	m_mesh.push_back(model2);
		
		m_mesh[0]->transformation.Scale = Vector3(0.3f, 0.3f, 0.3f);
		m_mesh[1]->transformation.Scale = Vector3(0.3f, 0.3f, 0.3f);

		m_mesh[1]->transformation.Orientation = Math::AngleDegreesAxis(180.0f, Vector3(0.0f, 1.0f, 0.0f));
		return true;
}

void Renderer::HandleKeyInput(bool* keys)
{
	
}

unsigned int Renderer::GetSelectedMesh()
{
	return selectedMesh;
}

void Renderer::SetSelectedMesh(unsigned int selected)
{
	selectedMesh = selected;
}

Mesh* Renderer::GetMesh()
{
	return m_mesh[selectedMesh];
}