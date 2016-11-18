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
    hfile << "#include \"histos.h\"" << endl;
    hfile << "#include \"histos.cxx\"" << endl;
    hfile << endl; 
    hfile << Form("class %s : public h22Reader {",name.c_str()) << endl;  
    hfile << "    public:" << endl;
    hfile << Form("        %s();",name.c_str()) << endl;
    hfile << Form("        ~%s();",name.c_str()) << endl; 
    hfile << endl; 
    hfile << "\t Histograms histos;" << endl; 
    hfile << endl;
    hfile << "    void Loop();" << endl;
    hfile << "    void ProcessEvent();" << endl;
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
      cfile << "#include \"histos.h\"" << endl;
      cfile << "#include \"histos.cxx\"" << endl;
      cfile << endl; 
      cfile << " // Put your includes here " << endl;
      cfile << "#include \"DCut.h\" " << endl;
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
      cfile << "\t Init(); " << endl; 
      cfile << "\t histos.Init(); " << endl; 
      cfile << "        for(int ievent=0; ievent<GetEntries(); ievent++){" << endl;
      cfile << "                GetEntry(ievent); " << endl;
      cfile << "                ProcessEvent(); "   << endl;  
      cfile << "        } " << endl;
      cfile << "}" << endl;
      cfile << endl;
      cfile << Form("void %s::ProcessEvent(){",name.c_str()) << endl;
      cfile << "\t // Do your stuff here. " << endl;
      cfile << "\t histos.Fill(event);" << endl; 
      cfile << "}" << endl;
      cfile << "#endif" << endl;
    }
  
  cfile.close(); 


  // Doing histograms class 
  ofstream histoheader("out/histos.h");
  if (histoheader){
    histoheader << "#ifndef histo_h" << endl; 
    histoheader << "#define histo_h" << endl; 
    histoheader << endl; 
    histoheader << "#include \"TF1.h\"" << endl; 
    histoheader << "#include \"TGraphErrors.h\"" << endl; 
    histoheader << "#include \"TH1.h\"" << endl; 
    histoheader << "#include \"TH2.h\"" << endl; 
    histoheader << endl; 
    histoheader << "#include \"h22Event.h\"" << endl; 
    histoheader << endl; 
    histoheader << "class Histograms{" << endl; 
    histoheader << "\t public:" << endl; 
    histoheader << "\t Histograms(){}" << endl; 
    histoheader << "\t ~Histograms(){}" << endl; 
    histoheader << endl; 
    histoheader << "\t // Define histograms/graphs here" << endl; 
    histoheader << "\t void Init();" << endl; 
    histoheader << "\t void Fill(h22Event event);" << endl; 
    histoheader << "};"<< endl; 
    histoheader << endl; 
    histoheader << "#endif" << endl; 
    histoheader.close(); 
  }

  ofstream histobody("out/histos.cxx");
  if (histobody){
    histobody << "#ifndef histo_cxx" << endl; 
    histobody << "#define histo_cxx" << endl; 
    histobody << endl; 
    histobody << "#include \"TF1.h\"" << endl; 
    histobody << "#include \"TGraphErrors.h\"" << endl; 
    histobody << "#include \"TH1.h\"" << endl; 
    histobody << "#include \"TH2.h\"" << endl; 
    histobody << endl; 
    histobody << "#include \"h22Event.h\"" << endl; 
    histobody << "#include \"histos.h\"" << endl; 
    histobody << endl; 
    histobody << "\t void Histograms::Init(){ }" << endl; 
    histobody << "\t void Histograms::Fill(h22Event event){ }" << endl; 
    histobody << "#endif" << endl; 
    histobody.close(); 
  }

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
