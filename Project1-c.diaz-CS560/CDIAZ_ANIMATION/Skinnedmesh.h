#pragma once
#include <map>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include "Math/Math.h"

#define BONES_PER_VEREX 4

struct BoneInfo
{
	BoneInfo()
	{
		BoneOffset *= 0;
		FinalTransformation *= 0;
	}
	int parentIndex;
	std::vector<int>children;
	// Initial Matrix in bone space
	Matrix4 BoneOffset;
	// Transformation of the bone for that frame
	Matrix4 FinalTransformation;
};

struct VertexBoneData
{
	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		memset(IDs, 0, sizeof(IDs));
		memset(Weights, 0, sizeof(Weights));
	}

	void AddBoneData(unsigned int BoneID, float Weight);

	unsigned int IDs[BONES_PER_VEREX];
	float Weights[BONES_PER_VEREX];
};

enum VB_TYPES
{
	INDEX_BUFFER,
	POS_VB,
	NORMAL_VB,
	TEXCOORD_VB,
	BONE_VB,
	NUM_VBs
};

struct MeshEntry
{
	MeshEntry()
	{
		NumIndices = 0;
		BaseVertex = 0;
		BaseIndex = 0;
		MaterialIndex = -1;
	}

	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	~Mesh();

	bool LoadMesh(const std::string& Filename);
	void BoneTransform(float TimeInSeconds, std::vector<Matrix4>& Transforms);
	void Render();
	
	void BoneCounting(aiNode* pNode);
	void RenderBones();
	void GetBonesPositions(std::vector<Vector4>& bonePositions);
	Transformation transformation;
	void Clear();
	int numAnimations;
	int selectedAnimation;

private:
	aiVector3D InterpolateScaling(float AnimationTime, aiNodeAnim* pNodeAnim);
	aiQuaternion InterpolateRotation(float AnimationTime, aiNodeAnim* pNodeAnim);
	aiVector3D InterpolatePosition(float AnimationTime, aiNodeAnim* pNodeAnim);
	unsigned int searchScaling(float AnimationTime, aiNodeAnim* pNodeAnim);
	unsigned int searchRotation(float AnimationTime, aiNodeAnim* pNodeAnim);
	unsigned int searchPosition(float AnimationTime, aiNodeAnim* pNodeAnim);
	aiNodeAnim* FindNodeAnim(aiAnimation* pAnimation, std::string NodeName);
	void ReadNodeHierarchy(float AnimationTime, aiNode* pNode, Matrix4& ParentTransform);
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int MeshIndex, aiMesh* paiMesh,
		std::vector<Vector3>& Positions, std::vector<Vector3>& Normals, std::vector<Vector2>& TexCoords,
		std::vector<VertexBoneData>& Bones, std::vector<unsigned int>& Indices);
	void LoadBones(unsigned int MeshIndex, aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	

	void Slerp(aiQuaternion& pOut, aiQuaternion& pStart, aiQuaternion& pEnd, float pFactor);
	

	Matrix4 convertToMatrix(aiMatrix4x4 m);
	Matrix4 convertToMatrix(aiMatrix3x3 m);

	GLuint m_VAO;
	GLuint m_Buffers[NUM_VBs];
	GLuint b_Vao;
	GLuint b_Vbo;

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

	std::map<std::string, unsigned int> m_BoneMapping;
	unsigned int m_NumBones;
	std::vector<BoneInfo> m_BoneInfo;
	
	Matrix4 m_GlobalInverseTransform;
	std::vector<Vector3> bonesVertex;

	const aiScene* m_assimpScene;
	Assimp::Importer m_assimpImporter;
	
	
};