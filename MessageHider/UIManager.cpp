#include "UIManager.h"
#include <string>

#include "FileManager.h"
#include "FileHandler.h"

void UIManager::Init()
{
	UIManager::pImage = nullptr;
	journalManager = new JournalManager();
	pFileManager = new FileManager();
	pFileManager->SelectAlgorithm(ESteganoAlgorithm::BasicSteganoR);

	// Ajouter l'appel pour créer le dropdown et le bouton
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

		HWND hWnd = GetActiveWindow();
		InvalidateRect(hWnd, nullptr, TRUE);
		// Créer et afficher les boutons
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

#include <commdlg.h> // Pour la boîte de dialogue de sauvegarde
#include <string>
#include <algorithm> // Pour std::find_last_of

#include <commdlg.h> // Pour la boîte de dialogue de sauvegarde
#include <string>
#include <algorithm> // Pour std::find_last_of
#include <codecvt>   // Pour la conversion entre std::string et std::wstring

void UIManager::ClickCrypt(HWND hWnd) {
	// Vérifier si imagePath est défini
	if (imagePath.empty()) {
		MessageBox(hWnd, L"Aucune image à crypter.", L"Erreur", MB_OK | MB_ICONERROR);
		return;
	}

	// Convertir std::string en std::wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wideImagePath = converter.from_bytes(imagePath);

	// Extraire le nom de fichier et l'extension
	std::wstring fileName = wideImagePath.substr(wideImagePath.find_last_of(L"\\") + 1); // Obtenir le nom du fichier
	std::wstring baseName = fileName.substr(0, fileName.find_last_of(L"."));
	std::wstring extension = fileName.substr(fileName.find_last_of(L"."));

	// Initialiser le chemin de sauvegarde avec le nom du fichier d'origine
	std::wstring defaultFileName = baseName + L"_secret" + extension; // Ajoute "_copy" au nom de fichier

	// Structure pour la boîte de dialogue
	OPENFILENAME ofn;
	WCHAR szFile[260];      // Buffer pour le chemin du fichier

	// Initialiser la structure OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0'; // Initialiser le champ
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Images (*.bmp;*.jpg;*.jpeg;*.png)\0*.bmp;*.jpg;*.jpeg;*.png\0Tous les fichiers (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_OVERWRITEPROMPT; // Afficher un avertissement si le fichier existe déjà

	// Proposer un nom de fichier par défaut
	wcscpy_s(szFile, defaultFileName.c_str()); // Définir le nom par défaut

	// Ouvrir la boîte de dialogue de sauvegarde
	if (GetSaveFileName(&ofn)) {
		// Le chemin du fichier est maintenant dans ofn.lpstrFile
		std::wstring filePath = ofn.lpstrFile;
		pFileManager->SetPath(filePath);

		std::wstring content = this->GetTextBoxContent();

		int selectedIndex = SendMessage(hDropdown, CB_GETCURSEL, 0, 0);
;		switch (selectedIndex)
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
		case 1: // Identifiant du bouton "Load Image"
			this->LoadImage();
			break;
		case 2: // Identifiant du bouton "Décrypter"
		{
			ClickDecrypt(hWnd);
		}
		break;
		case 3: // Identifiant du bouton "Crypter"
		{
			ClickCrypt(hWnd);
		}
		break;
		case 4: {
			if (HIWORD(wParam) == EN_CHANGE) {
				int length = GetWindowTextLength(hTextBox);
				std::wstring charCountText = L"Caractères: " + std::to_wstring(length) + L"/100";

				// Met à jour le texte du label
				SetWindowTextW(hCharCountLabel, charCountText.c_str());
				WCHAR buffer[256]; // Buffer pour stocker le contenu
				auto tt = GetWindowText(hCharCountLabel, buffer, sizeof(buffer) / sizeof(WCHAR)); // Récupérer le texte

				//// Force uniquement le rafraîchissement du label
				//InvalidateRect(hCharCountLabel, nullptr, FALSE);
				//UpdateWindow(hCharCountLabel);
			}
		}
			  break;
		//case 5:
		//{
		//	if (selectedIndex != CB_ERR)
		//	{
		//		// Convertir l'index en chaîne de caractères
		//		std::wstring indexText = L"Index sélectionné : " + std::to_wstring(selectedIndex);

		//		// Afficher une boîte de message avec l'index sélectionné
		//		MessageBox(hWnd, indexText.c_str(), L"Index de l'option", MB_OK);
		//	}
		//	break;
		//}
		case ID_FICHIER_CHARGERUNEIMAGE:
		{
			this->LoadImage(); }
		break;
		case ID_FICHIER_CRYPTERUNEIMAGE:
			this->ClickCrypt(hWnd);

			break;
		case ID_FICHIER_DECRYPTERUNEIMAGE:
			this->ClickDecrypt(hWnd);
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
	case WM_NOTIFY: // Intercepter les notifications
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
		if (GetKeyState(VK_CONTROL) & 0x8000) // Vérifie si la touche Ctrl est enfoncée
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
				if (pImage != nullptr) {
					this->ClickDecrypt(hWnd);
				}
				else
				{
					MessageBox(hWnd, L"Veuillez charger une image", L"Erreur", MB_OK | MB_ICONERROR);
				}
				break;
			case 'C': // Ctrl + C
				if (pImage != nullptr) {
					this->ClickCrypt(hWnd);
				}
				else
				{
					MessageBox(hWnd, L"Veuillez charger une image", L"Erreur", MB_OK | MB_ICONERROR);
				}

				break;
			}
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);


		// Taille du rectangle
		int rectWidth = 300;
		int rectHeight = 200;

		// Récupérer la taille de la fenêtre
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);


		int windowWidth = clientRect.right - clientRect.left;
		int windowHeight = clientRect.bottom - clientRect.top;

		// Calculer les coordonnées pour centrer le rectangle
		int x = (windowWidth - rectWidth) / 2 + 150;
		int y = (windowHeight - rectHeight) / 2 - 150;

		// Dessiner le rectangle centré
		Rectangle(hdc, x, y, x + rectWidth, y + rectHeight);

		// Si l'image est chargée, dessinez-la
		if (pImage != nullptr) {
			// Calculer les nouvelles dimensions pour préserver le rapport d'aspect
			float aspectRatio = static_cast<float>(pImage->GetWidth()) / static_cast<float>(pImage->GetHeight());
			int newWidth;  // Déclarer newWidth ici
			int newHeight; // Déclarer newHeight ici

			if (aspectRatio > static_cast<float>(rectWidth) / static_cast<float>(rectHeight)) {
				newWidth = rectWidth - 2;
				newHeight = static_cast<int>(rectWidth / aspectRatio) - 2;
			}
			else {
				newHeight = rectHeight - 2;
				newWidth = static_cast<int>(rectHeight * aspectRatio) - 2;
			}

			// Calculer la position pour centrer l'image
			int imgX = x + (rectWidth - newWidth) / 2;
			int imgY = y + (rectHeight - newHeight) / 2;

			// Créer un contexte de dessin GDI+
			Graphics graphics(hdc);
			graphics.DrawImage(pImage, imgX, imgY, newWidth, newHeight); // Dessiner l'image
		}

		// Terminer le dessin
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
	// Créer le bouton "Décrypter"
	hDecryptButton = CreateWindow(
		L"BUTTON",  // Class name
		L"Décrypter",  // Button text
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
		50,  // Position X
		100,  // Position Y
		100,  // Width
		30,  // Height
		hWnd,  // Parent window
		(HMENU)2,  // Button ID
		nullptr,  // Instance handle
		nullptr); // No additional parameters
	// Créer le bouton "Crypter"
	hEncryptButton = CreateWindow(
		L"BUTTON",  // Class name
		L"Crypter",  // Button text
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
		200,  // Position X
		100,  // Position Y
		100,  // Width
		30,  // Height
		hWnd,  // Parent window
		(HMENU)3,  // Button ID
		nullptr,  // Instance handle
		nullptr); // No additional parameters
}

