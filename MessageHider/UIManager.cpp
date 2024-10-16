#include "UIManager.h"
#include <string>

#include "FileManager.h"
#include "FileHandler.h"

void UIManager::Init()
{
	UIManager::pImage = nullptr;
	journalManager = new JournalManager();
    pFileManager = new FileManager();
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

        pFileManager->LoadImageFromFilename(FileHandler::ConvertToChar(ofn.lpstrFile));

        HWND hWnd = GetActiveWindow(); 
        InvalidateRect(hWnd, nullptr, TRUE); 
        // Créer et afficher les boutons
        CreateButtons(hWnd);
        CreateTextBox(hWnd);
        ShowControls();


    }



    //redraw
    HWND hWnd = GetActiveWindow(); 
    InvalidateRect(hWnd, nullptr, TRUE); 


}

void UIManager::ClickDecrypt(HWND hWnd)
{
    std::wstring content = this->GetTextBoxContent(); // Récupérer le contenu de la zone de texte
    std::string message = pFileManager->Decrypt();
    SetWindowTextW(hTextBox, content.c_str());
    MessageBox(hWnd, content.c_str(), L"Texte dans la zone de texte", MB_OK); // Afficher le contenu
}

void UIManager::ClickCrypt(HWND hWnd)
{
    std::wstring content = this->GetTextBoxContent(); // Récupérer le contenu de la zone de texte
    pFileManager->Encrypt(FileHandler::ConvertWStringToString(content));
    MessageBox(hWnd, content.c_str(), L"Texte dans la zone de texte", MB_OK); // Afficher le contenu
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
        case ID_FICHIER_CHARGERUNEIMAGE:
        {
            HWND hMyButton = GetDlgItem(hWnd, 32772);
            if (hMyButton)
            {
                // Activer le bouton
                EnableWindow(hMyButton, TRUE);
            }
            else
            {
                MessageBox(hWnd, L"Le bouton n'a pas été trouvé", L"Erreur", MB_OK | MB_ICONERROR);
            }
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
			case 'T': // Ctrl + L
				this->journalManager->LogWrite(L"ouais");
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
                newWidth = rectWidth-2;
                newHeight = static_cast<int>(rectWidth / aspectRatio)-2;
            }
            else {
                newHeight = rectHeight-2;
                newWidth = static_cast<int>(rectHeight * aspectRatio)-2;
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
        20,  // Position X
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
        170,  // Position X
        100,  // Position Y
        100,  // Width
        30,  // Height
        hWnd,  // Parent window
        (HMENU)3,  // Button ID
        nullptr,  // Instance handle
        nullptr); // No additional parameters
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
        WS_EX_CLIENTEDGE,  // Style étendu
        L"EDIT",           // Class name
        L"",               // Initial text
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, // Styles
        20,               // Position X
        150,              // Position Y
        300,              // Width
        100,              // Height
        hWnd,             // Parent window
        (HMENU)4,        // ID de la zone de texte
        nullptr,         // Instance handle
        nullptr);        // No additional parameters
}









    



