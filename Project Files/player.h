#pragma once

#include "game_object.h"

class CPlayer : public CGameObject
{

private:
	/*class CModel* m_Model;*/

	class CanimatedModel* m_Model;

	int m_Frame = 0;

public:

	CPlayer() {}
	~CPlayer() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};