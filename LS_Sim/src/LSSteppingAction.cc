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
{
    MyAnalysisManager::GetInstance()->SteppingAction(aStep);
}
