#pragma once
#include "framework.h"
#include "MessageHider.h"
class UIManager
{

public:

	UIManager() = default;


	LRESULT CALLBACK ProcessWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

