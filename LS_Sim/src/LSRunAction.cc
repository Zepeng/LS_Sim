#include "LSRunAction.hh"
#include "LSAnalysisManager.hh"
#include "MyAnalysisManager.hh"
#include "MyAnalysisMessenger.hh"
#include "ContinuousSpectrumManager.hh"
#include "ContinuousSpectrumMessenger.hh"
#include "Randomize.hh"
#include <ctime>

LSRunAction::LSRunAction()
: G4UserRunAction(), fSaveRndm(0), fAutoSeed(true)
{
    fRunMessenger = new MyAnalysisMessenger();
    fConSpecMessenger = new ContinuousSpectrumMessenger();
}


LSRunAction::~LSRunAction()
{;}

void LSRunAction::BeginOfRunAction(const G4Run*)
{
    G4cout << "Begin of One Run" << G4endl;
    LSAnalysisManager* analysis = LSAnalysisManager::getInstance();
    analysis->book();

    if (ContinuousSpectrumManager::GetInstance()->GetActivate()) {
        ContinuousSpectrumManager::GetInstance()->LoadContinuousSpectrum();
    }

    MyAnalysisManager::GetInstance()->BeginOfRunAction();

    if (fAutoSeed) {
     // automatic (time-based) random seeds for each run
     G4cout << "*******************" << G4endl;
     G4cout << "*** AUTOSEED ON ***" << G4endl;
     G4cout << "*******************" << G4endl;
     long seeds[2];
     time_t systime = time(NULL);
     seeds[0] = (long) systime;
     seeds[1] = (long) (systime*G4UniformRand());
     G4Random::setTheSeeds(seeds);
     G4Random::showEngineStatus();
  } else {
     G4Random::showEngineStatus();
  }

  if (fSaveRndm > 0)
  {
     std::ostringstream os;
     os<<"beginOfRun_"<<fSaveRndm<<".rndm";
     G4Random::saveEngineStatus(os.str().c_str());
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSRunAction::EndOfRunAction(const G4Run* )
{
    LSAnalysisManager* analysis = LSAnalysisManager::getInstance();
    analysis->finish();

    MyAnalysisManager::GetInstance()->EndOfRunAction();

    if (fSaveRndm == 1) {
     G4Random::showEngineStatus();
     std::ostringstream os;
     os<<"endOfRun_"<<fSaveRndm<<".rndm";
     G4Random::saveEngineStatus(os.str().c_str());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


