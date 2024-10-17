#include "UIManager.h"
#include <string>

#include "FileManager.h"
#include "FileHandler.h"
#include <commdlg.h>
#include <string>
#include <algorithm>

#include <commdlg.h>
#include <string>
#include <algorithm>
#include <codecvt>

void UIManager::Init()
{
	UIManager::pImage = nullptr;
	journalManager = new JournalManager();
	pFileManager = new FileManager();
	pFileManager->SelectAlgorithm(ESteganoAlgorithm::BasicSteganoR);
}

void UIManager::LoadImage()
{
	OPENFILENAME ofn;
	WCHAR szFile[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Images (*.bmp;*.jpg;*.jpeg;*.png)\0*.bmp;*.jpg;*.jpeg;*.png\0Tous les fichiers (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) {
		delete pImage;
		pImage = new Gdiplus::Image(ofn.lpstrFile);

		std::string path = FileHandler::ConvertLPWSTRToString(ofn.lpstrFile);
		imagePath = path;
		pFileManager->LoadImageFromFilename(path);
		ImageMaxLength = pFileManager->GetMessageMaxLenght();

		HWND hWnd = GetActiveWindow();
		InvalidateRect(hWnd, nullptr, TRUE);
		CreateButtons(hWnd);
		CreateTextBox(hWnd);
		CreateDropdownAndButton(hWnd);

		ShowControls();
	}
	//redraw
	HWND hWnd = GetActiveWindow();
	InvalidateRect(hWnd, nullptr, TRUE);
}

void UIManager::ClickDecrypt(HWND hWnd)
{
	std::wstring content = this->GetTextBoxContent();
	std::string message = pFileManager->Decrypt();
	SetWindowTextW(hTextBox, FileHandler::ConvertStringToWString(message).c_str());
}

void UIManager::ClickCrypt(HWND hWnd) {
	if (imagePath.empty()) {
		MessageBox(hWnd, L"Aucune image à crypter.", L"Erreur", MB_OK | MB_ICONERROR);
		return;
	}

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wideImagePath = converter.from_bytes(imagePath);

	std::wstring fileName = wideImagePath.substr(wideImagePath.find_last_of(L"\\") + 1);
	std::wstring baseName = fileName.substr(0, fileName.find_last_of(L"."));
	std::wstring extension = fileName.substr(fileName.find_last_of(L"."));

	std::wstring defaultFileName = baseName + L"_secret" + extension;

	OPENFILENAME ofn;
	WCHAR szFile[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Images (*.bmp;*.jpg;*.jpeg;*.png)\0*.bmp;*.jpg;*.jpeg;*.png\0Tous les fichiers (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	wcscpy_s(szFile, defaultFileName.c_str());

	if (GetSaveFileName(&ofn)) {
		std::wstring filePath = ofn.lpstrFile;
		pFileManager->SetPath(filePath);

		std::wstring content = this->GetTextBoxContent();

		int selectedIndex = SendMessage(hDropdown, CB_GETCURSEL, 0, 0);
		;		
		switch (selectedIndex)
		{
		case 0: // NO FILTER
			break;
		case 1: // GRAY
			pFileManager->AddFilter(EFilterType::Grayscale);
			break;
		case 2: // NEGATIVE
			pFileManager->AddFilter(EFilterType::Negative);
			break;
		case 3: // SEPIA
			pFileManager->AddFilter(EFilterType::Sepia);
			break;
		case 4: // BLUR
			pFileManager->AddFilter(EFilterType::Blur);
			break;
		case 5: // SHARPENING
			pFileManager->AddFilter(EFilterType::Sharpening);
			break;
		case 6: // EDGE DETECTION
			pFileManager->AddFilter(EFilterType::EdgeDetection);
			break;
		default:
			break;
		}
		pFileManager->ApplyFilters();

		int selectedAlgoIndex = SendMessage(hDropdown, CB_GETCURSEL, 0, 0);
		switch (selectedAlgoIndex)
		{
		case 0: // Algo normal
			break;
		case 1: // Algo Complexe
			break;
		default:
			break;
		}

		pFileManager->Encrypt(FileHandler::ConvertWStringToString(content));
	}
}

INT_PTR UIManager::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT UIManager::ProcessWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case 1: // Load Image
			this->LoadImage();
			SetFocus(hWnd);
			break;
		case 2: // Décrypter
		{
			ClickDecrypt(hWnd);
			SetFocus(hWnd);
		}
		break;
		case 3: // Crypter
		{
			ClickCrypt(hWnd);
			SetFocus(hWnd);
		}
		break;
		case 4: {
			if (HIWORD(wParam) == WM_KEYDOWN) {
				if (wParam == 'L')
					JournalManager::Instance->LogWrite(L"ouais");
			}
			if (HIWORD(wParam) == EN_CHANGE) {
				int length = GetWindowTextLength(hTextBox);
				std::wstring charCountText = L"Caractères: " + std::to_wstring(length) + L"/100";

				// Met à jour le texte du label
				SetWindowTextW(hCharCountLabel, charCountText.c_str());
				WCHAR buffer[256];
				auto tt = GetWindowText(hCharCountLabel, buffer, sizeof(buffer) / sizeof(WCHAR)); // Récupérer le texte
			}
		}
			  break;
		case ID_FICHIER_CHARGERUNEIMAGE:
		{
			this->LoadImage();
			SetFocus(hWnd);
		}
		break;
		case ID_FICHIER_CRYPTERUNEIMAGE:
			this->ClickCrypt(hWnd);
			SetFocus(hWnd);
			break;
		case ID_FICHIER_DECRYPTERUNEIMAGE:
			this->ClickDecrypt(hWnd);
			SetFocus(hWnd);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_AIDE_LACHANSONOFFICIELLE:
			ShellExecute(0, 0, L"https://www.youtube.com/watch?v=zBbSH-w6L_8", 0, 0, SW_SHOW);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_NOTIFY:
	{
		if (((LPNMHDR)lParam)->idFrom == (UINT_PTR)hTextBox && ((LPNMHDR)lParam)->code == EN_CHANGE)
		{
			// Met à jour le compteur de caractères
			int length = GetWindowTextLength(hTextBox);
			std::wstring charCountText = L"Caractères: " + std::to_wstring(length) + L"/100";
			SetWindowText(hCharCountLabel, charCountText.c_str());
		}
		break;
	}
	case WM_KEYDOWN:
	{
		if (GetKeyState(VK_CONTROL) & 0x8000) // touche ctrl
		{
			switch (wParam)
			{
			case 'L': // Ctrl + L
				this->LoadImage();
				break;
			case 'J': // Ctrl + L
				this->ShowJournal();
				break;
			case 'D': // Ctrl + D
				if (pImage != nullptr)
					this->ClickDecrypt(hWnd);
				else
					MessageBox(hWnd, L"Veuillez charger une image", L"Erreur", MB_OK | MB_ICONERROR);
				break;
			case 'C': // Ctrl + C
				if (pImage != nullptr)
					this->ClickCrypt(hWnd);
				else
					MessageBox(hWnd, L"Veuillez charger une image", L"Erreur", MB_OK | MB_ICONERROR);
				break;
			}
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		int rectWidth = 300;
		int rectHeight = 200;

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		int windowWidth = clientRect.right - clientRect.left;
		int windowHeight = clientRect.bottom - clientRect.top;

		int x = (windowWidth - rectWidth) / 2 + 150;
		int y = (windowHeight - rectHeight) / 2 - 150;

		Rectangle(hdc, x, y, x + rectWidth, y + rectHeight);

		if (pImage != nullptr) {
			float aspectRatio = static_cast<float>(pImage->GetWidth()) / static_cast<float>(pImage->GetHeight());
			int newWidth;
			int newHeight;

			if (aspectRatio > static_cast<float>(rectWidth) / static_cast<float>(rectHeight)) {
				newWidth = rectWidth - 2;
				newHeight = static_cast<int>(rectWidth / aspectRatio) - 2;
			}
			else {
				newHeight = rectHeight - 2;
				newWidth = static_cast<int>(rectHeight * aspectRatio) - 2;
			}

			int imgX = x + (rectWidth - newWidth) / 2;
			int imgY = y + (rectHeight - newHeight) / 2;

			Graphics graphics(hdc);
			graphics.DrawImage(pImage, imgX, imgY, newWidth, newHeight);
		}

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void UIManager::CreateButtons(HWND hWnd) {
	// Décrypter
	hDecryptButton = CreateWindow(
		L"BUTTON",
		L"Décrypter",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		50,  // Position X
		100,  // Position Y
		100,  // Width
		30,  // Height
		hWnd,  // Parent window
		(HMENU)2,  // Button ID
		nullptr,
		nullptr);
	// Crypter
	hEncryptButton = CreateWindow(
		L"BUTTON",  // Class name
		L"Crypter",  // Button text
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
		250,  // Position X
		100,  // Position Y
		100,  // Width
		30,  // Height
		hWnd,  // Parent window
		(HMENU)3,  // Button ID
		nullptr,
		nullptr);
}

void UIManager::CreateDropdownAndButton(HWND hWnd)
{
	hDropdown = CreateWindow(
		L"COMBOBOX",   // Classe de la combobox
		nullptr,       // Pas de texte initial
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, // Styles
		50,  // Position X
		400, // Position Y
		150, // Largeur
		160, // Hauteur
		hWnd, // Fenêtre parent
		nullptr,
		nullptr,
		nullptr
	);

	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Aucun");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Nuance de gris");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Negatif");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Sepia");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Flou");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Nettete");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Detection de contours");

	// Sélectionner le premier élément par défaut
	SendMessage(hDropdown, CB_SETCURSEL, 0, 0);


	hDropdownAlgo = CreateWindow(
		L"COMBOBOX",   // Classe de la combobox
		nullptr,       // Pas de texte initial
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, // Styles
		200,  // Position X
		400, // Position Y
		150, // Largeur
		160, // Hauteur
		hWnd, // Fenêtre parent
		nullptr,
		nullptr,
		nullptr
	);

	SendMessage(hDropdownAlgo, CB_ADDSTRING, 0, (LPARAM)L"Algo basique");
	SendMessage(hDropdownAlgo, CB_ADDSTRING, 0, (LPARAM)L"Algo complexe");

	// Sélectionner le premier élément par défaut
	SendMessage(hDropdownAlgo, CB_SETCURSEL, 0, 0);

	
}

void UIManager::ShowControls() {
	ShowWindow(hDecryptButton, SW_SHOW);
	ShowWindow(hEncryptButton, SW_SHOW);
	ShowWindow(hTextBox, SW_SHOW);
}

void UIManager::HideControls() {
	ShowWindow(hDecryptButton, SW_HIDE);
	ShowWindow(hEncryptButton, SW_HIDE);
	ShowWindow(hTextBox, SW_HIDE);
}

void UIManager::ShowJournal()
{
	// Initialiser le journal s'il n'a pas déjà été créé
	if (!journalManager->hJournalWnd)
		JournalManager::Instance->Init(GetModuleHandle(nullptr), GetActiveWindow());
	ShowWindow(journalManager->hJournalWnd, SW_SHOW);
}

void UIManager::PrintText(LPCWSTR message)
{
	SetWindowTextW(hTextBox, message);
}

std::wstring UIManager::GetTextBoxContent() {
	WCHAR buffer[256];
	GetWindowText(hTextBox, buffer, sizeof(buffer) / sizeof(WCHAR));
	return std::wstring(buffer);
}

void UIManager::CreateTextBox(HWND hWnd) {
	// Créer la zone de texte
	hTextBox = CreateWindowEx(
		WS_EX_CLIENTEDGE,    // Style étendu
		L"EDIT",             // Class name
		L"",                 // Initial text
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, // Styles
		50,                 // Position X
		250,                // Position Y
		300,                // Width
		100,                // Height
		hWnd,               // Parent window
		(HMENU)4,          // ID de la zone de texte
		nullptr,
		nullptr);

	// Limiter le nombre de caractères à 100
	SendMessage(hTextBox, EM_SETLIMITTEXT, ImageMaxLength, 0);

	//// Créer un contrôle STATIC pour afficher le nombre de caractères
	//HWND hCharCountLabel = CreateWindowEx(
	//	0,                    // Style étendu
	//	L"STATIC",           // Class name
	//	L"Caractères: 0/100",// Initial text
	//	WS_CHILD | WS_VISIBLE, // Styles
	//	50,                  // Position X
	//	360,                 // Position Y
	//	200,                 // Width
	//	20,                  // Height
	//	hWnd,                // Parent window
	//	nullptr,
	//	nullptr,
	//	nullptr);
}