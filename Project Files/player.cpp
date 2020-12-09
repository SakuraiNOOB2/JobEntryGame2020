
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "scene.h"
#include "model.h"
#include "animatedModel.h"
#include "player.h"
#include "bullet.h"




void CPlayer::Init()
{

	//m_Model = new CModel();
	//m_Model->Load( "asset\\model\\torus.obj" );

	m_Model = new CanimatedModel();
	m_Model->Load("asset\\model\\player\\player.fbx");
	m_Model->LoadAnimation("asset\\model\\player\\idle1.fbx", "Idle1");
	m_Model->LoadAnimation("asset\\model\\player\\walk1.fbx", "Walk1");
	m_Model->LoadAnimation("asset\\model\\player\\crouchidle.fbx", "CrouchIdle");


	m_Position = D3DXVECTOR3( 0.0f, 0.5f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Scale = D3DXVECTOR3( 0.01f, 0.01f, 0.01f );


}


void CPlayer::Uninit()
{
	m_Model->Unload();
	delete m_Model;
}


void CPlayer::Update()
{

	

	if (CInput::GetKeyPress('A')) {

		m_Model->Update("Walk1", "Walk1", 1, m_Frame);
		m_Position.x -= 0.1f;

	}else if (CInput::GetKeyPress('D')) {
		
		m_Model->Update("Walk1", "Walk1", 1, m_Frame);
		m_Position.x += 0.1f;

	}else if (CInput::GetKeyPress('W')) {
		
		m_Model->Update("Walk1", "Walk1", 1, m_Frame);
		m_Position.z += 0.1f;

	}else if (CInput::GetKeyPress('S')) {
		
		m_Model->Update("Walk1", "Walk1", 1, m_Frame);
		m_Position.z -= 0.1f;

	}
	else {

		m_Model->Update("Idle1", "Idle1", 1, m_Frame);
	}
	
	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		CScene* scene = CManager::GetScene();
		scene->AddGameObject<CBullet>(1)->SetPosition( m_Position );
	}

	m_Frame++;

}




void CPlayer::Draw()
{
	
	//
	CScene* scene = CManager::GetScene();
	CCamera* camera = scene->GetGameObject<CCamera>(0);

	if (!camera->CheckView(m_Position))
		return;

	// マト?クス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();

}


