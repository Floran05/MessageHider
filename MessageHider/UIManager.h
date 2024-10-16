#pragma once
#include "framework.h"
#include <string>
#include <commdlg.h>
#include <windows.h>
#include <shellapi.h>
#include "MessageHider.h"
#include <objidl.h>
#include <gdiplus.h>
#include "JournalManager.h"
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

class FileManager;

class UIManager
{

public:

	HWND hDecryptButton;
	HWND hEncryptButton;
	HWND hTextBox; 
	Image* pImage;
	FileManager* pFileManager;

	JournalManager* journalManager; // Gestionnaire du journal

	UIManager() = default;

	void Init();
	void CreateButtons(HWND hWnd);
	void CreateTextBox(HWND hWnd); 
	void ShowControls(); 
	void HideControls(); 

	void ShowJournal();

	std::wstring GetTextBoxContent();


	void LoadImage();
	void ClickDecrypt(HWND hWnd);
	void ClickCrypt(HWND hWnd);


	LRESULT CALLBACK ProcessWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

