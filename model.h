// Интерфейсы модели рабочей области.
// Модель - двумерный массив 10*22, где каждый элемент - уно.
// "Падение" фигуры, как и любое другое действие в игре происходит
// сначала в модели и уже потом отправится в рисователь, реализованный на SDL2.

#ifndef MODEL_H
#define MODEL_H

#include <vector>

// Типы блоков(уно), из которых состоит всё
enum class UNO_TYPE
{
   VACUUM, // Пустота, в которой падает фигура
   FIGURE, // То, из чего состоит фигура
   BOTTOM  // Фундамент, который образуется после падения фигуры
};

// Цвета уно
enum class UNO_COLOR
{
   BLACK,   // Цвет пустоты
   CYAN,    // I
   YELLOW,  // O
   PURPLE,  // T
   GREEN,   // S
   RED,     // Z
   BLUE,    // J
   ORANGE   // L
};

// Положения, в которых могут находится фигурки
enum class Tetramino_POS
{ 
   T0,
   T90,
   T180,
   T270
};

// Тип вращения фигуры: по/против часовой стрелки
enum class TurningType
{
   CW, // По часовой стрелке
   CCW // Против часовой стрелки
};

// Куда тикнуть
enum class TickType
{
   LEFT,
   RIGHT,
   BOTTOM
};

// Уно, наименьшая неделимая сущность, из которой состоит вся рабочая область
class Uno
{
public:
   Uno() : m_color(UNO_COLOR::BLACK), m_type(UNO_TYPE::VACUUM) {}
   Uno(UNO_COLOR c, UNO_TYPE t) : m_color(c), m_type(t) {}
   UNO_TYPE getType() const { return m_type; }
   UNO_COLOR getColor() const { return m_color; }
   void setType(UNO_TYPE type) { m_type = type; }
private:
   UNO_COLOR m_color;
   UNO_TYPE m_type;
};

// Описатель фигуры.
// Фигура представляет собой двумерный массив(квадрат)
// со стороной равной самому длиному "измерению" тетромино 
class Tetramino
{
public:
   virtual ~Tetramino() {}
   // Вернуть себя в актуальной позе
   virtual Uno* getSelf() = 0;
   // Поменять позу
   void changePos(bool cw = true);
   // Получить размер
   int getSize() const { return m_size; }

protected:
   // Uno vacuum
   Uno o = Uno();

   // Текущая поза
   Tetramino_POS m_currPos = Tetramino_POS::T0;

   // Размерность тетромино (сторона квадрата)
   int m_size = 0;
};

class I : public Tetramino
{
public:
   I() { m_size = 4; }
   Uno* getSelf() override { return &m_tetros[(int)m_currPos][0][0]; }

private:
   Uno l = Uno(UNO_COLOR::CYAN, UNO_TYPE::FIGURE);
   using tetroArr = Uno[4][4];

   // Массив со всеми позами
   tetroArr m_tetros[4] =
   {
      {
         { o, o, o, o },
         { l, l, l, l },
         { o, o, o, o },
         { o, o, o, o }
      },
      {
         { o, o, l, o },
         { o, o, l, o },
         { o, o, l, o },
         { o, o, l, o }
      },
      {
         { o, o, o, o },
         { o, o, o, o },
         { l, l, l, l },
         { o, o, o, o }
      },
      {
         { o, l, o, o },
         { o, l, o, o },
         { o, l, o, o },
         { o, l, o, o }
      } 
   };
};

class O : public Tetramino
{
public:
   O() { m_size = 2; }
   Uno* getSelf() override { return &m_tetro[0][0]; };

private:
   Uno l = Uno(UNO_COLOR::YELLOW, UNO_TYPE::FIGURE);
   Uno m_tetro[2][2] = { {l, l}, { l, l } };
};

class T : public Tetramino
{
public:
   T() { m_size = 3; }
   Uno* getSelf() override { return &m_tetros[(int)m_currPos][0][0]; };

private:
   Uno l = Uno(UNO_COLOR::PURPLE, UNO_TYPE::FIGURE);
   using tetroArr = Uno[3][3];

   // Массив со всеми позами
   tetroArr m_tetros[4] =
   {
      {
         { o, l, o },
         { l, l, l },
         { o, o, o }
      },
      {
         { o, l, o },
         { o, l, l },
         { o, l, o }
      },
      {
         { o, o, o },
         { l, l, l },
         { o, l, o }
      },
      {
         { o, l, o },
         { l, l, o },
         { o, l, o }
      }
   };
};

class S : public Tetramino
{
public:
   S() { m_size = 3; }
   Uno* getSelf() override { return &m_tetros[(int)m_currPos][0][0]; };

private:
   Uno l = Uno(UNO_COLOR::GREEN, UNO_TYPE::FIGURE);
   using tetroArr = Uno[3][3];

