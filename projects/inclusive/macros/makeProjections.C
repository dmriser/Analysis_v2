{
  // ------------------------------------
  //   User Parameters
  // ------------------------------------
  TFile *inputFile        = TFile::Open("../data.root"); 
  const int numberWBins   = 50;
  const int numberQQBins  = 10;
  const int numberPhiBins = 5;

  // ------------------------------------
  //   The Rest 
  // ------------------------------------
  THnSparseF *events = (THnSparseF*) inputFile->Get("dataEvents_events"); 

  int numberWBinsInSparse   = events->GetAxis(3)->GetNbins();
  int numberQQBinsInSparse  = events->GetAxis(2)->GetNbins();  
  int numberPhiBinsInSparse = events->GetAxis(1)->GetNbins();
  int rebinFactor[4]        = {1, (int) numberPhiBinsInSparse/numberPhiBins,  (int) numberQQBinsInSparse/numberQQBins,  (int) numberWBinsInSparse/numberWBins};
  events->Rebin(rebinFactor);

  for(int sector=1; sector<=events->GetAxis(0)->GetNbins(); sector++){
    events->GetAxis(0)->SetRange(sector,sector); 
    for(int phiBin=1; phiBin<=events->GetAxis(0)->GetNbins(); phiBin++){
      events->GetAxis(1)->SetRange(phiBin,phiBin); 
      for(int qqBin=1; qqBin<=events->GetAxis(2)->GetNbins(); qqBin++){
	events->GetAxis(2)->SetRange(qqBin,qqBin); 
	TH1D *proj  = events->Projection(3);
	string name = Form("data_w_sector%d_phi%d_qq%d",sector,phiBin,qqBin);
	proj->SetName(name.c_str());
	proj->SetTitle(name.c_str());

	cout << " Creating " << name << " with Entries = " << proj->GetEntries() << endl; 
      }
    }
  }

  events->GetAxis(0)->SetRange(1, events->GetAxis(0)->GetNbins());
  events->GetAxis(1)->SetRange(1, events->GetAxis(1)->GetNbins());
  events->GetAxis(2)->SetRange(1, events->GetAxis(2)->GetNbins());
  events->GetAxis(3)->SetRange(1, events->GetAxis(3)->GetNbins());

  for(int sector=1; sector<=events->GetAxis(0)->GetNbins(); sector++){
    events->GetAxis(0)->SetRange(sector,sector); 
    for(int phiBin=1; phiBin<=events->GetAxis(0)->GetNbins(); phiBin++){
      events->GetAxis(1)->SetRange(phiBin,phiBin); 
      for(int wBin=1; wBin<=events->GetAxis(3)->GetNbins(); wBin++){
	events->GetAxis(3)->SetRange(wBin,wBin); 
	TH1D *proj  = events->Projection(2);
	string name = Form("data_qq_sector%d_phi%d_w%d",sector,phiBin,wBin);
	proj->SetName(name.c_str());
	proj->SetTitle(name.c_str());

	cout << " Creating " << name << " with Entries = " << proj->GetEntries() << endl; 
      }
    }
  }

  events->GetAxis(0)->SetRange(1, events->GetAxis(0)->GetNbins());
  events->GetAxis(1)->SetRange(1, events->GetAxis(1)->GetNbins());
  events->GetAxis(2)->SetRange(1, events->GetAxis(2)->GetNbins());
  events->GetAxis(3)->SetRange(1, events->GetAxis(3)->GetNbins());

  for(int sector=1; sector<=events->GetAxis(0)->GetNbins(); sector++){
    events->GetAxis(0)->SetRange(sector,sector); 
    for(int wBin=1; wBin<=events->GetAxis(0)->GetNbins(); wBin++){
      events->GetAxis(3)->SetRange(wBin,wBin); 
      for(int qqBin=1; qqBin<=events->GetAxis(2)->GetNbins(); qqBin++){
	events->GetAxis(2)->SetRange(qqBin,qqBin); 
	TH1D *proj  = events->Projection(1);
	string name = Form("data_phi_sector%d_w%d_qq%d",sector,wBin,qqBin);
	proj->SetName(name.c_str());
	proj->SetTitle(name.c_str());

	cout << " Creating " << name << " with Entries = " << proj->GetEntries() << endl; 
      }
    }
  }
  
  // Launch for user to view slices. 
  TBrowser *b = new TBrowser();
}
