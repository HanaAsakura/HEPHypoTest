#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "TStyle.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include "TSystem.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TCanvas.h"
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

void chist(string dataFileName="pseudoData.root",
                  string dataHistName="PseudoData",
                  string bkgFileName="totalbkg.root",
                  string bkgHistName="transverse_mass_lumi",
                  string sigFileName="signal.root",
                  string sigHistName="transverse_mass_lumi",
                  int calculator = 2,
                  int statistic = 2,
                  int nToys=5000,
                  //const char* hypoTestGraphFile = "hypoTestGraph.root",
                  int newHypoTest = 0){

    // Open the ROOT workspace file.
    //TString s_data = "pseudoData.root";
    //TString s_sig = "signal.root";
    //TString s_bkg = "totalbkg.root";
   // cout << "open file " << myInputFileName << endl;
    TFile *_filedata = TFile::Open(dataFileName.c_str());
    TFile *_filesig = TFile::Open(sigFileName.c_str());
    TFile *_filebkg = TFile::Open(bkgFileName.c_str());
    // Access the workspace.
    //cout << "access workspace" << endl;
    TH1 *myData = (TH1*)_filedata->Get(dataHistName.c_str()); 
    TH1 *mySignal = (TH1*)_filesig->Get(sigHistName.c_str());
    TH1 *myBackground = (TH1*)_filebkg->Get(bkgHistName.c_str());

    //myData->SetName("myData");
    //mySignal->SetName("mySignal");
    //myBackground->SetName("myBackground"); 

    //myData->Rebin(n);
    //mySignal->Rebin(n);
    //myBackground->Rebin(n);	
 	

    HistFactory::Measurement measurement("measurement", "measurement");
    //measurement.SetOutputFilePrefix( "./workspaces/test_workspace" );
    measurement.SetOutputFilePrefix( "test" );
    measurement.SetPOI("SigXsecOverSM");	
    measurement.SetLumi(1.0);
    measurement.SetLumiRelErr(0.10);
    measurement.AddConstantParam("Lumi");
    
    HistFactory::Data dataHF ;
    dataHF.SetHisto(myData) ;	
    
    HistFactory::Sample signal("signal") ;
    signal.AddNormFactor("SigXsecOverSM", 1, 0.00001, 30);
    signal.SetHisto(mySignal) ;
    signal.ActivateStatError() ;
    
    HistFactory::Sample BKG("BKG") ;
    BKG.SetHisto(myBackground) ;
    BKG.ActivateStatError() ;
    HistFactory::Channel channel("channel1") ;
    channel.SetData(dataHF) ; // Add the DATA to the channel
    channel.AddSample(signal) ; // Add the SIGNAL to the channel
    channel.AddSample(BKG) ; // Add the BACKGROUND to the channel
    measurement.AddChannel(channel);
    //measurement.CollectHistograms();
    HistFactory::HistoToWorkspaceFactoryFast h2w(measurement) ;
    RooWorkspace* w = h2w.MakeCombinedModel(measurement) ;
    //RooWorkspace *w = HistFactory::MakeModelAndMeasurementFast(measurement);	
    
    w->writeToFile("workspace.root");
    //gROOT->ProcessLine(".q");
    char hypoTestParam[50]; //String that will take the parameters given as imput and execute the test according to these tests
    
    //Loads the macro StandardHypoTestDemo.C so it can be used later on
    gROOT->ProcessLine(".L StandardHypoTestDemo.C");
    
    if(calculator==2 || calculator==3){
        
        //Declare the parameters pass on to StandardHypoTestDemo
        sprintf(hypoTestParam,"StandardHypoTestDemo(\"workspace.root\",\"combined\",\"ModelConfig\",\"\",\"obsData\",%d,%d,%d)",calculator,statistic,newHypoTest);
        
        gROOT->ProcessLine(hypoTestParam);//Asymptotic Calculator
    }
    else if(calculator==0){
        
        //Declare the parameters pass on to StandardHypoTestDemo
        sprintf(hypoTestParam,"StandardHypoTestDemo(\"workspace.root\",\"combined\",\"ModelConfig\",\"\",\"obsData\",0,%d,%d,%d)",statistic,newHypoTest,nToys);

        gROOT->ProcessLine(hypoTestParam);//Frequentist Calculator, Despues del estadistico va el numero de Toys que usa
    }	
	
    
    //TFile myFile("test_histograms.root", "RECREATE");
    //mySignal->Write();
    //myBackground->Write();
    //myData->Write();
    //myFile.Close();		
   
 
}
