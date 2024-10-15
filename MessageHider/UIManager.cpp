#include "UIManager.h"
#include <string>

void UIManager::Init()
{
    UIManager::pImage = nullptr;
    std::wstring name(L"C:/Users/BN/Downloads/ARTICLE-INOX.jpg");
    const wchar_t* szName = name.c_str();
    pImage = new Image(szName);
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
        // Analyse les sélections de menu :
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Taille du rectangle
        int rectWidth = 200;
        int rectHeight = 150;

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
                newWidth = rectWidth;
                newHeight = static_cast<int>(rectWidth / aspectRatio);
            }
            else {
                newHeight = rectHeight;
                newWidth = static_cast<int>(rectHeight * aspectRatio);
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


    



