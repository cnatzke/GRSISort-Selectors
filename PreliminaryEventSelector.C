#define PreliminaryEventSelector_cxx
// The class definition in PreliminaryEventSelector.h has been generated automatically
#include "PreliminaryEventSelector.h"
#include "GValue.h"

bool CoincidenceCondition(TGriffinHit* hit_one, TGriffinHit* hit_two){
   return TMath::Abs(hit_one->GetTimeStamp() - hit_two->GetTimeStamp()) < 30;
}
bool BGCondition(TGriffinHit* hit_one, TGriffinHit* hit_two){
   return (TMath::Abs(hit_one->GetTimeStamp() - hit_two->GetTimeStamp()) >= 30 && TMath::Abs(hit_one->GetTimeStamp() - hit_two->GetTimeStamp()) < 200);
}
bool DetCondition(TGriffinHit* hit_one, TGriffinHit* hit_two){
   return hit_one->GetDetector() != hit_two->GetDetector();
}
bool CryCondition(TGriffinHit* hit_one, TGriffinHit* hit_two){
   return hit_one->GetCrystal() != hit_two->GetCrystal();
}
bool DetEnergyCondition(TGriffinHit* hit_one){
   return hit_one->GetEnergy() > 100; // keV
}

int timeBins = 2100;
double timeLow = -300.;
double timeHigh = 1800.;
int enBins = 4000;
int enFineBins = 8000;
double enLow = 0.;
double enHigh = 4000.;

void PreliminaryEventSelector::CreateHistograms() {
 
   // Gamma Singles
   fH1["gE"] = new TH1D("gE","#gamma Singles", enBins, enLow, enHigh);
   //Addback spectra
   fH1["aE"] = new TH1D("aE","#gamma Addback", enBins, enLow, enHigh);
   // Sum Spectra
   fH1["gS"] = new TH1D("gS", "#gamma Sum", enBins, enLow, enHigh);
   fH1["aS"] = new TH1D("aS", "#gamma Sum Addback", enBins, enLow, enHigh);

   //Timing spectra
   fH1["ggDeltaT"] = new TH1D("ggDeltaT","#gamma-#gamma time difference; #Deltat [10 ns]",600,-300,300);
   fH1["abDeltaT"] = new TH1D("abDeltaT","#gamma-#gamma time difference (addback); #Deltat [10 ns]",600,-300,300);
   fH2["ggSDeltaT"] = new TH2D("ggSDeltaT","#gamma-#gamma time difference (sum); Energy (keV); #Deltat [10 ns]",enBins,enLow,enHigh,300,0,300);

   //2D histograms
   fH2["gEchan"] = new TH2D("gEchan","#gamma energy vs. GRIFFIN channel number", 129, 0., 129., enBins, enLow, enHigh);
   fH2["ggE"] = new TH2D("ggE"," #gamma-#gamma coincidence",4000,0,4000,4000,0,4000);
   fH2["ggE_bg"] = new TH2D("ggE_bg"," #gamma-#gamma coincidence (time random)",4000,0,4000,4000,0,4000);
   fH2["abE"] = new TH2D("abE"," #gamma-#gamma coincidence (addback)",4000,0,4000,4000,0,4000);
   fH2["abE_bg"] = new TH2D("abE_bg"," #gamma-#gamma coincidence (addback, time random)",4000,0,4000,4000,0,4000);
   fH2["ggS"] = new TH2D("ggS"," #gamma-#gamma coincidence (sum)",4000,0,4000,4000,0,4000);

   //3D histograms
//   fHnSparse["ccS"] = new THnSparse("ccS","channel:channel:sumEnergy",64,0.,64,64,0.,64,enBins,enLow,enHigh);


   for(auto it : fH1) {
      GetOutputList()->Add(it.second);
   }
   for(auto it : fH2) {
      GetOutputList()->Add(it.second);
   }
   for(auto it : fHSparse) {
      GetOutputList()->Add(it.second);
   }
}

void PreliminaryEventSelector::FillHistograms() {

   for(auto i = 0; i < fGrif->GetMultiplicity(); ++i) {
      auto grif = fGrif->GetGriffinHit(i);
      fH1["gE"]->Fill(grif->GetEnergy());
      fH2["gEchan"]->Fill(grif->GetArrayNumber(), grif->GetEnergy());

      //SINGLES coinc.
      for(auto j = i+1; j < fGrif->GetMultiplicity(); ++j) {
         auto grifb = fGrif->GetGriffinHit(j);
         fH1["ggDeltaT"]->Fill(grif->GetTimeStamp()-grifb->GetTimeStamp());
         if(CoincidenceCondition(grif, grifb)) {
            fH2["ggE"]->Fill(grif->GetEnergy(),grifb->GetEnergy());
         }
         if(BGCondition(grif, grifb)) {
            fH2["ggE_bg"]->Fill(grif->GetEnergy(),grifb->GetEnergy());
         }
      }
      
      if(fGrif->GetMultiplicity() == 2){
        for(auto j = i+1; j < fGrif->GetMultiplicity(); ++j) {
        auto grifb = fGrif->GetGriffinHit(j);
            if(CoincidenceCondition(grif, grifb) && CryCondition(grif, grifb)) {
                fH1["gS"]->Fill(grif->GetEnergy()+grifb->GetEnergy());
                fH2["ggSDeltaT"]->Fill(grif->GetEnergy()+grifb->GetEnergy(),TMath::Abs(grif->GetTimeStamp() - grifb->GetTimeStamp()));
                fH2["ggS"]->Fill(grif->GetEnergy(),grifb->GetEnergy());
            }
        }
      }
   }

   //ADDBACK coinc.
   for(auto i=0; i < fGrif->GetAddbackMultiplicity(); ++i){
      auto grifAB = fGrif->GetAddbackHit(i);
      fH1["aE"]->Fill(fGrif->GetAddbackHit(i)->GetEnergy());

      for(auto j = i+1; j < fGrif->GetAddbackMultiplicity(); ++j) {
         auto grifABb = fGrif->GetAddbackHit(j);
         fH1["abDeltaT"]->Fill(grifAB->GetTimeStamp()-grifABb->GetTimeStamp());
         if(CoincidenceCondition(grifAB, grifABb)) {
            fH2["abE"]->Fill(grifAB->GetEnergy(),grifABb->GetEnergy());
         }
         if(BGCondition(grifAB, grifABb)) {
            fH2["abE_bg"]->Fill(grifAB->GetEnergy(),grifABb->GetEnergy());
         }
      }
      
      if(fGrif->GetAddbackMultiplicity() == 2){
        for(auto j = i+1; j < fGrif->GetAddbackMultiplicity(); ++j) {
        auto grifABb = fGrif->GetAddbackHit(j);
            if(CoincidenceCondition(grifAB, grifABb) && DetCondition(grifAB, grifABb)) {
                fH1["aS"]->Fill(grifAB->GetEnergy()+grifABb->GetEnergy());
//              std::cout << "Multiplicity is: " << fGrif->GetAddbackMultiplicity() << endl;
//              std::cout << grifAB->GetEnergy() << " "  << grifABb->GetEnergy() << endl;
//              std::cout << "plotted energy is: " << grifAB->GetEnergy()+grifABb->GetEnergy() << endl;
            }
        }
      }
  }
}