   // Массив со всеми позами
   tetroArr m_tetros[4] =
   {
      {
         { o, l, l },
         { l, l, o },
         { o, o, o }
      },
      {
         { o, l, o },
         { o, l, l },
         { o, o, l }
      },
      {
         { o, o, o },
         { o, l, l },
         { l, l, o }
      },
      {
         { l, o, o },
         { l, l, o },
         { o, l, o }
      }
   };
};

class Z : public Tetramino
{
public:
   Z() { m_size = 3; }
   Uno* getSelf() override { return &m_tetros[(int)m_currPos][0][0]; };

private:
   Uno l = Uno(UNO_COLOR::RED, UNO_TYPE::FIGURE);
   using tetroArr = Uno[3][3];

   // Массив со всеми позами
   tetroArr m_tetros[4] =
   {
      {
         { l, l, o },
         { o, l, l },
         { o, o, o }
      },
      {
         { o, o, l },
         { o, l, l },
         { o, l, o }
      },
      {
         { o, o, o },
         { l, l, o },
         { o, l, l }
      },
      {
         { o, l, o },
         { l, l, o },
         { l, o, o }
      }
   };
};

class J : public Tetramino
{
public:
   J() { m_size = 3; }
   Uno* getSelf() override { return &m_tetros[(int)m_currPos][0][0]; };

private:
   Uno l = Uno(UNO_COLOR::BLUE, UNO_TYPE::FIGURE);
   using tetroArr = Uno[3][3];

   // Массив со всеми позами
   tetroArr m_tetros[4] =
   {
      {
         { l, o, o },
         { l, l, l },
         { o, o, o }
      },
      {
         { o, l, l },
         { o, l, o },
         { o, l, o }
      },
      {
         { o, o, o },
         { l, l, l },
         { o, o, l }
      },
      {
         { o, l, o },
         { o, l, o },
         { l, l, o }
      }
   };
};

class L : public Tetramino
{
public:
   L() { m_size = 3; }
   Uno* getSelf() override { return &m_tetros[(int)m_currPos][0][0]; };

private:
   Uno l = Uno(UNO_COLOR::ORANGE, UNO_TYPE::FIGURE);
   using tetroArr = Uno[3][3];

   // Массив со всеми позами
   tetroArr m_tetros[4] =
   {
      {
         { o, o, l },
         { l, l, l },
         { o, o, o }
      },
      {
         { o, l, o },
         { o, l, o },
         { o, l, l }
      },
      {
         { o, o, o },
         { l, l, l },
         { l, o, o }
      },
      {
         { l, l, o },
         { o, l, o },
         { o, l, o }
      }
   };
};
// ====================================
// Рабочая область, где происходит все действо (стакан)
class Glass
{
public:
   Glass();
   ~Glass();
   // Сместить фигуру на одно уно
   void tick(TickType tt);
   // Вернуть себя(нужно для рисователя)
   const Uno* getSelf() { return &m_glass[0][0]; };
   // Тикнуть влево
   void TickToLeft();
   // Тикнуть вправо
   void TickToRight();
   // Тикнуть вниз
   void TickToBottom();
   // Повернуть по часовой стрелке
   void Turn(TurningType tp);
   // Уничтожить заполненные строки
   void DestroyFilledRows();
   // Ускорить тик?
   bool NeedFasterTick();
   // Посмотреть дельту, на которую уменьшаем тик
   int GetTickDelta() const { return m_tickDelta; }
   // Конец игры?
   bool IsExit() const { return m_exit == true; }
 
private:
   // Есть ли место для текущей фигуры в конкретной части стакана?
   bool hasPlace(int row, int col);
   // Удалить падающую фигуру из стакана
   // param bToBottom - закатать в бетон
   void deleteFigure(bool bToBottom = false);
   // Записать фигуру в стакан
   void writeFigure();
   // Посмотреть, заполнена ли строка(готова к сносу)
   bool isRowFilled(int row);
   // Проверить, есть ли куски бетона в строке(строка не пуста)
   bool isRowNotEmpty(int row);
   // Рандомно выбрать фигурку
   void selectRandomFigure();

   // Координаты текущей фигуры
   int m_curRow = -1, m_curCol = 4;
   // Рабочая область(стакан)
   Uno m_glass[22][10];
   // Набор фигур
   std::vector<Tetramino*> m_figures;
   // Текущая фигура(индекс в наборе)
   int m_currFigureIndex = 0;
   // Счетчик уничтоженных строк
   int m_destroyedRowsCount = 0;
   // Дельта, на которую уменьшаем время тика
   const int m_tickDelta = 100;
   // Флаг конца игры
   bool m_exit = false;
};

#endif // MODEL_H
