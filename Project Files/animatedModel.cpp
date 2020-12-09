#include "main.h"
#include "renderer.h"
#include "animatedModel.h"


void CanimatedModel::Load(const char *Filename) {

	const std::string modelPath(Filename);

	m_Scene = aiImportFile(Filename, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	assert(m_Scene);

	m_VertexBuffer = new ID3D11Buffer*[m_Scene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_Scene->mNumMeshes];

	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_Scene->mNumMeshes];

	//�ċA�I�Ƀ{�[������
	CreateBone(m_Scene->mRootNode);

	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++) {

		aiMesh* mesh = m_Scene->mMeshes[m];

		//���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++) {

				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				/*vertex[v].Diffuse = D3DXVECTOR4(mesh->mColors[v]->r, mesh->mColors[v]->g, mesh->mColors[v]->b, mesh->mColors[v]->a);*/
				vertex[v].Diffuse = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);

			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D)*mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);


			delete[] vertex;
		}

		//�C���f�b�N�X�o�b�t�@����
		{

			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++) {

				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int)*mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}

		//�ό`�㒸�_�f�[�^������
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {

			DEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.boneNum = 0;

			for (unsigned int b = 0; b < 4; b++) {

				deformVertex.boneName[b] = "";
				deformVertex.boneWeight[b] = 0.0f;
			}

			m_DeformVertex[m].push_back(deformVertex);
		}

		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++) {

			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
			for (unsigned int w = 0; w < bone->mNumWeights; w++) {

				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].boneNum;

				m_DeformVertex[m][weight.mVertexId].boneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].boneName[num] = bone->mName.C_Str();
				m_DeformVertex[m][weight.mVertexId].boneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].boneNum <= 4);

			}
			
		}


		//�e�X�N�`���̓ǂݍ���
		{
			for (unsigned int m = 0; m < m_Scene->mNumMaterials; m++) {

				aiString path;

				if (m_Scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

					if (path.data[0] == '*') {

						//FBX�t�@�C��������ǂݍ���
						if (m_Texture[path.data] == NULL) {

							ID3D11ShaderResourceView* texture;
							int id = atoi(&path.data[1]);

							D3DX11CreateShaderResourceViewFromMemory(
								CRenderer::GetDevice(),
								(const unsigned char*)m_Scene->mTextures[id]->pcData,
								m_Scene->mTextures[id]->mWidth,
								NULL,
								NULL,
								&texture,
								NULL);

							m_Texture[path.data] = texture;

						}

					}
					else {

						//�O���t�@�C������ǂݍ���


					}

				}
				else {




				}


			}
		}


	}


}

void CanimatedModel::LoadAnimation(const char* filename, const char* name)
{
	m_Animation[name] = aiImportFile(filename, 0);
	assert(m_Animation[name]);

}

void CanimatedModel::CreateBone(aiNode* node) {

	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {

		CreateBone(node->mChildren[n]);
	}


}

void CanimatedModel::UpdateBoneMatrix(aiNode * node, aiMatrix4x4 matrix)
{

	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�ݒ�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {

		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}

}



void CanimatedModel::Unload() {

	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++) {

		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	//for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture) {

	//	pair.second->Release();
	//}

	for (auto pair : m_Texture) {

		pair.second->Release();
	}

	aiReleaseImport(m_Scene);
}

