#pragma once
#include "framework.h"
#include <commdlg.h>
#include "MessageHider.h"
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;
class UIManager
{

public:

	HWND hDecryptButton;
	HWND hEncryptButton;
	Image* pImage;

	UIManager() = default;

	void Init();
	void LoadImage();
	void CreateButtons(HWND hWnd); 
	void ShowButtons(); 
	void HideButtons();


	LRESULT CALLBACK ProcessWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

