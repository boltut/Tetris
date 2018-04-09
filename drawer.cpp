// Drawer implementation
#include "drawer.h"
#include <string>

// --------------------------------------------------
bool Drawer::init()
{
   bool success = true;

   // Инициализировать библиотеку SDL
   if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
   {
      printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
      success = false;
   }
   else
   {
      // Создать окно
      m_gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   screenWidth, screenHeight, SDL_WINDOW_SHOWN);
      if(m_gWindow == NULL)
      {
         printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
         success = false;
      }
      else
      {
         // Создать рендер
         m_gRenderer = SDL_CreateRenderer(m_gWindow, -1, SDL_RENDERER_ACCELERATED);
         if(m_gRenderer == NULL)
         {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
         }
         else
         {
            TTF_Init();
            m_font = TTF_OpenFont("/usr/share/fonts/sourcecode/SourceCodeVariable-Roman.ttf", 72);
            m_glassViewportRect = {1, 1, 300, screenHeight};
            m_infoPanelViewportRect = {301, 1, 99, screenHeight};
         }
      }
   }
   return success;
}
// --------------------------------------------------
void Drawer::DrawGlass(Glass& glass)
{
   // Натравить рендер на стакан
   SDL_RenderSetViewport(m_gRenderer, &m_glassViewportRect);

    // Нарисовать уно		
   auto drawUno = [this](SDL_Rect& unoRect, UNO_COLOR unoColor)
   {
      switch(unoColor)
      {
      case UNO_COLOR::BLACK:  SDL_SetRenderDrawColor(m_gRenderer, 0, 0, 0, 0); break;
      case UNO_COLOR::CYAN:   SDL_SetRenderDrawColor(m_gRenderer, 0, 255, 255, 0); break;
      case UNO_COLOR::YELLOW: SDL_SetRenderDrawColor(m_gRenderer, 255, 255, 0, 0); break;
      case UNO_COLOR::PURPLE: SDL_SetRenderDrawColor(m_gRenderer, 128, 0, 128, 0); break;
      case UNO_COLOR::GREEN:  SDL_SetRenderDrawColor(m_gRenderer, 0, 128, 0, 0); break;
      case UNO_COLOR::RED:    SDL_SetRenderDrawColor(m_gRenderer, 255, 0, 0, 0); break;
      case UNO_COLOR::BLUE:   SDL_SetRenderDrawColor(m_gRenderer, 0, 0, 255, 0); break;
      case UNO_COLOR::ORANGE: SDL_SetRenderDrawColor(m_gRenderer, 255, 165, 0, 0); break;
      }
      SDL_RenderFillRect(m_gRenderer, &unoRect);

      // Нарисовать окантовку уно
      if(unoColor != UNO_COLOR::BLACK)
      {
         SDL_SetRenderDrawColor(m_gRenderer, 0, 0, 0, 0);
         SDL_RenderDrawRect(m_gRenderer, &unoRect);
      }
   };

   // Собрать стакан из уно
   const Uno* firstUno = glass.getSelf();
   for(int row = 0; row < 22; ++row)
      for(int col = 0; col < 10; ++col)
      {
         SDL_Rect unoRect = {col * unoSize, row * unoSize, unoSize, unoSize};
         drawUno(unoRect, firstUno->getColor());
         ++firstUno;
      }
}
// --------------------------------------------------
void Drawer::DrawInfoPanel(InfoPanel& infopanel)
{
   // Выйти если не нужна перерисовка
   if(!infopanel.NeedRedraw())
      return;

   // Натравить рендер на инфопанель
   SDL_RenderSetViewport(m_gRenderer, &m_infoPanelViewportRect);

   // Затереть предыдущую инфопанель
   SDL_SetRenderDrawColor(m_gRenderer, 0, 0, 0, 1); // Почему-то с первого раза не рисуется чистый черный
   SDL_Rect rect = {0, 0, 100, screenHeight};
   SDL_RenderFillRect(m_gRenderer, &rect);

   // Разделительная линия между стаканом и инфопанелью
   SDL_SetRenderDrawColor(m_gRenderer, 105, 105, 105, 0);
   SDL_RenderDrawLine(m_gRenderer, 0, 0, 0, screenHeight);

   // Текстура с уровнем игры
   SDL_Color white = {255, 255, 255};
   std::string level = "Level:" + std::to_string(infopanel.GetLevel());
   SDL_Surface* surfaceInfoPanel = TTF_RenderText_Solid(m_font, level.c_str(), white);
   SDL_Texture* infoPanelTexture = SDL_CreateTextureFromSurface(m_gRenderer, surfaceInfoPanel);
   SDL_Rect dstRect = {5, 2, 70, 20};
   SDL_RenderCopy(m_gRenderer, infoPanelTexture, nullptr, &dstRect);

   // Освободить ресурсы
   SDL_FreeSurface(surfaceInfoPanel);
   SDL_DestroyTexture(infoPanelTexture);

   // Текстура с уничтоженными линиями
   std::string linesDestroyed = "Destroyed:" + std::to_string(infopanel.GetDestroyedRows());
   surfaceInfoPanel = TTF_RenderText_Solid(m_font, linesDestroyed.c_str(), white);
   infoPanelTexture = SDL_CreateTextureFromSurface(m_gRenderer, surfaceInfoPanel);
   dstRect = {2, 50, 90, 30};
   SDL_RenderCopy(m_gRenderer, infoPanelTexture, nullptr, &dstRect);

   // Освободить ресурсы
   SDL_FreeSurface(surfaceInfoPanel);
   SDL_DestroyTexture(infoPanelTexture);
}
// --------------------------------------------------
void Drawer::ClearScreen()
{
   SDL_SetRenderDrawColor(m_gRenderer, 0, 0, 0, 0);
   SDL_RenderClear(m_gRenderer);
}
// --------------------------------------------------
void Drawer::close()
{
   // Release resources
   SDL_DestroyRenderer(m_gRenderer);
   SDL_DestroyWindow(m_gWindow);
   m_gWindow = nullptr;
   m_gRenderer = nullptr;

   // Quit SDL subsystems
   SDL_Quit();
   TTF_Quit();
}