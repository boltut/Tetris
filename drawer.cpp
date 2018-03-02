// Drawer implementation
#include "drawer.h"

// --------------------------------------------------
bool Drawer::init()
{
   //Initialization flag
   bool success = true;

   //Initialize SDL
   if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
   {
      printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
      success = false;
   }
   else
   {
      //Create window
      m_gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   screenWidth, screenHeight, SDL_WINDOW_SHOWN);
      if ( m_gWindow == NULL )
      {
         printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
         success = false;
      }
      else
      {
         //Create renderer for window
         m_gRenderer = SDL_CreateRenderer(m_gWindow, -1, SDL_RENDERER_ACCELERATED);
         if ( m_gRenderer == NULL )
         {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
         }
         else
         {
            //Initialize renderer color
            SDL_SetRenderDrawColor(m_gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
         }
      }
   }

   return success;
}
// --------------------------------------------------
void Drawer::close()
{
   // Destroy window	
   SDL_DestroyRenderer(m_gRenderer);
   SDL_DestroyWindow(m_gWindow);
   m_gWindow = NULL;
   m_gRenderer = NULL;

   // Quit SDL subsystems
   SDL_Quit();
}
// --------------------------------------------------
void Drawer::DrawGlass(const Uno* firstUno)
{
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

   for(int row = 0; row < 22; ++row)
      for(int col = 0; col < 10; ++col)
      {
         SDL_Rect unoRect = { col * unoSize, row * unoSize, unoSize, unoSize };
         drawUno(unoRect, firstUno->getColor());
         ++firstUno;
      }
}
// --------------------------------------------------
void Drawer::ClearScreen()
{
   SDL_SetRenderDrawColor(m_gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
   SDL_RenderClear(m_gRenderer);
}
// --------------------------------------------------
void Drawer::drawInfoPanel()
{
   // Нарисовать:
   // Уровень игры(считай, скорость)
   // Следующую фигурку
   // Кол-во снесенных линий
}