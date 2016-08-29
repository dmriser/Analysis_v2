#include <iostream>
#include <fstream>

#include "TString.h"
using namespace std;

void Print_Usage();

int main(int argc, char * argv[])
{

  if (argc != 2) { Print_Usage(); return 0; }  

  string name = argv[1];
  string headername = Form("out/%s.h",argv[1]);
  string filename   = Form("out/%s.cxx",argv[1]); 
  
  ofstream hfile(headername.c_str()); 
  if (hfile){
    hfile << Form("#ifndef %s_h",name.c_str()) << endl;
    hfile << Form("#define %s_h",name.c_str()) << endl;
    hfile << endl;
    hfile << endl; 
    hfile << " // Put your includes here " << endl;
    hfile << "#include \"DBins.h\" " << endl;
    hfile << "#include \"DCut.h\" " << endl;
    hfile << "#include \"DEvent.h\" " << endl;
    hfile << "#include \"DSelection.h\" " << endl;
    hfile << "#include \"h22Event.h\" " << endl;
    hfile << "#include \"h22Reader.h\" " << endl; 
    hfile << endl;
    hfile << Form("class %s : public h22Reader {",name.c_str()) << endl;  
    hfile << "    public:" << endl;
    hfile << Form("        %s();",name.c_str()) << endl;
    hfile << Form("        ~%s();",name.c_str()) << endl; 
    hfile << endl;
    hfile << "    void Loop();" << endl;
    hfile << "};" << endl;
    hfile << "#endif" << endl;
  }

  hfile.close(); 

  ofstream cfile(filename.c_str());
  if (cfile)
    {
      cfile << Form("#ifndef %s_cxx",name.c_str()) << endl;
      cfile << Form("#define %s_cxx",name.c_str()) << endl;
      cfile << endl;
      cfile << Form("#include \"%s.h\"",name.c_str()) << endl;
      cfile << endl; 
      cfile << " // Put your includes here " << endl;
      cfile << "#include \"DBins.h\" " << endl;
      cfile << "#include \"DCut.h\" " << endl;
      cfile << "#include \"DEvent.h\" " << endl;
      cfile << "#include \"DSelection.h\" " << endl;
      cfile << "#include \"h22Event.h\" " << endl;
      cfile << "#include \"h22Reader.h\" " << endl; 
      cfile << endl;
      cfile << " // Class Constructor/Destructor " << endl;
      cfile << Form("%s::%s(){ \n }",name.c_str(), name.c_str()) << endl;
      cfile << endl; 
      cfile << Form("%s::~%s(){ \n }",name.c_str(), name.c_str()) << endl;
      cfile << endl;
      cfile << Form("void %s::Loop(){",name.c_str()) << endl;
      cfile << endl;
      cfile << " // Event loop below. " << endl; 
      cfile << "        for(int ievent=0; ievent<GetEntries(); ievent++){" << endl;
      cfile << "                GetEntry(ievent); " << endl;
      cfile << "                DEvent event = h22Event(GetEvent()); " << endl; 
      cfile << "        } " << endl;
      cfile << "}" << endl;
      cfile << "#endif" << endl;
    }
  
  cfile.close(); 

  // Spitting out main! 
  ofstream mainfile("out/main.cxx");
  if (mainfile){
    mainfile << "#include <iostream>" << endl; 
    mainfile << "using namespace std;" << endl;
    mainfile << endl;
    mainfile << Form("#include \"%s.h\"",name.c_str()) << endl;
    mainfile << Form("#include \"%s.cxx\"",name.c_str()) << endl; 
    mainfile << endl;
    mainfile << endl;
    mainfile << "int main(int argc, char * argv[]){" << endl;
    mainfile << endl;
    mainfile << Form("        %s Analyzer;",name.c_str()) << endl;
    mainfile << "        if (argc < 2) { return 0; }" << endl;
    mainfile << endl;
    mainfile << "        for (int ifile=1; ifile<argc; ifile++){ Analyzer.AddFile(argv[ifile]); } " << endl; 
    mainfile << "        Analyzer.Init(); " << endl;
    mainfile << "        Analyzer.Loop(); " << endl; 
    mainfile << "        return 0; " << endl; 
    mainfile << "}" << endl;
  } 
  
  return 0;
}

void Print_Usage()
{
  cout << " Usage: make_analysis AnalysisName " << endl; 
}
