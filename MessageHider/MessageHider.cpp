// MessageHider.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "MessageHider.h"
#include <windows.h>
#include <shellapi.h>
#include <commdlg.h>
#include <objidl.h>
#include <gdiplus.h> // Inclure GDI+

using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale
Gdiplus::Bitmap* pBitmap = nullptr;             // Bitmap pour l'image


// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                OpenImageFile(HWND hWnd);
void                DrawImage(HDC hdc);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr); // Initialiser GDI+

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MESSAGEHIDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MESSAGEHIDER));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (pBitmap)
    {
        delete pBitmap; // Libérer le bitmap
    }

    Gdiplus::GdiplusShutdown(gdiplusToken); // Fermer GDI+
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MESSAGEHIDER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MESSAGEHIDER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}






void OpenImageFile(HWND hWnd) {
    OPENFILENAME ofn;
    WCHAR szFile[260] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Images (*.bmp;*.jpg;*.jpeg;*.png)\0*.bmp;*.jpg;*.jpeg;*.png\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        delete pBitmap; // Libérer l'ancien bitmap
        pBitmap = Gdiplus::Bitmap::FromFile(ofn.lpstrFile);
        InvalidateRect(hWnd, nullptr, TRUE); // Redessiner la fenêtre
    }
}

HWND hEdit; // Champ de texte
HWND hButton; // Bouton

void DrawImage(HDC hdc, HWND hWnd) {
    if (pBitmap) {
        RECT rect;
        GetClientRect(hWnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top - 50; // Réserver de l'espace pour le champ de texte

        Gdiplus::Graphics graphics(hdc);
        graphics.DrawImage(pBitmap, 0, 0, windowWidth, windowHeight); // Dessiner l'image ajustée
    }
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;

    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Créer le champ de texte
    hEdit = CreateWindowEx(0, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 10, 200, 25, // Position et taille
        hWnd, NULL, hInstance, NULL);

    // Créer le bouton
    hButton = CreateWindow(L"BUTTON", L"Envoyer",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        220, 10, 80, 25, // Position et taille
        hWnd, NULL, hInstance, NULL);

    return TRUE;
}



// WndProc et autres fonctions restent inchangées...

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Analyse les sélections de menu :
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case ID_AIDE_LACHANSONOFFICIELLE:
            ShellExecute(0, 0, L"https://www.youtube.com/watch?v=zBbSH-w6L_8", 0, 0, SW_SHOW);
            break;
        case ID_FICHIER_CRYPTERUNEIMAGE:
            OpenImageFile(hWnd); // Ouvrir une image
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
        DrawImage(hdc, hWnd); // Passer hWnd ici
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

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
