void plot_tofmass_systematics(std::string fileList){

  std::ifstream list; 
  list.open(fileList.c_str());

  std::string currentFilename; 
  while(get_line(list, currentFilename)){
    std::cout << currentFilename << std::endl; 
  }
  
}
