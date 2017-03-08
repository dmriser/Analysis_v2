#ifndef style_h
#define style_h

#include <iostream>

namespace SimplePlot{

  class Style{
  public:
    Style(); 
    ~Style();
    
  protected:
    int primaryColor, secondaryColor, fontIndex, backgroundColor, markerStyle, lineStyle, lineWidth, markerSize; 
    bool gridx, gridy, fillStatus, lineStatus, markerStatus;
    double alpha; 
    std::string name; 

  public:
    void setPrimaryColor(int color)  { primaryColor   = color; }
    void setSecondaryColor(int color){ secondaryColor = color; }
    void setGridX(bool t){ gridx = t; }
    void setGridY(bool t){ gridy = t; }
    void setFillStatus(bool t){ fillStatus = t; }
    void setName(std::string n){ name = n; }
    void setAlpha(double a){ alpha=a; }
    void setLineStyle(int l){ lineStyle=l; }
    void setLineWidth(int l){ lineWidth=l; }
    void setMarkerStyle(int l){ markerStyle=l; }
    void setMarkerSize(int l){ markerSize=l; }
    void setLineStatus(bool s){ lineStatus=s; }
    void setMarkerStatus(bool s){ markerStatus=s; }

    //! Save and Load Styles from files 
    void saveStyle(std::string filename);
    bool loadStyle(std::string filename);

    int getPrimaryColor()   { return primaryColor;    }
    int getSecondaryColor() { return secondaryColor;  }
    int getFontIndex()      { return fontIndex;       }
    int getBackgroundColor(){ return backgroundColor; }
    int getMarkerStyle()    { return markerStyle;     }
    int getMarkerSize()     { return markerSize;      }
    int getLineStyle()      { return lineStyle;       }
    int getLineWidth()      { return lineWidth;       }
    bool getGridX()         { return gridx;           }    
    bool getGridY()         { return gridy;           }    
    bool getFillStatus()    { return fillStatus;      }
    bool getMarkerStatus()  { return markerStatus;    }
    bool getLineStatus()    { return lineStatus;      }
    double getAlpha()       { return alpha;           }
    std::string getName()   { return name;            }
  };

}


#endif
