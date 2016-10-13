/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                         October 6, 2016                              #
   #                                                                      #
   ######################################################################## */
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

#include "CommonTools.h"
#include "h22Option.h"

#include "common/BaseDISHistograms.h"
#include "common/DIS1DHistograms.h"
#include "common/ModelCrossSectionLoader.h"
#include "common/FaradayCupLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options * theseOpts); 

int main(int argc, char * argv[]){

  // ------------- Physics Options -------------
  int numberOfXBins = 40; 
  int numberOfWBins = 40; 

  double normalizationScale = cm_to_outhouse*(hydrogen_molar_weight*electron_c*1e6)/(5.00*avogadro*hydrogen_density);
  // -------------------------------------------

  // Check for improper usage of this code. 
  if (argc < 3){
    return PrintUsage();
  }

  // Collect command line options, configure ours and compare.
  h22Options * options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  string inputFilename = options->args["INPUT"].args; 
  string outputFilename = options->args["OUT"].args; 
  string binCenteringStatus = options->args["BINCORR"].args; 
  string radCorrStatus = options->args["RADCORR"].args; 

  if (inputFilename != "UNSET"){
    FaradayCupQuickLoader fcup; 
    normalizationScale /= fcup.openFileGetCharge(inputFilename); 
    cout << "Normalization Scale=" << normalizationScale << endl;

    BaseDISHistograms * dataEvents2D = new BaseDISHistograms();
    dataEvents2D->Load(inputFilename.c_str(),"dataEvents");
    
    int xRebinFactor = floor(dataEvents2D->numberOfXBins/numberOfXBins); 
    int wRebinFactor = floor(dataEvents2D->numberOfWBins/numberOfWBins); 
    dataEvents2D->Rebin2D(xRebinFactor, wRebinFactor); 

    BaseDISHistograms * recEventsRad2D = new BaseDISHistograms();
    recEventsRad2D->Load(inputFilename.c_str(),"recEventsRad");
    recEventsRad2D->Rebin2D(xRebinFactor, wRebinFactor); 

    BaseDISHistograms * genEventsRad2D = new BaseDISHistograms();
    genEventsRad2D->Load(inputFilename.c_str(),"genEventsRad");
    genEventsRad2D->Rebin2D(xRebinFactor, wRebinFactor); 

    BaseDISHistograms * recAndGenEventsRad2D = new BaseDISHistograms();
    recAndGenEventsRad2D->Load(inputFilename.c_str(),"recAndGenEventsRad");

    DIS1DHistograms * dataEvents = new DIS1DHistograms();
    dataEvents->Create(dataEvents2D);
    dataEvents->SetErrors();
    dataEvents->Save(outputFilename.c_str(),"recreate");

    DIS1DHistograms * recEventsRad = new DIS1DHistograms();
    recEventsRad->Create(recEventsRad2D);
    recEventsRad->SetErrors();
    recEventsRad->Save(outputFilename.c_str(),"update");

    DIS1DHistograms * genEventsRad = new DIS1DHistograms();
    genEventsRad->Create(genEventsRad2D);
    genEventsRad->SetErrors();
    genEventsRad->Save(outputFilename.c_str(),"update");

    DIS1DHistograms * acceptance = new DIS1DHistograms();
    acceptance->CreateByDivision(recEventsRad,genEventsRad,"acceptance","Acceptance");
    acceptance->Save(outputFilename.c_str(),"update");

    DIS1DHistograms * crossSection = new DIS1DHistograms();
    crossSection->CreateByDivision(dataEvents,acceptance,"crossSection","Cross Section W/ Acceptance");
    crossSection->Scale(normalizationScale);
    crossSection->ScaleByBinWidth();
    crossSection->ScaleAllByNumberBins();
    crossSection->Save(outputFilename.c_str(),"update");

    ModelCrossSectionLoader * modelLoader = new ModelCrossSectionLoader();
    modelLoader->provideBinningTemplate(dataEvents);
    modelLoader->loadCrossSection(1,1,5.498);
    modelLoader->Save(outputFilename.c_str(),"update");

    DIS1DHistograms * modelCrossSection = modelLoader->getCrossSection();
    DIS1DHistograms * modelCrossSectionAverage = modelLoader->getCrossSectionAverage();

    DIS1DHistograms * crossSectionRatio = new DIS1DHistograms();
    crossSectionRatio->CreateByDivision(crossSection,modelCrossSection,"crossSectionRatio","Cross Section Ratio W/ Acceptance");
    crossSectionRatio->Save(outputFilename.c_str(),"update");


    if (binCenteringStatus == "true"){
      DIS1DHistograms * binCenterCorrection = new DIS1DHistograms();
      binCenterCorrection->CreateByDivision(modelCrossSection, modelCrossSectionAverage, "binCenterCorrection", "Bin Centering Correction");
      binCenterCorrection->Save(outputFilename.c_str(),"update");

      DIS1DHistograms * binCorrCS = new DIS1DHistograms();
      binCorrCS->CreateByDivision(crossSection,binCenterCorrection,"binCorrectedCrossSection","Cross Section W/ Bin Centering Corr.");
      binCorrCS->Save(outputFilename.c_str(),"update");
      
      DIS1DHistograms * binCorrCrossSectionRatio = new DIS1DHistograms();
      binCorrCrossSectionRatio->CreateByDivision(binCorrCS,modelCrossSection,"binCorrectedCrossSectionRatio","Cross Section Ratio W/ Bin Centering Correction");
      binCorrCrossSectionRatio->Save(outputFilename.c_str(),"update");
    }

    if (radCorrStatus == "true"){
      BaseDISHistograms * recEventsNoRad2D = new BaseDISHistograms();
      recEventsNoRad2D->Load(inputFilename.c_str(),"recEventsNoRad");
      
      BaseDISHistograms * genEventsNoRad2D = new BaseDISHistograms();
      genEventsNoRad2D->Load(inputFilename.c_str(),"genEventsNoRad");
      
      BaseDISHistograms * recAndGenEventsNoRad2D = new BaseDISHistograms();
      recAndGenEventsNoRad2D->Load(inputFilename.c_str(),"recAndGenEventsNoRad");
    }

  } else {
    return PrintUsage();
  }

  return 0;
}

void configureCommandLineOptions(h22Options * theseOpts){
  
  theseOpts->args["INPUT"].args = "UNSET";
  theseOpts->args["INPUT"].type = 1;
  theseOpts->args["INPUT"].name = "Processed file from fillHistograms";
  
  theseOpts->args["RADCORR"].args = "false";
  theseOpts->args["RADCORR"].type = 1;
  theseOpts->args["RADCORR"].name = "Radiative Corrections status";
  
  theseOpts->args["BINCORR"].args = "false";
  theseOpts->args["BINCORR"].type = 1;
  theseOpts->args["BINCORR"].name = "Bin Centering Corrections status";
  
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    doCrossSection -> Used to fill CS histograms    #" << endl;
  cout << "#     after fillHistograms has been run.             #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   This code requires an input/output flagged       #" << endl;
  cout << "#   -INPUT=filename.root    -OUT=outputFile.root     #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   You may also choose to specify -RADCORR=true     #" << endl;
  cout << "#   if your file contains radiative corrections.     #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   You may also choose to specify -BINCORR=true     #" << endl;
  cout << "#   if your file contains bin centering corrections. #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
