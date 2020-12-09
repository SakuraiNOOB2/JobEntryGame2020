#pragma once

#include <map>
#include <vector>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#pragma comment (lib,"assimp.lib")



//変形後頂点構造体

struct DEFORM_VERTEX {

	aiVector3D Position;
	aiVector3D Normal;

	int boneNum;
	std::string boneName[4];		//本来はボーンインデックスで管理するべき
	float boneWeight[4];

};

//ボーン構造体

struct BONE {

	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;

};




class CanimatedModel {

private:
	const aiScene* m_Scene = NULL;
	std::map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer** m_VertexBuffer;	//配列ポインタ
	ID3D11Buffer** m_IndexBuffer;

	std::map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;	//変形後頂点データ
	std::map<const std::string, BONE> m_Bone;	//ボーンデータ(名前で参照)

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

public:

	void Load(const char *Filename);
	void LoadAnimation(const char *filename, const char *name);
	void Unload();
	void Update(const char* animationName1,const char* animationName2,float blendRate,int frame);
	void Draw();



};