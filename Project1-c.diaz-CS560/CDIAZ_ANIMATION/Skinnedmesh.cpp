#include "Skinnedmesh.h"

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

void VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	unsigned int IDsSize = (sizeof(IDs) / sizeof(IDs[0]));
	for (unsigned int i = 0; i < IDsSize; i++)
	{
		if (Weights[i] == 0.0)
		{
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}
	printf("ERROR: More bones that max.");
}

Mesh::Mesh()
{
	m_VAO = 0;
	memset(m_Buffers, 0, sizeof(m_Buffers));
	m_NumBones = 0;
	m_assimpScene = NULL;
}

Mesh::Mesh(const Mesh& mesh) :m_Entries(mesh.m_Entries), m_Textures(mesh.m_Textures),
						m_BoneMapping(mesh.m_BoneMapping), m_NumBones(mesh.m_NumBones),
						m_BoneInfo(mesh.m_BoneInfo), m_GlobalInverseTransform(mesh.m_GlobalInverseTransform),
						bonesVertex(mesh.bonesVertex), m_assimpScene(mesh.m_assimpScene),
						m_assimpImporter(mesh.m_assimpImporter), m_VAO(mesh.m_VAO),
						b_Vao(mesh.b_Vao), b_Vbo(mesh.b_Vbo)
{

}

Mesh::~Mesh()
{
	Clear();
}

void Mesh::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		if (m_Textures[i]) { 
			delete m_Textures[i]; 
			m_Textures[i] = NULL; 
		}
	}

	if (m_Buffers[0] != 0)
	{
		unsigned int bufferSize = (sizeof(m_Buffers) / sizeof(m_Buffers[0]));
		glDeleteBuffers(bufferSize, m_Buffers);
	}

	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}


bool Mesh::LoadMesh(const std::string& Filename)
{
	Clear();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	unsigned int bufferSize = (sizeof(m_Buffers) / sizeof(m_Buffers[0]));
	glGenBuffers(bufferSize, m_Buffers);

	bool Ret = false;
	m_assimpScene = m_assimpImporter.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (m_assimpScene)
	{
		m_GlobalInverseTransform = convertToMatrix(m_assimpScene->mRootNode->mTransformation);
		m_GlobalInverseTransform = Math::Inverse(m_GlobalInverseTransform);
		Ret = InitFromScene(m_assimpScene, Filename);
		
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_assimpImporter.GetErrorString());
	}

	glBindVertexArray(0);

	glGenVertexArrays(1, &b_Vao);
	glGenBuffers(1, &b_Vbo);
	BoneCounting(m_assimpScene->mRootNode);


	return Ret;
}


bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Reading model data
	std::vector<Vector3> Positions;
	std::vector<Vector3> Normals;
	std::vector<Vector2> TexCoords;
	std::vector<VertexBoneData> Bones;
	std::vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Bones.resize(NumVertices);
	Indices.reserve(NumIndices);

	// Initializing assimp meshes.
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}

	if (!InitMaterials(pScene, Filename))
	{
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return (glGetError() == GL_NO_ERROR);
}


