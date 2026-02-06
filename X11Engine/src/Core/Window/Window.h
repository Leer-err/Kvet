#pragma once

#include "Logger.h"

class SDL_Window;
union SDL_Event;

class Window {
   public:
    static Window& get() {
        static Window window;
        return window;
    }

    void init();

    bool processMessages();

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

   private:
    Window();
    ~Window();

    void processKeyDown(const SDL_Event* event);
    void processKeyUp(const SDL_Event* event);
    void processMouseButtonDown(const SDL_Event* event);
    void processMouseButtonUp(const SDL_Event* event);
    void processMouseMove(const SDL_Event* event);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    int width;
    int height;

    SDL_Window* handle;

    Logger logger;
};