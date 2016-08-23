/* Main Sketch */

int main()
{
  // Get Files 
  h22Option opts;
  opts.set(argc, argv);
  
  // Setup Binning 
  DBins xBins(10,0.1,1);
  DBins qqBins(10,1.0,5);
  DBins wBins(10,2.05,5);

  // Setup Event Selection 
  DSelection selector; 
  VirtualityCut * qq_cut = new VirtualityCut();
  WCut * w_cut           = new WCut();
  qq_cut->set_min(1.0);
  w_cut ->set_min(2.05);
  selector.add_cut( qq_cut );
  selector.add_cut( w_cut );
  selector.enable_all();
  
  // Setup Manager 
  DISManager manager;
  manager.add_files(data_files, 0);
  manager.add_files(mc_files, 1);
  manager.set_bins(xBins, qqBins, wBins);
  manager.set_parfile(parfile);
  manager.set_outfile(outfile);
  manager.set_selector(selector);
  manager.init();
  manager.loop(0);
  manager.loop(1);
  manager.draw();
  manager.close();

  return 0;
}
