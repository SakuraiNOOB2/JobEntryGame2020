#pragma once

#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

#include <dinput.h>

#include <wrl.h>

class CInput
{
private:
	
	

	//Keyboard
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	//Mouse
	static LPDIRECTINPUT8 m_directInput;
	static Microsoft::WRL::ComPtr<IDirectInputDevice8> m_DImouse;
	static DIMOUSESTATE m_oldMouseState;
	static DIMOUSESTATE m_mouseState;

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	//Init Mouse
	static bool InitDirectInput(HINSTANCE hInstance);
	static DIMOUSESTATE GetMouseState(bool isOldMouseState);

};
