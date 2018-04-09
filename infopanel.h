// Описатель инфопанели

#ifndef INFOPANEL_H
#define INFOPANEL_H

enum class FigureType { I, O, T, S, Z, J, L };

class InfoPanel
{
public:
   int GetDestroyedRows() const { return m_destroyedRows; }
   void SetDestroyedRows(int rows) { m_destroyedRows += rows; }
   int GetLevel() const { return m_level; }
   void SetLevel() { m_level += 1; }
   FigureType GetNextFigure() const { m_nextFigure; }
   void SetNextFigure(FigureType ft) { m_nextFigure =ft; }
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
private:
   int m_level = 0;
   int m_destroyedRows = 0;
   FigureType m_nextFigure;
   bool m_needRedraw = false;
};

#endif // INFOPANEL_H