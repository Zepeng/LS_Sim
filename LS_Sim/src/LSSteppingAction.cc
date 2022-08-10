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
    if(0){
        G4Track* track    = step->GetTrack();
        G4StepPoint *pre  = step->GetPreStepPoint();
        G4StepPoint* post = step->GetPostStepPoint();

        G4int trackID = track->GetTrackID();
        G4int stepNo  = track->GetCurrentStepNumber();

        G4cout << "TrackID = "<< trackID << ", stepNo = " << stepNo
            << "，PreStep (" << pre->GetPosition().x() <<", " << pre->GetPosition().y() << ", " << pre->GetPosition().z() << ") -> " << pre->GetStepStatus() << " vol " << pre->GetPhysicalVolume()->GetName() 
            << "，PostStep (" << post->GetPosition().x() <<", " << post->GetPosition().y() << ", "<< post->GetPosition().z() << ") -> " << post->GetStepStatus() << " vol " << post->GetPhysicalVolume()->GetName()
            << ", edep = " << step->GetTotalEnergyDeposit() << ", stepLength = " << step->GetStepLength()
            << G4endl;
    }

    MyAnalysisManager::GetInstance()->SteppingAction(step);
}
