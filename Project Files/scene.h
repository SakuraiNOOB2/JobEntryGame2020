#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "game_object.h"
#include "camera.h"
#include "field.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "grass.h"

class CScene
{
protected:
	std::list<CGameObject*>	m_GameObject[3];

public:
	CScene(){}
	virtual ~CScene(){}

	virtual void Init()
	{
		CBullet::Load();
		CExplosion::Load();

/*
		CCamera* camera = new CCamera();
		camera->Init();
		m_GameObject.push_back(camera);

		CField* field = new CField();
		field->Init();
		m_GameObject.push_back(field);

		CPlayer* player = new CPlayer();
		player->Init();
		m_GameObject.push_back(player);

		CPolygon* polygon = new CPolygon();
		polygon->Init();
		m_GameObject.push_back(polygon);
*/
		AddGameObject<CCamera>(0);
		AddGameObject<CField>(1);
		AddGameObject<CPlayer>(1);
/*
		AddGameObject<CEnemy>();
		AddGameObject<CEnemy>();
		AddGameObject<CEnemy>();
*/

		//CEnemy* enemy = AddGameObject<CEnemy>();
		//enemy->SetPosition( D3DXVECTOR3( -5.0f, 0.0f, 5.0f ) );

		AddGameObject<CPolygon>(2);

		AddGameObject<CEnemy>(1)->SetPosition( D3DXVECTOR3( -5.0f, 0.0f, 5.0f ) );
		AddGameObject<CEnemy>(1)->SetPosition( D3DXVECTOR3(  0.0f, 0.0f, 5.0f ) );
		AddGameObject<CEnemy>(1)->SetPosition( D3DXVECTOR3(  5.0f, 0.0f, 5.0f ) );

		AddGameObject<CGrass>(1)->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 5.0f));
	}

	virtual void Uninit()
	{
		for( int i = 0; i < 3; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();
		}

		CBullet::Unload();
		CExplosion::Unload();
	}

	virtual void Update()
	{
		for( int i = 0; i < 3; i++)
		{
			for( CGameObject* object : m_GameObject[i] )
			{
				object->Update();
			}
			m_GameObject[i].remove_if( [](CGameObject* object)
				{ return object->Destroy(); } );
			//??ƒ_Ž®
		}
	}

	virtual void Draw()
	{
		for( int i = 0; i < 3; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}


	template <typename T>
	T* AddGameObject( int Layer )
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back( gameObject );
		gameObject->Init();

		return gameObject;
	}


	template <typename T>
	T* GetGameObject( int Layer )
	{
		for( CGameObject* object : m_GameObject[Layer] )
		{
			if( typeid(*object) == typeid(T) )//Œ^‚ð’²‚×‚é
			{
				return (T*)object;
			}
		}

		return NULL;
	}


	template <typename T>
	std::vector<T*> GetGameObjects( int Layer )
	{
		std::vector<T*> objects;//STL‚Ì”z—ñ
		for( CGameObject* object : m_GameObject[Layer] )
		{
			if( typeid(*object) == typeid(T) )
			{
				objects.push_back((T*)object);
			}
		}

		return objects;
	}



/*
	void DestroyGameObject( CGameObject* GameObject )
	{
		GameObject->SetDestroy();
	}
*/

};