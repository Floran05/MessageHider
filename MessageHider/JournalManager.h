#pragma once
#include <string>
#include "framework.h"

class JournalManager
{
public:
    JournalManager();
    void Init(HINSTANCE hInstance, HWND hParent);
    void LogWrite(const std::wstring& message);
    void LogWarning(const std::wstring& message);
    void LogError(const std::wstring& message);
    HWND hJournalWnd;
    HWND hTextBox;

    static JournalManager* Instance ;

private:
    static LRESULT CALLBACK JournalProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    std::wstring logContent; 
};
