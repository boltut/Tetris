// Реализация модели
#include "model.h"
#include <iostream>
#include <set>

// =======================================
// Изменить позицию фигуры
void Tetramino::changePos(bool cw)
{
   // Вычислить новую позу
   switch(m_currPos)
   {
      case Tetramino_POS::T0:
      {
         if(cw) m_currPos = Tetramino_POS::T90;
         else   m_currPos = Tetramino_POS::T270;
         break;
      }
      case Tetramino_POS::T90:
      {
         if(cw) m_currPos = Tetramino_POS::T180;
         else   m_currPos = Tetramino_POS::T0;
         break;
      }
      case Tetramino_POS::T180:
      {
         if(cw) m_currPos = Tetramino_POS::T270;
         else   m_currPos = Tetramino_POS::T90;
         break;
      }
      case Tetramino_POS::T270:
      {
         if(cw) m_currPos = Tetramino_POS::T0;
         else   m_currPos = Tetramino_POS::T180;
         break;
      }
   }
}
// =======================================
Glass::Glass()
{
   setlocale(LC_ALL, "Russian");
   // Создать фигурку рандомно
   createRandomFigure();
}
// ---------------------------------------
Glass::~Glass()
{
   if(m_currFigure)
      delete m_currFigure;
}
// ---------------------------------------
void Glass::tick(bool left, bool right)
{
   // Обработать смещение по вертикали
   if(!left && !right)
   {
      // Записать в след. строку если есть место
      if(hasPlace(m_curRow + 1, m_curCol))
      {
         ++m_curRow;
         deleteFigure();
         writeFigure();
      }
      // Места нет
      else 
      {
         // Обработать конец игры
         if(m_curRow == 0)
            m_exit = true;

         deleteFigure(true);
         m_curRow = -1;
         m_curCol = 4;
         createRandomFigure();

         // Уничтожить заполненые строки
         DestroyFilledRows();
      }
   }
   // Обработать смещение по горизонтали
   else
   {
      int horShift = 0;
      if(left) horShift = -1;
      if(right) horShift = 1;

      if(hasPlace(m_curRow, m_curCol + horShift))
      {
         m_curCol += horShift;
         deleteFigure();
         writeFigure();
      }
   }
}
// ---------------------------------------
void Glass::deleteFigure(bool bToBottom)
{
   for(int row = 0; row < 22; ++row)
      for(int column = 0; column < 10; ++column)
         if(m_glass[row][column].getType() == UNO_TYPE::FIGURE)
         {
            if(bToBottom)
               m_glass[row][column].setType(UNO_TYPE::BOTTOM);
            else
               m_glass[row][column] = Uno();
         }
}
// ---------------------------------------
void Glass::print(int tickNo) const
{
   using namespace std;

   for(int row = 0; row < 22; ++row)
   {
      for(int column = 0; column < 10; ++column)
         cout << (int)m_glass[row][column].getType();
      cout << endl;
   }
   cout << "tick " << tickNo << endl;
}
// ---------------------------------------
bool Glass::hasPlace(int row, int col)
{
   Uno* uno = m_currFigure->getSelf();        // Начало текущей фигурки
   int currFigSize = m_currFigure->getSize(); // Размер текущей фигурки

   // Смотреть, можно ли разместить фигуру в данном куске стакана
   for(int i = 0; i < currFigSize; ++i)
      for(int j = 0; j < currFigSize; ++j)
      {
         if((*uno).getType() == UNO_TYPE::FIGURE) // Уно падающей фигурки
         {
            // Проверить что не вылезли за границы стакана
            if(row + i > 21 || col + j > 9 || col + j < 0)
               return false;

            // Если в пределах стакана наткнулись на бетон
            if(m_glass[row + i][col + j].getType() == UNO_TYPE::BOTTOM) // Соотв. уно стакана
               return false;
         }
         ++uno;
      }  
   return true;
}
// ---------------------------------------
void Glass::writeFigure()
{
   Uno* uno = m_currFigure->getSelf();        // Начало текущей фигурки
   int currFigSize = m_currFigure->getSize(); // Размер текущей фигурки

   for(int i = 0; i < currFigSize; ++i)
      for(int j = 0; j < currFigSize; ++j)
      {
         if((*uno).getType() == UNO_TYPE::FIGURE)
            m_glass[m_curRow + i][m_curCol + j] = *uno;
         ++uno;
      }
}
// ---------------------------------------
bool Glass::isRowFilled(int row)
{
   for(int i = 0; i < 10; ++i)
      if(m_glass[row][i].getType() != UNO_TYPE::BOTTOM)
         return false;
   return true;
}
// ---------------------------------------
bool Glass::isRowNotEmpty(int row)
{
   for(int i = 0; i < 10; ++i)
      if(m_glass[row][i].getType() == UNO_TYPE::BOTTOM)
         return true;
   return false;
}
// ---------------------------------------
void Glass::createRandomFigure()
{
   if(m_currFigure)
   {
      delete m_currFigure;
      m_currFigure = nullptr;
   }

   switch(std::rand() % 7)
   {
   case 0: m_currFigure = new I; break;
   case 1: m_currFigure = new O; break;
   case 2: m_currFigure = new T; break;
   case 3: m_currFigure = new S; break;
   case 4: m_currFigure = new Z; break;
   case 5: m_currFigure = new J; break;
   case 6: m_currFigure = new L; break;
   }
}
// ---------------------------------------
void Glass::TickToLeft()
{
   tick(true, false);
}
// ---------------------------------------
void Glass::TickToRight()
{
   tick(false, true);
}
// ---------------------------------------
void Glass::TickToBottom()
{
   tick();
}
// ---------------------------------------
void Glass::Turn(bool cw)
{
   m_currFigure->changePos(cw);

   // Перерисовать фигуру
   if(hasPlace(m_curRow, m_curCol))
   {
      deleteFigure();
      writeFigure();
   }
   else
      m_currFigure->changePos(!cw);
}
// ---------------------------------------
void Glass::DestroyFilledRows()
{
   std::set<int> filledRows;

   // Сложить в кучу номера строк, подлежащих уничтожению
   for(int row = 0; row < 22; ++row)
      if(isRowFilled(row))
         filledRows.insert(row);
   
   // Запомнить кол-во уничтоженных строк
   m_destroyedRowsCount += (int)filledRows.size();

   // Пройтись по заполненным строкам
   for(auto it : filledRows)
   {
      // Пройтись по строкам стакана снизу вверх
      for(int row = it; row > 0; --row)
      {
         int prevRow = row - 1;
         if(prevRow < 0) break;

         for(int col = 0; col < 10; ++col)
         {
            // Заменить сносимую строку предыдущей
            m_glass[row][col] = m_glass[prevRow][col];

            // Предыдущую - затереть, если не пуста
            if(isRowNotEmpty(prevRow))
               m_glass[prevRow][col] = Uno();
         }
      }
   }
}      
// ---------------------------------------
bool Glass::needFasterTick()
{
   // Если снесли пять строк - ускорить тик(уменьшить время на дельту)
   if(!(m_destroyedRowsCount < 5))
   {
      m_destroyedRowsCount -= 5;
      return true;
   }
   return false;
}
// ---------------------------------------

