//Isaac Mooney, WSU, June 2019

#include <ctime>
#include <iostream>
#include <iomanip>
#include <math.h>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TF1.h>
#include <TH2.h>
#include <TH3.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TRandom.h>

using namespace std;

//self explanatory: takes branches in a tree, "treestr", in file f, and fills histograms with them
void TreetoHist (TFile *f, string treestr, vector<TH1D*> h1Ds, vector<TH2D*> h2Ds) {  
  //initializing the variables that will be filled by the values in the branches later
  double a = -9999;
  vector<double> * b = 0;
  
  //getting the tree and linking the branches with the variables
  TTree *t = (TTree*) f->Get(treestr.c_str());
  t->SetBranchAddress("a", &a);
  t->SetBranchAddress("b", &b);

  cout << ("RUNNING OVER TREE "+treestr+"! Entries: ").c_str() << t->GetEntries() << endl;
  const clock_t begin_time = clock(); //timing - for debugging and for fun
  for (unsigned i = 0; i < t->GetEntries(); ++ i) { //"event" loop
    if (i % 100 == 0 && i != 0) { //can change this to a frequency of your preference (for real data I use 1e5 or 1e6)
      cout << "Still chuggin. On event " << i << endl;
      cout << "Total time passed: " << fixed << setprecision(5) << double(clock() - begin_time) /(double) CLOCKS_PER_SEC << " secs" << endl;
    }
    t->GetEntry(i);
    
    //filling "event" observables
    h1Ds[0]->Fill(a);
    //looping over "tracks" and filling histograms
    for (unsigned j = 0; j < b->size(); ++ j) {
      h1Ds[1]->Fill(b->at(j));
      h2Ds[0]->Fill(a,b->at(j));
    }//! track size loop
    
  }//! event loop
  
  //! needs to be outside the event loop; not sure exactly what it does
  t->ResetBranchAddresses();
  
  return;
}

int main (int argc, const char ** argv) {
  //intro
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //basic argument checking.
  if (argc != 4) {
    cerr << "Should be three arguments: output location, output name, input name. Received "
	 << argc << ". Exiting." << endl;
    exit(1);
  }

  //opening file containing some example trees
  //argv[3] should be the name of the input file
  string fin_name = (string) argv[3];
  TFile *fin = new TFile(fin_name.c_str(),"READ");
  cout << "DEBUG: input file name is " << fin->GetName() << endl;

  //~~~hists~~~
  TH1D* hevt = new TH1D("hevt","hevt",100,-4,4);
  TH1D* htrk = new TH1D("htrk","htrk",100,-4,4);
  TH2D* hevt_v_trk = new TH2D("hevt_v_trk","hevt_v_trk",100,-4,4,100,-4,4);

  //putting them in a vector to more easily shuttle them back and forth in the function. Drawback: have to know their order.
  vector<TH1D*> h1Ds = {hevt,htrk};
  vector<TH2D*> h2Ds = {hevt_v_trk};
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

  //calling analysis function(s)! "t" here is the internal name of the tree in "fin"  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  TreetoHist (fin, "t", h1Ds, h2Ds);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

  //outro
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //creating output file in which to deposit histograms
  //argv[1] should be the desired location of the output file, argv[2] should be the desired name
  TFile *fout = new TFile(((string) argv[1]+(string) argv[2]).c_str(),"RECREATE");
  cout << "DEBUG: output file name is " << fout->GetName() << endl;
  
  //writing hists to file
  for (unsigned i = 0; i < h1Ds.size(); ++ i) {
    h1Ds[i]->Write();
  }
  for (unsigned i = 0; i < h2Ds.size(); ++ i) {
    h2Ds[i]->Write();
  }
  cout << "Wrote to " << fout->GetName() << endl;
  
  //closing file
  fout->Close();
  cout << "Closed " << fout->GetName() << endl;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  return 0;
}
