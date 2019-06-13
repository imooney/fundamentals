//Isaac Mooney, WSU, June 2019

#include <ctime>
#include <iostream>
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

//this file (not itself part of the fundamentals) just produces two root output files (so hadding must be done at some point, to mimic a real analysis) with a few histograms and tree branches. The output is then used later by other files.
//can be compiled in an interactive root session by changing int main() to void produce_file_for_tests() and changing return 0; to return; 
int main () {
  
  //used to GetRandom from TRandom::Uniform() later
  TRandom *r = new TRandom();
    
  //functions from which to pull random variates to fill histograms, trees
  TF1 * fsin = new TF1("fsin","abs(sin(x))",-M_PI,M_PI);
  TF1 * fline = new TF1("fline","abs(x)",-3,3);
  
  //local, corresponding to branches (to be defined) of same name
  double a1, a2; vector<double> b1, b2;

  //creating file1 and assigning a tree to it
  
  TFile *fout1 = new TFile("../out/file1.root","RECREATE");
  cout << "created file1.root" << endl;
  
  TTree *t1 = new TTree("t","t");
  t1->Branch("a",&a1);
  t1->Branch("b",&b1);
  
  //creating histograms and filling them with random variates from simple functions, defined above ("gaus" is "pre-"defined)
  TH1D * hgaus1 = new TH1D("hgaus","",100,-3,3);
  hgaus1->FillRandom("gaus",999);
 
  TH1D * hsin1 = new TH1D("hsin","",100,-M_PI,M_PI);
  hsin1->FillRandom("fsin",999);
  
  //creating file2 and assigning a tree to it
  
  TFile *fout2 = new TFile("../out/file2.root","RECREATE");
  cout << "created file2.root" << endl;

  TTree *t2 = new TTree("t","t");
  t2->Branch("a",&a2);
  t2->Branch("b",&b2);
  
  //creating histograms and filling them with random variates from simple functions, defined above ("gaus" is "pre-"defined)
  TH1D * hgaus2 = new TH1D("hgaus","",100,-3,3);
  hgaus2->FillRandom("gaus",1001);
  
  TH1D * hsin2 = new TH1D("hsin","",100,-M_PI,M_PI);
  hsin2->FillRandom("fsin",1001);

  //populating branches and filling trees
  const int nEvents = 1000; const int nTracks = 5;
  for (int i = 0; i < nEvents; ++ i) {
    //clearing values so we don't write the same thing multiple times
    a1 = -9999; a2 = -9999;
    b1.clear(); b2.clear();
    
    a1 = fsin->GetRandom(); a2 = fsin->GetRandom();
    const int nTracks_in_event = r->Uniform(nTracks);//some random number of "tracks" from 0 to nTracks = 5 currently. Then shift it to 1 to 6 so we don't have events with 0 tracks.
    for (int j = 1; j <= (nTracks_in_event + 1); ++ j) {
      b1.push_back(fline->GetRandom()); b2.push_back(fline->GetRandom());
    }
    t1->Fill(); t2->Fill(); //fill the trees
  }

  //writing and closing

  fout1->cd();
  hgaus1->Write(); hsin1->Write(); fsin->Write(); t1->Write();
  cout << "wrote to file1.root" << endl;
  fout2->cd();
  hgaus2->Write(); hsin2->Write(); fsin->Write(); t2->Write();
  cout << "wrote to file2.root" << endl;
  
  fout1->Close(); fout2->Close();
  cout << "closed file1 and file2" << endl;

  return 0;
}