void CanimatedModel::Update(const char* animationName1,const char* animationName2,float blendRate,int frame) {

	if (!m_Animation[animationName1]->HasAnimations()) {

		return;
	}
	if (!m_Animation[animationName2]->HasAnimations()) {

		return;
	}


	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[animationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[animationName2]->mAnimations[0];

	for (unsigned int c = 0; c < animation1->mNumChannels; c++) {

		aiNodeAnim* nodeAnim1 = animation1->mChannels[c];
		aiNodeAnim* nodeAnim2 = animation2->mChannels[c];
		BONE* bone = &m_Bone[nodeAnim1->mNodeName.C_Str()];

		int f;
		f = frame % nodeAnim1->mNumRotationKeys;	//�ȈՎ���
		aiQuaternion rot1 = nodeAnim1->mRotationKeys[f].mValue;

		f = frame % nodeAnim1->mNumPositionKeys;	//�ȈՎ���
		aiVector3D pos1 = nodeAnim1->mPositionKeys[f].mValue;

		f = frame % nodeAnim2->mNumRotationKeys;	//�ȈՎ���
		aiQuaternion rot2 = nodeAnim2->mRotationKeys[f].mValue;

		f = frame % nodeAnim2->mNumPositionKeys;	//�ȈՎ���
		aiVector3D pos2 = nodeAnim2->mPositionKeys[f].mValue;

		aiVector3D pos = pos1 * (1.0f - blendRate) + pos2 * blendRate;
		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, blendRate);

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�{�[���}�g���N�X���X�V
	UpdateBoneMatrix(m_Scene->mRootNode, aiMatrix4x4());

	//���_�ϊ�(CPU�X�L�j���O)
	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++) {

		aiMesh* mesh = m_Scene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D *vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {

			DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;

			matrix[0] = m_Bone[deformVertex->boneName[0]].Matrix;
			matrix[1] = m_Bone[deformVertex->boneName[1]].Matrix;
			matrix[2] = m_Bone[deformVertex->boneName[2]].Matrix;
			matrix[3] = m_Bone[deformVertex->boneName[3]].Matrix;

			//�E�F�C�g���l�����ă}�g���N�X�Z�o
			//outMatrix = matrix[0] * deformVertex->boneWeight[0]
			//		  + matrix[1] * deformVertex->boneWeight[1]
			//		  + matrix[2] * deformVertex->boneWeight[2]
			//		  + matrix[3] * deformVertex->boneWeight[3];

			{

				//A
				outMatrix.a1 = matrix[0].a1 * deformVertex->boneWeight[0]
							 + matrix[1].a1 * deformVertex->boneWeight[1]
							 + matrix[2].a1 * deformVertex->boneWeight[2]
							 + matrix[3].a1 * deformVertex->boneWeight[3];

				outMatrix.a2 = matrix[0].a2 * deformVertex->boneWeight[0]
							 + matrix[1].a2 * deformVertex->boneWeight[1]
							 + matrix[2].a2 * deformVertex->boneWeight[2]
							 + matrix[3].a2 * deformVertex->boneWeight[3];

				outMatrix.a3 = matrix[0].a3 * deformVertex->boneWeight[0]
							 + matrix[1].a3 * deformVertex->boneWeight[1]
							 + matrix[2].a3 * deformVertex->boneWeight[2]
							 + matrix[3].a3 * deformVertex->boneWeight[3];

				outMatrix.a4 = matrix[0].a4 * deformVertex->boneWeight[0]
							 + matrix[1].a4 * deformVertex->boneWeight[1]
							 + matrix[2].a4 * deformVertex->boneWeight[2]
							 + matrix[3].a4 * deformVertex->boneWeight[3];


				//B
				outMatrix.b1 = matrix[0].b1 * deformVertex->boneWeight[0]
						 	 + matrix[1].b1 * deformVertex->boneWeight[1]
						 	 + matrix[2].b1 * deformVertex->boneWeight[2]
						 	 + matrix[3].b1 * deformVertex->boneWeight[3];

				outMatrix.b2 = matrix[0].b2 * deformVertex->boneWeight[0]
							 + matrix[1].b2 * deformVertex->boneWeight[1]
							 + matrix[2].b2 * deformVertex->boneWeight[2]
							 + matrix[3].b2 * deformVertex->boneWeight[3];

				outMatrix.b3 = matrix[0].b3 * deformVertex->boneWeight[0]
							 + matrix[1].b3 * deformVertex->boneWeight[1]
							 + matrix[2].b3 * deformVertex->boneWeight[2]
							 + matrix[3].b3 * deformVertex->boneWeight[3];

				outMatrix.b4 = matrix[0].b4 * deformVertex->boneWeight[0]
							 + matrix[1].b4 * deformVertex->boneWeight[1]
							 + matrix[2].b4 * deformVertex->boneWeight[2]
							 + matrix[3].b4 * deformVertex->boneWeight[3];

				//C

				outMatrix.c1 = matrix[0].c1 * deformVertex->boneWeight[0]
							 + matrix[1].c1 * deformVertex->boneWeight[1]
							 + matrix[2].c1 * deformVertex->boneWeight[2]
							 + matrix[3].c1 * deformVertex->boneWeight[3];

				outMatrix.c2 = matrix[0].c2 * deformVertex->boneWeight[0]
							 + matrix[1].c2 * deformVertex->boneWeight[1]
							 + matrix[2].c2 * deformVertex->boneWeight[2]
							 + matrix[3].c2 * deformVertex->boneWeight[3];

				outMatrix.c3 = matrix[0].c3 * deformVertex->boneWeight[0]
							 + matrix[1].c3 * deformVertex->boneWeight[1]
							 + matrix[2].c3 * deformVertex->boneWeight[2]
							 + matrix[3].c3 * deformVertex->boneWeight[3];

				outMatrix.c4 = matrix[0].c4 * deformVertex->boneWeight[0]
							 + matrix[1].c4 * deformVertex->boneWeight[1]
							 + matrix[2].c4 * deformVertex->boneWeight[2]
							 + matrix[3].c4 * deformVertex->boneWeight[3];

				//D

				outMatrix.d1 = matrix[0].d1 * deformVertex->boneWeight[0]
							 + matrix[1].d1 * deformVertex->boneWeight[1]
							 + matrix[2].d1 * deformVertex->boneWeight[2]
							 + matrix[3].d1 * deformVertex->boneWeight[3];

				outMatrix.d2 = matrix[0].d2 * deformVertex->boneWeight[0]
							 + matrix[1].d2 * deformVertex->boneWeight[1]
							 + matrix[2].d2 * deformVertex->boneWeight[2]
							 + matrix[3].d2 * deformVertex->boneWeight[3];

				outMatrix.d3 = matrix[0].d3 * deformVertex->boneWeight[0]
							 + matrix[1].d3 * deformVertex->boneWeight[1]
							 + matrix[2].d3 * deformVertex->boneWeight[2]
							 + matrix[3].d3 * deformVertex->boneWeight[3];

				outMatrix.d4 = matrix[0].d4 * deformVertex->boneWeight[0]
							 + matrix[1].d4 * deformVertex->boneWeight[1]
							 + matrix[2].d4 * deformVertex->boneWeight[2]
							 + matrix[3].d4 * deformVertex->boneWeight[3];
			}

			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;

			//�@���ϊ��p�Ɉړ�������
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;

			//���_�o�b�t�@�֏�������
			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);

	}



}

void CanimatedModel::Draw() {

	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++) {

		aiMesh* mesh = m_Scene->mMeshes[m];

		aiMaterial* material = m_Scene->mMaterials[mesh->mMaterialIndex];

		//�e�N�X�`���ݒ�
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[path.data]);

		//���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�ݒ�
		CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//�|���S���`��
		CRenderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);

	}

}