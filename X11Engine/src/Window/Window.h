#pragma once

#include <Windows.h>

class Window {
   public:
    static Window& get() {
        static Window window;
        return window;
    }

    void init();

    bool processMessages();

    inline HWND getHandle() const { return handle; }
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

   private:
    Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void registerWindowClass();
    RECT createWindowRect(DWORD style);

    void centerCursor();

    HWND handle;
    int width;
    int height;
};