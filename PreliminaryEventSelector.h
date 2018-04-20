//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 25 13:18:27 2016 by ROOT version 5.34/24
// from TTree FragmentTree/FragmentTree
// found on file: fragment07844_000.root
//////////////////////////////////////////////////////////

#ifndef PreliminaryEventSelector_h
#define PreliminaryEventSelector_h

#include "TChain.h"
#include "TFile.h"

#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"

// Header file for the classes stored in the TTree if any.
#include "TGriffin.h"
#include "TSceptar.h"
#include "TZeroDegree.h"
#include "TLaBr.h"
#include "TTAC.h"
#include "TPaces.h"
#include "TGRSISelector.h"

// Fixed size dimensions of array or collections stored in the TTree if any.

std::vector<int> DefineChannels();

class PreliminaryEventSelector : public TGRSISelector {

 public :
   TGriffin    * fGrif;
   TSceptar    * fScep;
   TZeroDegree * fZDS;
   std::vector<int> fDefineChannels;	//necessary as ancilliary module 9 has some broken channels
   std::map<int, int> fChannelMap;

   PreliminaryEventSelector(TTree * /*tree*/ =0) : TGRSISelector(), fGrif(0), fScep(0), fZDS(0) {
      SetOutputPrefix("PreliminaryEvent");
      fDefineChannels = DefineChannels();
      for(int i = 0; i < static_cast<int>(fDefineChannels.size()); ++i) {
         fChannelMap.insert(std::make_pair(fDefineChannels.at(i), i));
      }
   }
   virtual ~PreliminaryEventSelector() { }
   virtual Int_t   Version() const { return 2; }
   void CreateHistograms();
   void FillHistograms();
   void InitializeBranches(TTree *tree);

   ClassDef(PreliminaryEventSelector,2);
};

#endif

#ifdef PreliminaryEventSelector_cxx
void PreliminaryEventSelector::InitializeBranches(TTree* tree) {
   if (!tree) return;
   tree->SetBranchAddress("TGriffin", &fGrif);
   tree->SetBranchAddress("TZeroDegree", &fZDS);
}

#endif // #ifdef PreliminaryEventSelector_cxx

std::vector<int> DefineChannels()
{
   std::vector<int> result;
   const int nchan = 10;
   int address[nchan]={0x0800,0x0801,0x0802,0x0803,0x0804,0x0805,0x0806,0x0809,0x080A,0x080B};
   for(int i=0;i<int(nchan);i++){
      result.push_back (address[i]);
   }
   return result;
}

