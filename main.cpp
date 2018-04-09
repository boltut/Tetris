#include "drawer.h"
#include <chrono>
#include <thread>

int prevInterval = 0;

// Тикнуть
Uint32 Tick(Uint32 interval, void* glass)
{
   // Показать время тика, для отладки, временно
   if(interval != prevInterval)
   {
      std::cout << "tick time = ";
      std::cout << interval << " ms." << std::endl;
      prevInterval = interval;
   }

   static_cast<Glass*>(glass)->tick(TickType::BOTTOM);
   return interval;
}

int main(int argc, char* args[])
{
   // Интервал тика
   int tickInterval = 800;

   // Обеспечить новую последовательность псевдо-случайных чисел
   std::srand((unsigned)std::time(0));

   Glass glass;
   Drawer drawer;

   // Надо ли выйти
   bool quit = false;

   // Обработчик событий
   SDL_Event e;

   // Тикающий колбэк
   SDL_TimerID mainTimerID = SDL_AddTimer(800, Tick, &glass);

   // Нарисовать инфопанель первый раз
   glass.m_infopanel.SetNeedRedraw();

   // Молотить пока не жмякнем на выход
   while(!quit)
   {
      // Обрабатывать очередь сообщений
      while(SDL_PollEvent(&e) != 0)
      {
         if(e.type == SDL_QUIT)
            quit = true;
         else if(e.type == SDL_KEYDOWN)
         {
            switch(e.key.keysym.sym)
            {
               case SDLK_DOWN:     glass.TickToBottom();         break;
               case SDLK_LEFT:     glass.TickToLeft();           break;
               case SDLK_RIGHT:    glass.TickToRight();          break;
               case SDLK_PAGEUP:   glass.Turn(TurningType::CCW); break;
               case SDLK_PAGEDOWN: glass.Turn(TurningType::CW);  break;
            }
         }
      }

      drawer.DrawGlass(glass); // Рисовать стакан
      drawer.DrawInfoPanel(glass.m_infopanel); // Рисовать инфопанель
      drawer.UpdateScreen();

      // Попытаться ускорить тик
      if(glass.NeedNextLevel())
      {
         // Запустить новый колбэк, с уменьшенным интервалом
         SDL_RemoveTimer(mainTimerID);
         if(tickInterval >= 200) // 100 мс - пока минимальный интервал
         {
            tickInterval -= glass.GetTickDelta();
            mainTimerID = SDL_AddTimer(tickInterval, Tick, &glass);
            std::cout << "new timer!!!" << std::endl;
         }
      }

      // Закончить игру(пока тупо закроется окно)
      if(glass.IsExit()) quit = true;

      // Отдохнуть 10мс, чтобы не насиловать машину
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }
   SDL_RemoveTimer(mainTimerID);

   return 0;
}