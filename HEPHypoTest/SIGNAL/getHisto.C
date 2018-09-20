#include "TH1.h"
#include "TF1.h"
#include "TLine.h"
#include "TSpline.h"
#include "RooAbsData.h"
#include "RooDataHist.h"
#include "RooCategory.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooSimultaneous.h"
#include "RooProdPdf.h"
#include "RooNLLVar.h"
#include "RooProfileLL.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooRandom.h"
#include "RooMinuit.h"
#include "TRandom3.h"
#include "RooWorkspace.h"
#include "RooStats/RooStatsUtils.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "TStopwatch.h"
using namespace std;
using namespace RooFit;
using namespace RooStats;

void getHisto(string sigRootName= "normalizedHistos_signal.root",
	      string sigHistName = "transverse_mass_lumi"){

  //TString s_sig = "normalizedHistos_signal.root";
  TFile *_filesig = TFile::Open(sigRootName.c_str());
  TH1 *mySignal = (TH1*)_filesig->Get(sigHistName.c_str());

mySignal->SetTitle("signal"); 


TFile* f_mySignal = new TFile("signal.root","RECREATE") ;

mySignal->Write() ;
f_mySignal->Close() ;

gROOT->ProcessLine(".q");










//gROOT->ProcessLine(".x generatePseudoData.C");

//gROOT->ProcessLine(".x filemerger.C");

//gROOT->ProcessLine(".x chist.C");

//gROOT->ProcessLine(".L StandardHypoTestDemo.C");

//gROOT->ProcessLine("StandardHypoTestDemo( " test2.root ", " combined " , " ModelConfig " , "" , " obsData " ,2,3)");





}
