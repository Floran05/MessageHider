#include "JournalManager.h"
#include <windows.h>

// Constructeur

JournalManager* JournalManager::Instance = nullptr;
JournalManager::JournalManager() : hJournalWnd(nullptr), hTextBox(nullptr) {
    Instance = this;
}
// Initialiser le JournalManager
void JournalManager::Init(HINSTANCE hInstance, HWND hParent)
{
    // Enregistrer la classe de fen�tre pour le journal
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = JournalProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"JournalClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc))
    {
        MessageBox(nullptr, L"�chec de l'enregistrement de la classe de fen�tre pour le journal.", L"Erreur", MB_ICONERROR);
        return;
    }

    // Cr�ation de la fen�tre du journal
    hJournalWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW, L"JournalClass", L"Journal des Erreurs",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        hParent, nullptr, hInstance, this); // Passer 'this' pour acc�der � l'instance dans JournalProc

    if (!hJournalWnd)
    {
        MessageBox(nullptr, L"�chec de la cr�ation de la fen�tre du journal.", L"Erreur", MB_ICONERROR);
        return;
    }

    // Cr�er la zone de texte pour afficher le journal
    hTextBox = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
        10, 10, 360, 240, hJournalWnd, nullptr, hInstance, nullptr);

    if (!hTextBox)
    {
        MessageBox(nullptr, L"�chec de la cr�ation de la zone de texte du journal.", L"Erreur", MB_ICONERROR);
        return;
    }

    // Initialiser le contenu du journal
    SetWindowText(hTextBox, logContent.c_str());
}

// �crire un message dans le journal
void JournalManager::LogWrite(const std::wstring& message)
{
    // Ajouter le message au journal
    logContent += message + L"\r\n";

    // Mettre � jour le texte de la zone de texte
    if (hTextBox)
    {
        SetWindowText(hTextBox, logContent.c_str());
    }
}

// Proc�dure de fen�tre pour le journal
LRESULT CALLBACK JournalManager::JournalProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    JournalManager* pThis = nullptr;

    if (message == WM_CREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (JournalManager*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else
    {
        pThis = (JournalManager*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (message)
        {
        case WM_CLOSE:
            ShowWindow(hWnd, SW_HIDE); // Masquer la fen�tre au lieu de la d�truire
            return 0;
        default:
            break;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
