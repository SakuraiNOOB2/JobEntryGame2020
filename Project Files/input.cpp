
#include "main.h"
#include "input.h"
#include <WinUser.h>

//Keyboard
BYTE CInput::m_OldKeyState[256];
BYTE CInput::m_KeyState[256];

//Mouse
LPDIRECTINPUT8 CInput::m_directInput;
Microsoft::WRL::ComPtr<IDirectInputDevice8> CInput::m_DImouse;
DIMOUSESTATE CInput::m_mouseState;
DIMOUSESTATE CInput::m_oldMouseState;

void CInput::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

}

void CInput::Uninit()
{


}

void CInput::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );

	m_oldMouseState = m_mouseState;
}

bool CInput::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool CInput::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool CInput::InitDirectInput(HINSTANCE hInstance)
{
	HRESULT hr;
	
	hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		CLSID_DirectInput8,
		(void**)&m_directInput,
		NULL);

	hr = m_directInput->CreateDevice(GUID_SysMouse,
		m_DImouse.GetAddressOf(),
		NULL);

	hr = m_DImouse->SetDataFormat(&c_dfDIMouse);
	hr = m_DImouse->SetCooperativeLevel(GetActiveWindow(), DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;

}

DIMOUSESTATE CInput::GetMouseState(bool isOldMouseState)
{
	
	m_DImouse->Acquire();

	if (isOldMouseState) {

		m_DImouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_oldMouseState);
		return m_oldMouseState;
	}
	else {
		m_DImouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
		return m_mouseState;
	}

}
