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
	int ImageMaxLength;
	std::string imagePath;
	FileManager* pFileManager; 
	HWND hCharCountLabel;
	HWND hDropdown; // Handle pour la combobox
	HWND hDropdownAlgo; // Handle pour la combobox
	HWND hDropdownButton; // Handle pour le bouton associ�

	JournalManager* journalManager; // Gestionnaire du journal

	UIManager() = default;

	void Init();
	void CreateButtons(HWND hWnd);
	void CreateTextBox(HWND hWnd); 
	void CreateDropdownAndButton(HWND hWnd);

	void ShowControls(); 
	void HideControls(); 

	void ShowJournal();
	void PrintText(LPCWSTR message);

	std::wstring GetTextBoxContent();


	void LoadImage();
	void ClickDecrypt(HWND hWnd);
	void ClickCrypt(HWND hWnd);


	LRESULT CALLBACK ProcessWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

