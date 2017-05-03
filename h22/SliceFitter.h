#ifndef SLICEFITTER_H
#define SLICEFITTER_H

#include <vector>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"

class SliceFitter {
public:
    SliceFitter(int nSlices, double min, double max);

    int GetNumberSlices() const;
    void SetNumberSlices(int value);

    double GetXMin() const;
    void SetXMin(double value);

    double GetXMax() const;
    void SetXMax(double value);

    void Fit(TH2D *histo, std::string title);
    void SetExpectedMean(std::string meanFunction);

    TGraphErrors GetGraphMu(std::string title);
    TGraphErrors GetGraphSigma(std::string title);

    TF1 GetFitToMu(std::string fitFunction, std::string title); 
    TF1 GetFitToSigma(std::string fitFunction, std::string title); 

    TF1 GetFitToMu(std::string fitFunction, std::string title, double min, double max); 
    TF1 GetFitToSigma(std::string fitFunction, std::string title, double min, double max); 

    double GetUpperTolerance() const;
    void SetUpperTolerance(double value);

    double GetLowerTolerance() const;
    void SetLowerTolerance(double value);

    std::vector<TF1> GetFits() const;
    std::vector<TH1D> GetSlices() const;

protected:
    int fNumberSlices;
    double fxMin, fxMax;

    /** Describes the upper and lower bounds on the expected mean to fit between.
    The fit will be setup fit->SetRange(fLowerTolerance*fExpectedMean->Eval(x),
                                        fUpperTolerance*fExpectedMean->Eval(x)) */
    double fUpperTolerance, fLowerTolerance;

    TH2D                *fSourceHisto;
    std::vector<TH1D>    fSlices;
    std::vector<TF1>     fFits;

    TF1                *fExpectedMean;

    void SetupDefaultExpectedMean();
};

#endif // SLICEFITTER_H
