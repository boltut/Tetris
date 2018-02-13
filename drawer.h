﻿// Интерфейс Рисователя, абстракция над SDL2

#ifndef DRAWER_H
#define DRAWER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>
#include "model.h"

// Сторона уно, в px
const int unoSize = 30;

// Разрешение окна
const int screenWidth = 10 * unoSize;
const int screenHeight = 22 * unoSize;

class Drawer
{
public:
   Drawer() { init(); }
   ~Drawer() { close(); }

   // Отрисовать стакан
   // param fstUno - ссылка на первое уно стакана
   void DrawGlass(const Uno* firstUno);

   // Очистить экран
   void ClearScreen();

   // Обновить экран
   void UpdateScreen() { SDL_RenderPresent(m_gRenderer); }
private:

   // Окно в котором будет жить стакан
   SDL_Window* m_gWindow = NULL;
   
   // Ссылка на sdl-рисователь
   SDL_Renderer* m_gRenderer = NULL;

   // Инициализировать рисователь 
   bool init();

   // Загрузить медиа-контент
   void loadMedia() {}

   // Убить ресурсы
   void close();
};

#endif