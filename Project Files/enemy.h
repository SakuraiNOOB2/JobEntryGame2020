#pragma once

#include "game_object.h"

class CEnemy : public CGameObject
{
private:
	class CModel* m_Model;
	//�O���錾�i�|�C?�^��?�̗L���j

public:
	CEnemy() {}
	~CEnemy() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};