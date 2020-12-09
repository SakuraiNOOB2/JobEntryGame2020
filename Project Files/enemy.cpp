
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "enemy.h"
#include "scene.h"




void CEnemy::Init()
{

	m_Model = new CModel();
	m_Model->Load( "asset\\model\\cube.obj" );


	m_Position = D3DXVECTOR3( 0.0f, 0.0f, 10.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_Scale = D3DXVECTOR3( 2.0f, 1.0f, 1.0f );

}


void CEnemy::Uninit()
{
	m_Model->Unload();
	delete m_Model;
}


void CEnemy::Update()
{

	m_Rotation.y += 0.05f;

}




void CEnemy::Draw()
{
	CScene* scene = CManager::GetScene();
	CCamera* camera = scene->GetGameObject<CCamera>(0);

	if (!camera->CheckView(m_Position))
		return;


	//
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();

}


