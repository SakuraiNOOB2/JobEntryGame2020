
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "scene.h"
#include "player.h"


void CCamera::Init()
{

	m_Position = D3DXVECTOR3( 0.0f, 5.0f, -3.0f );
	m_Target = D3DXVECTOR3( 0.0f, 0.0f, 2.0f );

}


void CCamera::Uninit()
{

}


void CCamera::Update()
{

	CScene* scene = CManager::GetScene();
	CPlayer* player = scene->GetGameObject<CPlayer>(1);


	m_Target = player->GetPosition();
	m_Position = m_Target + D3DXVECTOR3(0.0f, 2.0f, -5.0f);


	if (CInput::GetKeyPress(VK_UP))
		m_Target.y += 0.1f;

	if (CInput::GetKeyPress(VK_DOWN))
		m_Target.y -= 0.1f;

	if (CInput::GetKeyPress(VK_LEFT))
		m_Target.x += 0.1f;

	if (CInput::GetKeyPress(VK_RIGHT))
		m_Target.x -= 0.1f;
}


void CCamera::Draw()
{

	//View Matrix
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);
	CRenderer::SetViewMatrix(&m_ViewMatrix);


	//Projection
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);

}

bool CCamera::CheckView(D3DXVECTOR3 Position)
{
	D3DXMATRIX vp, invvp;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, n;

	v = Position - m_Position;

	//Left
	v1 = wpos[0] - m_Position;
	v2 = wpos[2] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	if (D3DXVec3Dot(&n, &v) < 0.0f) {

		return false;
	}

	//Right
	v1 = wpos[1] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	if (D3DXVec3Dot(&n, &v) > 1.0f) {

		return false;
	}

	//Up
	v1 = wpos[0] - m_Position;
	v2 = wpos[1] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	if (D3DXVec3Dot(&n, &v) > 1.0f) {

		return false;
	}

	//Bottom
	v1 = wpos[2] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	if (D3DXVec3Dot(&n, &v) < 0.0f) {

		return false;
	}

	return true;
}