void UIManager::CreateDropdownAndButton(HWND hWnd)
{
	// Créer le dropdown (combobox)
	hDropdown = CreateWindow(
		L"COMBOBOX",   // Classe de la combobox
		nullptr,       // Pas de texte initial
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, // Styles
		50,  // Position X
		400, // Position Y
		150, // Largeur
		160, // Hauteur
		hWnd, // Fenêtre parent
		nullptr, // Pas d'ID pour ce cas
		nullptr, // Pas d'instance spécifique
		nullptr  // Pas de paramètres additionnels
	);

	// Ajouter les éléments dans le dropdown
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Aucun");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Nuance de gris");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Negatif");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Sepia");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Flou");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Nettete");
	SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)L"Detection de contours");


	// Sélectionner le premier élément par défaut
	SendMessage(hDropdown, CB_SETCURSEL, 0, 0);

	// Créer le bouton à côté du dropdown
	hDropdownButton = CreateWindow(
		L"BUTTON",     // Classe du bouton
		L"Valider",    // Texte du bouton
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, // Styles
		270, // Position X
		400, // Position Y
		80,  // Largeur
		30,  // Hauteur
		hWnd, // Fenêtre parent
		(HMENU)5, // ID du bouton pour WM_COMMAND
		nullptr, // Pas d'instance spécifique
		nullptr  // Pas de paramètres additionnels
	);
}


void UIManager::ShowControls() {
	ShowWindow(hDecryptButton, SW_SHOW);
	ShowWindow(hEncryptButton, SW_SHOW);
	ShowWindow(hTextBox, SW_SHOW); // Afficher la zone de texte


}

void UIManager::HideControls() {
	ShowWindow(hDecryptButton, SW_HIDE);
	ShowWindow(hEncryptButton, SW_HIDE);
	ShowWindow(hTextBox, SW_HIDE); // Cacher la zone de texte
}

void UIManager::ShowJournal()
{

	// Initialiser le journal s'il n'a pas déjà été créé
	if (!journalManager->hJournalWnd)
	{
		JournalManager::Instance->Init(GetModuleHandle(nullptr), GetActiveWindow());
	}
	ShowWindow(journalManager->hJournalWnd, SW_SHOW);
}

void UIManager::PrintText(LPCWSTR message)
{
	SetWindowTextW(hTextBox, message);
}


std::wstring UIManager::GetTextBoxContent() {
	WCHAR buffer[256]; // Buffer pour stocker le contenu
	GetWindowText(hTextBox, buffer, sizeof(buffer) / sizeof(WCHAR)); // Récupérer le texte
	return std::wstring(buffer); // Convertir en wstring et retourner
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
		nullptr,           // Instance handle
		nullptr);          // No additional parameters

	// Limiter le nombre de caractères à 100
	SendMessage(hTextBox, EM_SETLIMITTEXT, 100, 0);

	// Créer un contrôle STATIC pour afficher le nombre de caractères
	HWND hCharCountLabel = CreateWindowEx(
		0,                    // Style étendu
		L"STATIC",           // Class name
		L"Caractères: 0/100",// Initial text
		WS_CHILD | WS_VISIBLE, // Styles
		50,                  // Position X
		360,                 // Position Y
		200,                 // Width
		20,                  // Height
		hWnd,                // Parent window
		nullptr,            // No ID for this label
		nullptr,           // Instance handle
		nullptr);          // No additional parameters
}














