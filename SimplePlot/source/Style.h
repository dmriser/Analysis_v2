#ifndef style_h
#define style_h

#include <iostream>

namespace SimplePlot{

  class Style{
  public:
    Style(); 
    ~Style();
    
  protected:
    int primaryColor, secondaryColor, fontIndex; 
    bool gridx, gridy, fillStatus;
    double alpha; 
    std::string name; 

  public:
    void setPrimaryColor(int color)  { primaryColor   = color; }
    void setSecondaryColor(int color){ secondaryColor = color; }
    void setGridX(bool t){ gridx = t; }
    void setGridY(bool t){ gridy = t; }
    void setFillStatus(bool t){ fillStatus = t; }
    void setName(std::string n){ name = n; }
    void setAlpha(double a){alpha=a;}

    //! Save and Load Styles from files 
    void saveStyle(std::string filename);
    bool loadStyle(std::string filename);

    int getPrimaryColor()  { return primaryColor;   }
    int getSecondaryColor(){ return secondaryColor; }
    int getFontIndex()     { return fontIndex;      }
    bool getGridX()        { return gridx;          }    
    bool getGridY()        { return gridy;          }    
    bool getFillStatus()   { return fillStatus;     }
    double getAlpha()      { return alpha;          }
    std::string getName()  { return name;           }
  };

}


#endif
