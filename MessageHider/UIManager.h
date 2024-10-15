#pragma once
#include "framework.h"
#include <string>
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
	HWND hTextBox; 
	Image* pImage;

	UIManager() = default;

	void Init();
	void LoadImage();
	void CreateButtons(HWND hWnd);
	void CreateTextBox(HWND hWnd); 
	void ShowControls(); 
	void HideControls(); 
	std::wstring GetTextBoxContent();


	LRESULT CALLBACK ProcessWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

