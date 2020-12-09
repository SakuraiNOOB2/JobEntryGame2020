#pragma once

#include "game_object.h"

class CEnemy : public CGameObject
{
private:
	class CModel* m_Model;
	//前方宣言（ポイ?タ変?の有効）

public:
	CEnemy() {}
	~CEnemy() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};