// Реализация модели
#include "model.h"
#include <cstdlib>
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
   m_figures = { new I, new O, new T, new S, new Z, new J, new L };

   // Выбрать фигурку рандомно
   selectRandomFigure();
}
// ---------------------------------------
Glass::~Glass()
{
   for(auto figure : m_figures)
      delete figure;
}
// ---------------------------------------
void Glass::tick(TickType tt)
{
   // Обработать смещение по вертикали
   if(tt == TickType::BOTTOM)
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
         m_figures[m_currFigureIndex]->resetPos(); // Сбросить позицию в начальную
         selectRandomFigure();

         // Уничтожить заполненые строки
         DestroyFilledRows();

         // Обновить инфопанель
         m_infopanel.SetNeedRedraw();
      }
   }
   // Обработать смещение по горизонтали
   else
   {
      int horShift = 0;
      if(tt == TickType::LEFT) horShift = -1;
      if(tt == TickType::RIGHT) horShift = 1;

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
bool Glass::hasPlace(int row, int col)
{
   Uno* uno = m_figures[m_currFigureIndex]->getSelf();        // Начало текущей фигурки
   int currFigSize = m_figures[m_currFigureIndex]->getSize(); // Размер текущей фигурки

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
   Uno* uno = m_figures[m_currFigureIndex]->getSelf();        // Начало текущей фигурки
   int currFigSize = m_figures[m_currFigureIndex]->getSize(); // Размер текущей фигурки

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
void Glass::selectRandomFigure(bool firstTime)
{
   if(firstTime) // Первый запуск
      m_currFigureIndex = std::rand() % 7;
   else
      m_currFigureIndex = m_nextFigureIndex;
   
   m_nextFigureIndex = static_cast<int>(std::rand() % 7);
   m_infopanel.SetNextFigure(m_figures[m_nextFigureIndex]);
}
// ---------------------------------------
void Glass::TickToLeft()
{
   tick(TickType::LEFT);
}
// ---------------------------------------
void Glass::TickToRight()
{
   tick(TickType::RIGHT);
}
// ---------------------------------------
void Glass::TickToBottom()
{
   tick(TickType::BOTTOM);
}
// ---------------------------------------
void Glass::Turn(TurningType tp)
{
   bool cw = (tp == TurningType::CW) ? true : false;
   m_figures[m_currFigureIndex]->changePos(cw);

   // Перерисовать фигуру
   if(hasPlace(m_curRow, m_curCol))
   {
      deleteFigure();
      writeFigure();
   }
   else
      m_figures[m_currFigureIndex]->changePos(!cw);
}
// ---------------------------------------
void Glass::DestroyFilledRows()
{
   // Сколько нужно снести строк для одного уровня
   const int oneLevelRowsCount = 5;

   // Сложить в кучу номера строк, подлежащих уничтожению
   std::set<int> filledRows;
   for(int row = 0; row < 22; ++row)
      if(isRowFilled(row))
         filledRows.insert(row);

   // Количество заполненных строк, образованных за падение одной фигуры
   int oneTimefilledRowsCount = (int)filledRows.size();
   
   if(oneTimefilledRowsCount > 0) // Если что-то снесли
   {
      // Проверить, надо ли повысить уровень
      if((m_destroyedRowsCount % oneLevelRowsCount + oneTimefilledRowsCount)
         >= oneLevelRowsCount)
      {
         m_needNextLevel = true;
         m_infopanel.SetLevel();
      }
      // Запомнить кол-во уничтоженных строк
      m_destroyedRowsCount += oneTimefilledRowsCount;
      m_infopanel.SetDestroyedRows(oneTimefilledRowsCount);
   }

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
bool Glass::NeedNextLevel()
{
   if(m_needNextLevel)
   {
      m_needNextLevel = false;
      return true;
   }
   return m_needNextLevel;
}