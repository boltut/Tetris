// Описатель инфопанели

#ifndef INFOPANEL_H
#define INFOPANEL_H

class Tetramino; // Forward declaration

class InfoPanel
{
public:
   int GetDestroyedRows() const { return m_destroyedRows; }
   void SetDestroyedRows(int rows) { m_destroyedRows += rows; }
   int GetLevel() const { return m_level; }
   void SetLevel() { ++m_level; }
   bool NeedRedraw()
   { 
      if(m_needRedraw)
      {
         m_needRedraw = false;
         return true;
      }
      return m_needRedraw;
   }
   void SetNeedRedraw() { m_needRedraw = true; }
   void SetNextFigure(Tetramino* t) { m_nextFigure = t; }
   Tetramino* GetNextFigure() { return m_nextFigure; }
private:
   int m_level = 0;
   int m_destroyedRows = 0;
   bool m_needRedraw = false;
   Tetramino* m_nextFigure = nullptr;
};

#endif // INFOPANEL_H