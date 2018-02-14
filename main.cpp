#include "drawer.h"
#include <chrono>
#include <thread>
#include <ctime>
#include <iostream>

int prevInterval = 0;

// Тикнуть
Uint32 Tick(Uint32 interval, void* glass)
{
   // Показать скорость тика, для отладки, временно
   if(interval != prevInterval)
   {
      std::cout << "время тика = ";
      std::cout << interval << " мс." << std::endl;
      prevInterval = interval;
   }

   static_cast<Glass*>(glass)->tick();
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
               case SDLK_DOWN:     glass.TickToBottom(); break;
               case SDLK_LEFT:     glass.TickToLeft();   break;
               case SDLK_RIGHT:    glass.TickToRight();  break;
               case SDLK_PAGEUP:   glass.Turn(false);    break;
               case SDLK_PAGEDOWN: glass.Turn();         break;
            }
         }
      }

      // Рисовать стакан
      drawer.ClearScreen();
      drawer.DrawGlass(glass.getSelf());
      drawer.UpdateScreen();

      // Попытаться ускорить тик
      if(glass.needFasterTick())
      {
         // Запустить новый колбэк, с уменьшенным интервалом
         SDL_RemoveTimer(mainTimerID);
         if(tickInterval >= 200) // 100 мс - пока минимальный интервал
         {
            tickInterval -= glass.getTickDelta();
            mainTimerID = SDL_AddTimer(tickInterval, Tick, &glass);
         }
      }

      // Закончить игру(пока тупо закроется окно)
      if(glass.isExit()) quit = true;

      // Отдохнуть 10мс, чтобы не насиловать машину
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }
   SDL_RemoveTimer(mainTimerID);

   return 0;
}