void Mesh::InitMesh(unsigned int MeshIndex, aiMesh* paiMesh, std::vector<Vector3>& Positions, std::vector<Vector3>& Normals,
	std::vector<Vector2>& TexCoords, std::vector<VertexBoneData>& Bones, std::vector<unsigned int>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(Vector3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(Vector3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(Vector2(pTexCoord->x, pTexCoord->y));
	}

	LoadBones(MeshIndex, paiMesh, Bones);
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	numAnimations = m_assimpScene->mNumAnimations > 3 ? 3 : m_assimpScene->mNumAnimations;
	selectedAnimation = 0;
}

// Loading bones into the bone mapping vector. This'll store bones bind position for future reference
void Mesh::LoadBones(unsigned int MeshIndex, aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
	for (unsigned int i = 0; i < pMesh->mNumBones; i++)
	{
		unsigned int BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			m_BoneInfo[BoneIndex].BoneOffset = convertToMatrix(pMesh->mBones[i]->mOffsetMatrix);
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else
		{
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
		{
			unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

// Get Assimp materials and load textures
bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;
	unsigned int numMaterials = pScene->mNumMaterials;
	for (unsigned int i = 0; i < numMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\")
				{
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());
				m_Textures[i]->type = "diffuse";

				if (!m_Textures[i]->Load())
				{
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else
				{
					printf("%d - loaded texture \n");
				}
			}
		}
		
	}

	return Ret;
}

void Mesh::Render()
{
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (m_Textures[MaterialIndex])
		{
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}
	glBindVertexArray(0);
}

// Find the position reference for that time in the animation
unsigned int Mesh::searchPosition(float AnimationTime, aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

// Find the rotation reference for that time in the animation
unsigned int Mesh::searchRotation(float AnimationTime, aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

// Find the scaling reference for that time in the animation
unsigned int Mesh::searchScaling(float AnimationTime, aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

// Interpolate position (VQM)
aiVector3D Mesh::InterpolatePosition(float AnimationTime, aiNodeAnim* pNodeAnim)
{
	aiVector3D Out;
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return Out;
	}

	unsigned int PositionIndex = searchPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
	return Out;
}

// Interpolate rotation (VQM)
aiQuaternion Mesh::InterpolateRotation(float AnimationTime, aiNodeAnim* pNodeAnim)
{
	aiQuaternion Out;
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return Out;
	}

	unsigned int RotationIndex = searchRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	Slerp(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
	return Out;
}


void Mesh::Slerp(aiQuaternion& pOut, aiQuaternion& pStart, aiQuaternion& pEnd, float pFactor)
{
	float alpha, beta;
	float cosom = pStart.x*pEnd.x + pStart.y*pEnd.y + pStart.z*pEnd.z + pStart.w*pEnd.w;
	float slerp_epsilon = 0.00001f;

	bool flip;

	if (flip = (cosom < 0))
	{
		cosom = -cosom;
	}

	if ((1.0 - cosom) > slerp_epsilon)
	{
		float omega = acos(cosom);
		float sinom = sin(omega);
		alpha = (float)(sin((1.0 - pFactor) * omega) / sinom);
		beta = (float)(sin(pFactor * omega) / sinom);
	}
	else
	{
		alpha = (float)(1.0 - pFactor);
		beta = (float)pFactor;
	}

	if (flip)
	{
		beta = -beta;
	}

	pOut.x = (float)(alpha*pStart.x + beta*pEnd.x);
	pOut.y = (float)(alpha*pStart.y + beta*pEnd.y);
	pOut.z = (float)(alpha*pStart.z + beta*pEnd.z);
	pOut.w = (float)(alpha*pStart.w + beta*pEnd.w);
}

// Interpolate scaling (VQM)
aiVector3D Mesh::InterpolateScaling(float AnimationTime, aiNodeAnim* pNodeAnim)
{
	aiVector3D Out;
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return Out;
	}

	unsigned int ScalingIndex = searchScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
	return Out;
}

// Recursively calculate the bone transformation 
void Mesh::ReadNodeHierarchy(float AnimationTime, aiNode* pNode, Matrix4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);
	int na = m_assimpScene->mNumAnimations;
	
	aiAnimation* pAnimation = m_assimpScene->mAnimations[selectedAnimation];
	Matrix4 NodeTransformation = convertToMatrix(pNode->mTransformation);
	aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim)
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling = InterpolateScaling(AnimationTime, pNodeAnim);
		Vector3 glmScaling = Vector3(Scaling.x, Scaling.y, Scaling.z);
		Matrix4 ScalingM;
		ScalingM = Math::Scale(	 glmScaling);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ = InterpolateRotation(AnimationTime, pNodeAnim);
		Matrix4 RotationM = convertToMatrix(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation = InterpolatePosition(AnimationTime, pNodeAnim);
		Vector3 glmTranslation = Vector3(Translation.x, Translation.y, Translation.z);
		Matrix4 TranslationM = Math::Translate( glmTranslation);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	Matrix4 GlobalTransformation = ParentTransform * NodeTransformation;

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		ReadNodeHierarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
	{
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}
}

// Entry point to bones actualization
void Mesh::BoneTransform(float TimeInSeconds, std::vector<Matrix4>& Transforms)
{
	Matrix4 Identity(1);
	//cout << m_assimpScene->mNumAnimations << endl;
	
	float TicksPerSecond = (float)(m_assimpScene->mAnimations[selectedAnimation]->mTicksPerSecond != 0 ? m_assimpScene->mAnimations[selectedAnimation]->mTicksPerSecond : 25.0f);
	float TimeInTicks = TimeInSeconds * TicksPerSecond ;
	float AnimationTime = fmod(TimeInTicks, (float)m_assimpScene->mAnimations[selectedAnimation]->mDuration);

	
	 // always get the first animation
	ReadNodeHierarchy(AnimationTime, m_assimpScene->mRootNode, Identity);
	Transforms.resize(m_NumBones);

	for (unsigned int i = 0; i < m_NumBones; i++)
	{
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

aiNodeAnim* Mesh::FindNodeAnim(aiAnimation* pAnimation, std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
	{
		aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
		if (std::string(pNodeAnim->mNodeName.data) == NodeName)
		{
			return pNodeAnim;
		}
	}
	return NULL;
}

Matrix4 Mesh::convertToMatrix(aiMatrix4x4 m)
{
	Matrix4 result;
	result[0][0] = m.a1; result[0][1] = m.b1; result[0][2] = m.c1; result[0][3] = m.d1;
	result[1][0] = m.a2; result[1][1] = m.b2; result[1][2] = m.c2; result[1][3] = m.d2;
	result[2][0] = m.a3; result[2][1] = m.b3; result[2][2] = m.c3; result[2][3] = m.d3;
	result[3][0] = m.a4; result[3][1] = m.b4; result[3][2] = m.c4; result[3][3] = m.d4;
	return result;
}

Matrix4 Mesh::convertToMatrix(aiMatrix3x3 m)
{
	Matrix4 result;
	result[0][0] = m.a1; result[0][1] = m.b1; result[0][2] = m.c1; result[0][3] = 0.0f;
	result[1][0] = m.a2; result[1][1] = m.b2; result[1][2] = m.c2; result[1][3] = 0.0f;
	result[2][0] = m.a3; result[2][1] = m.b3; result[2][2] = m.c3; result[2][3] = 0.0f;
	result[3][0] = 0.0f; result[3][1] = 0.0f; result[3][2] = 0.0f; result[3][3] = 1;
	return result;
}

void Mesh::BoneCounting(aiNode* pNode)
{
	std::string NodeName(pNode->mName.data);
	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
	{
		unsigned int ParentBoneIndex = m_BoneMapping[NodeName];

		for (GLuint i = 0; i < pNode->mNumChildren; i++)
		{
			if (pNode->mChildren[i] != NULL && m_BoneMapping.find(pNode->mChildren[i]->mName.data) != m_BoneMapping.end())
			{
				GLuint ChildBoneIndex = m_BoneMapping[pNode->mChildren[i]->mName.data];

				//add child info in parent
				m_BoneInfo[ParentBoneIndex].children.push_back(ChildBoneIndex);

				//add parent info in child
				m_BoneInfo[ChildBoneIndex].parentIndex = ParentBoneIndex;
			}
		}
	}

	for (GLuint i = 0; i < pNode->mNumChildren; i++) {
		BoneCounting(pNode->mChildren[i]);
	}

}

void Mesh::RenderBones()
{
	std::vector<Vector4> bonePositions;
	GetBonesPositions(bonePositions);
	
	glBindVertexArray(b_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, b_Vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(bonePositions[0]) * bonePositions.size(),
		&bonePositions[0],
		GL_STATIC_DRAW
		);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	glDisable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(b_Vao);
	glDrawArrays(GL_POINTS, 0, bonePositions.size());
	glDrawArrays(GL_LINES, 0, bonePositions.size());
	glDisable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(b_Vao);

	bonePositions.clear();
}

void Mesh::GetBonesPositions(std::vector<Vector4>& bonePositions)
{
	
	for (unsigned int i = 0; i < m_BoneInfo.size(); i++)
	{
		int c = 0;

		Matrix4 inverse = Math::Inverse(m_BoneInfo[i].BoneOffset);
		Matrix4 v = m_BoneInfo[i].FinalTransformation * inverse;

		Vector4 parentPos = Vector4(v[3].x, v[3].y, v[3].z, v[3].w);

		for (unsigned int c = 0; c < m_BoneInfo[i].children.size(); c++)
		{
			int childIndex = m_BoneInfo[i].children[c];

			Matrix4 g = m_BoneInfo[childIndex].FinalTransformation * Math::Inverse(m_BoneInfo[childIndex].BoneOffset);
			Vector4 childPos = Vector4(g[3].x, g[3].y, g[3].z, g[3].w);

			bonePositions.push_back(parentPos);
			bonePositions.push_back(childPos);

		}

		if (bonePositions.size() == 0)
		{
			continue;
		}
	}
}