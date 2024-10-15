#pragma once
#include "framework.h"
#include "MessageHider.h"
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;
class UIManager
{

public:


	Image* pImage;

	UIManager() = default;

	void Init();
	LRESULT CALLBACK ProcessWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

