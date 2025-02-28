#include "LSSteppingAction.hh"
#include "LSEventAction.hh"
#include "MyAnalysisManager.hh"

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4StepPoint.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4Scintillation.hh"
#include "G4Cerenkov.hh"
#include "LSOpticksEventConfigMessenger.hh"
#include "LSDetectorSD.hh"
#ifdef WITH_G4CXOPTICKS
#  include "U4.hh"
#  include "SEvt.hh"
#  include "G4CXOpticks.hh"
#endif

namespace
{
  G4Mutex opticks_mutex = G4MUTEX_INITIALIZER;
}
LSSteppingAction::LSSteppingAction() {}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
LSSteppingAction::~LSSteppingAction() {}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSSteppingAction::UserSteppingAction(const G4Step* aStep)
{/*
#ifdef WITH_G4CXOPTICKS
  if(LSOpticksEventConfigMessenger::GetInstance()->GetOpticksMode())
  {
    G4int fNumPhotons = 0;  // number of scintillation photons this step
    const G4Track* aTrack;
    G4SteppingManager* fpSteppingManager =
      G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
    G4StepStatus stepStatus = fpSteppingManager->GetfStepStatus();
    if(stepStatus != fAtRestDoItProc)
    {
      G4ProcessVector* procPost = fpSteppingManager->GetfPostStepDoItVector();
      size_t MAXofPostStepLoops = fpSteppingManager->GetMAXofPostStepLoops();
      for(size_t i3 = 0; i3 < MAXofPostStepLoops; i3++)
      {
        if((*procPost)[i3]->GetProcessName() == "Cerenkov")
        {
          aTrack                             = aStep->GetTrack();
          const G4DynamicParticle* aParticle = aTrack->GetDynamicParticle();
          G4double charge                    = aParticle->GetDefinition()->GetPDGCharge();
          const G4Material* aMaterial        = aTrack->GetMaterial();
          G4MaterialPropertiesTable* MPT     = aMaterial->GetMaterialPropertiesTable();
          G4MaterialPropertyVector* Rindex   = MPT->GetProperty(kRINDEX);
          G4Cerenkov* proc                   = (G4Cerenkov*) (*procPost)[i3];
          fNumPhotons                        = proc->GetNumPhotons();
          Photoncounter += fNumPhotons;
          if(fNumPhotons > 0)
          {
            G4double Pmin        = Rindex->Energy(0);
            G4double Pmax        = Rindex->GetMaxEnergy();
            G4double nMax        = Rindex->GetMaxValue();
            G4double beta1       = aStep->GetPreStepPoint()->GetBeta();
            G4double beta2       = aStep->GetPostStepPoint()->GetBeta();
            G4double beta        = (beta1 + beta2) * 0.5;
            G4double BetaInverse = 1. / beta;
            G4double maxCos      = BetaInverse / nMax;
            G4double maxSin2     = (1.0 - maxCos) * (1.0 + maxCos);
            G4double MeanNumberOfPhotons1 =
              proc->GetAverageNumberOfPhotons(charge, beta1, aMaterial, Rindex);
            G4double MeanNumberOfPhotons2 =
              proc->GetAverageNumberOfPhotons(charge, beta2, aMaterial, Rindex);
            U4::CollectGenstep_G4Cerenkov_modified(aTrack, aStep, fNumPhotons, BetaInverse, Pmin,
                                                   Pmax, maxCos, maxSin2, MeanNumberOfPhotons1,
                                                   MeanNumberOfPhotons2);
            GenStepcounter++;
          }
        }
        if((*procPost)[i3]->GetProcessName() == "Scintillation")
        {
          G4Scintillation* proc1 = (G4Scintillation*) (*procPost)[i3];
          fNumPhotons            = proc1->GetNumPhotons();
          Photoncounter += fNumPhotons;
          G4double timeconst = 0.0;
          if(fNumPhotons > 0)
          {
            aTrack                         = aStep->GetTrack();
            const G4Material* aMaterial    = aTrack->GetMaterial();
            G4MaterialPropertiesTable* MPT = aMaterial->GetMaterialPropertiesTable();
            timeconst                      = MPT->GetConstProperty(kSCINTILLATIONTIMECONSTANT1);
            const G4Track* aTrack          = aStep->GetTrack();
            U4::CollectGenstep_DsG4Scintillation_r4695(aTrack, aStep, fNumPhotons, 1, timeconst);
            GenStepcounter++;
          }
        }
      }
      if(Photoncounter > LSOpticksEventConfigMessenger::GetInstance()->GetOpticksMode())//Need to implement GetMax
      {
        G4int inum_photon          = SEvt::GetNumPhotonFromGenstep(0);
        G4int inum_genstep         = SEvt::GetNumGenstepFromGenstep(0);
        G4int num_PhotonCollected  = SEvt::GetNumPhotonCollected(0);
        G4int num_PhotonGenstepMax = SEvt::GetNumPhotonGenstepMax(0);
        G4int num_Hit              = SEvt::GetNumHit(0);
        // SEvt::Save();
	bool fDebug = true;
        if(fDebug)
	{
          std::cout << "-------------------------------------------------------------------"
                    << std::endl;
          std::cout << "SteppingAction: GenStepcounter:           " << GenStepcounter << std::endl;
          std::cout << "SteppingAction: PhotonCounter:            " << Photoncounter << std::endl;
          std::cout << "SteppingAction: GetNumPhotonFromGenstep:  " << inum_photon << std::endl;
          std::cout << "SteppingAction: GetNumGenstepFromGenstep: " << inum_genstep << std::endl;
          std::cout << "SteppingAction: GetNumPhotonCollected:    " << num_PhotonCollected
                    << std::endl;
          std::cout << "SteppingAction: GetNumPhotonGenstepMax:   " << num_PhotonGenstepMax
                    << std::endl;
          std::cout << "SteppingAction: GetNumHit:                " << num_Hit << std::endl;
          std::cout << "-------------------------------------------------------------------"
                    << std::endl;
        }
        SEvt::Check(0);
        cudaDeviceSynchronize();
        //        G4AutoLock lock(&opticks_mutex);
        G4RunManager* rm     = G4RunManager::GetRunManager();
        const G4Event* event = rm->GetCurrentEvent();
        G4int eventid        = event->GetEventID();
        if(inum_photon > 0)
        {
          // if(ConfigurationManager::getInstance()->isEnable_verbose())
          // std::cout << "SteppingAction: Launch Opticks: " << std::endl;
          G4CXOpticks::Get()->simulate(eventid, false);
          cudaDeviceSynchronize();
          unsigned int num_hits = SEvt::GetNumHit(0);
          // std::cout << "SteppingAction: NumHits:  " << num_hits << std::endl;
          if(fDebug)
	  {
            std::cout << "SteppingAction: GetNumPhotonFromGenstep: " << inum_photon << std::endl;
            std::cout << "SteppingAction: GetNumGenstepFromGenstep: " << inum_genstep << std::endl;
            std::cout << "SteppingAction: NumHits:  " << num_hits << std::endl;
          }
          if(num_hits > 0)
          {
            G4HCtable* hctable = G4SDManager::GetSDMpointer()->GetHCtable();
            for(G4int i = 0; i < hctable->entries(); ++i)
            {
	      std::string sdn   = hctable->GetSDname(i);
	      std::size_t found = sdn.find("detectorSD");
          	if(found != std::string::npos)
          	{
                      std::cout << "detectorSD: " << sdn << std::endl;
		      LSDetectorSD* aSD = (LSDetectorSD*) G4SDManager::GetSDMpointer()->FindSensitiveDetector(sdn);
		      aSD->AddOpticksHits();
		}
            }
          }
        }
        LSSteppingAction::ResetPhotoncounter();
        LSSteppingAction::ResetGenStepcounter();
        G4CXOpticks::Get()->reset(eventid);
      }
    }
  }
#endif*/
    MyAnalysisManager::GetInstance()->SteppingAction(aStep);
}
