#pragma once

#include <map>
#include <vector>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#pragma comment (lib,"assimp.lib")



//�ό`�㒸�_�\����

struct DEFORM_VERTEX {

	aiVector3D Position;
	aiVector3D Normal;

	int boneNum;
	std::string boneName[4];		//�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float boneWeight[4];

};

//�{�[���\����

struct BONE {

	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;

};




class CanimatedModel {

private:
	const aiScene* m_Scene = NULL;
	std::map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer** m_VertexBuffer;	//�z��|�C���^
	ID3D11Buffer** m_IndexBuffer;

	std::map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;	//�ό`�㒸�_�f�[�^
	std::map<const std::string, BONE> m_Bone;	//�{�[���f�[�^(���O�ŎQ��)

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

public:

	void Load(const char *Filename);
	void LoadAnimation(const char *filename, const char *name);
	void Unload();
	void Update(const char* animationName1,const char* animationName2,float blendRate,int frame);
	void Draw();



};