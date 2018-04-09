// Рисователь, абстракция над графической частью SDL2

#ifndef DRAWER_H
#define DRAWER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "model.h"

// Сторона уно, в px
const int unoSize = 30;
// Ширина инфопанели, в px
const int infoPanelWidth = 100;

// Разрешение окна
const int screenWidth = 10 * unoSize + infoPanelWidth;
const int screenHeight = 22 * unoSize;

class Drawer
{
public:
   Drawer() { init(); }
   ~Drawer() { close(); }

   // Отрисовать стакан
   // param fstUno - ссылка на первое уно стакана
   void DrawGlass(Glass& glass);
   // Отрисовать инфопанель  
   void DrawInfoPanel(InfoPanel& infopanel);
   // Очистить экран
   void ClearScreen();
   // Обновить экран
   void UpdateScreen() {SDL_RenderPresent(m_gRenderer);}
private:

   // Инициализировать рисователь 
   bool init();
   // Загрузить медиа-контент
   void loadMedia() {}
   // Убить ресурсы
   void close();
   // Окно в котором будет жить стакан
   SDL_Window* m_gWindow = nullptr;
   // Ссылка на sdl-рисователь
   SDL_Renderer* m_gRenderer = nullptr;
   // Вьюпорт для стакана(вьюпорты нужны для раздельного рендеринга)
   SDL_Rect m_glassViewportRect;
   // Вьюпорт инфопанели
   SDL_Rect m_infoPanelViewportRect;
   // Шрифт для инфопанели
   TTF_Font* m_font = nullptr;
};

#endif // DRAWER_H