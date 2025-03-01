#include "LSSteppingAction.hh"
#include "LSEventAction.hh"
#include "MyAnalysisManager.hh"

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4StepPoint.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

LSSteppingAction::LSSteppingAction(LSEventAction* event)
: G4UserSteppingAction()
{
}

LSSteppingAction::~LSSteppingAction()
{;}

void LSSteppingAction::UserSteppingAction(const G4Step* step)
{
    MyAnalysisManager::GetInstance()->SteppingAction(step);
}
