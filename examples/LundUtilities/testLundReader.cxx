#include <iostream>
using namespace std; 

#include "LundEvent.h"
#include "LundReader.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

int main(int argc, char *argv[]){

  if (argc != 2){
    cout << "Please input file" << endl;
    return 0;
  }

  TH1I *checkMomentum = new TH1I("checkMomentum","",100,0,11);
  TH2I *checkDirection = new TH2I("checkDirection","",100,-1,1,100,-1,1);
  TH1I *checkX = new TH1I("checkX","",100,0,1);
  TH1I *checkY = new TH1I("checkY","",100,0,1);
  TH1I *checkW = new TH1I("checkW","",100,0,8);
  TH1I *checkQQ = new TH1I("checkQQ","",100,0,4);

  string fileName(argv[1]);
  LundReader testReader(fileName);
  PhysicsEventBuilder builder;

  while(testReader.HasEvent()){
    LundEvent event = testReader.GetEvent();
    LundHeader header = event.GetHeader();  

    TLorentzVector electron = event.GetParticle(0).GetTLorentzVector();
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    cout.width(16); cout << electron.X();
    cout.width(16); cout << electron.Y();
    cout.width(16); cout << electron.Z();
    cout.width(16); cout << electron.E() << endl;
    
    checkMomentum->Fill(electron.Z());
    checkDirection->Fill(electron.X(), electron.Y());

    //    checkX->Fill(header.x);
    //    checkY->Fill(header.y);
    //    checkW->Fill(header.w);
    //    checkQQ->Fill(header.qq);
 
    checkX->Fill(physicsEvent.x);
    checkY->Fill(physicsEvent.y);
    checkW->Fill(physicsEvent.w);
    checkQQ->Fill(physicsEvent.qq);
  }

  cout << "Events processed: " << testReader.GetNumberOfEvents() << endl; 

  TCanvas *checkCanvas = new TCanvas("checkCanvas","",1200,800);
  checkCanvas->Divide(3,2);
  checkCanvas->cd(1);
  checkMomentum->Draw();
  checkCanvas->cd(2);
  checkCanvas->SetLogz();
  checkDirection->Draw("colz");
  checkCanvas->cd(3);
  checkX->Draw();
  checkCanvas->cd(4);
  checkY->Draw();
  checkCanvas->cd(5);
  checkW->Draw();
  checkCanvas->cd(6);
  checkQQ->Draw();
  checkCanvas->Print("checkLundEvents.png");

  return 0;
}
