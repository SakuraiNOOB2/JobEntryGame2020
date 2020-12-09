
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "bullet.h"
#include "enemy.h"
#include "scene.h"
#include "explosion.h"


CModel* CBullet::m_Model;

void CBullet::Load()
{
	m_Model = new CModel();
	m_Model->Load( "asset\\model\\cube.obj" );
}

void CBullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void CBullet::Init()
{
	m_Position = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Scale = D3DXVECTOR3( 0.2f, 0.2f, 0.2f );
}

void CBullet::Uninit()
{

}


void CBullet::Update()
{
	m_Position.z += 0.1f;


	CScene* scene = CManager::GetScene();

	if( m_Position.z > 10.0f )
	{
		scene->AddGameObject<CExplosion>(1)->SetPosition( m_Position );
		SetDestroy();
		return;
	}



	std::vector<CEnemy*> enemyList = scene->GetGameObjects<CEnemy>(1);

	for( CEnemy* enemy : enemyList )
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPosition;
		
		D3DXVECTOR3 obbx, obbz;
		float obbLenX, obbLenZ;

		obbx = enemy->GetObbX();
		obbLenX = D3DXVec3Length(&obbx);
		obbx /= obbLenX;


		obbz = enemy->GetObbZ();
		obbLenZ = D3DXVec3Length(&obbz);
		obbz /= obbLenZ;

		float lenX, lenZ;
		lenX = D3DXVec3Dot(&obbx, &direction);
		lenZ = D3DXVec3Dot(&obbz, &direction);

		if (fabs(lenX) < obbLenX && fabs(lenZ) < obbLenZ) {

				SetDestroy();
		}


		//float length = D3DXVec3Length( &direction );
		//if( length < 2.0f )
		//{
		//	scene->AddGameObject<CExplosion>(1)->SetPosition( m_Position );

		//	enemy->SetDestroy();
		//	SetDestroy();
		//	return;
		//}
	}

}




void CBullet::Draw()
{

	// マト?クス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();

}


