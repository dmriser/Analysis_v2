#include <vector>

vector<TH1D*> loadHistos(TFile *inputFile, string targetTitle){
  
  TList * tableOfContents = inputFile->GetListOfKeys();
  TIter next(tableOfContents);
  vector<TH1D*> foundHistos;

  TRegexp reg(targetTitle);

  while(TObject *objectFromFile = next()){
    TString currentObjectName = objectFromFile->GetName();

    if (currentObjectName.Contains(reg)){
      foundHistos.push_back( (TH1D*) inputFile->Get(currentObjectName) );
      cout << "[loadHistos] Found " << currentObjectName << endl; 
    }
  }

  return foundHistos;
}


