#include "MyAnalysisManager.hh"
#include "MyRootBasedAnalysis.hh"

MyAnalysisManager* MyAnalysisManager::fInstance = 0;

MyAnalysisManager::MyAnalysisManager()
{
    fMyRootBasedAnalysis = new MyRootBasedAnalysis();
    fMyRootBasedAnalysis->Activated();
    
}

MyAnalysisManager::~MyAnalysisManager()
{}

void MyAnalysisManager::BeginOfRunAction()
{
    fMyRootBasedAnalysis->BeginOfRunAction();
}


void MyAnalysisManager::EndOfRunAction()
{
    fMyRootBasedAnalysis->EndOfRunAction();
}


void MyAnalysisManager::BeginOfEventAction(const G4Event* evt)
{
    fMyRootBasedAnalysis->BeginOfEventAction(evt);
}

void MyAnalysisManager::EndOfEventAction(const G4Event* evt)
{
    fMyRootBasedAnalysis->EndOfEventAction(evt);
}


void MyAnalysisManager::PreTrackingAction(const G4Track* aTrack)
{
    fMyRootBasedAnalysis->PreTrackingAction(aTrack);
}

void MyAnalysisManager::PostTrackingAction(const G4Track* aTrack)
{
    fMyRootBasedAnalysis->PostTrackingAction(aTrack);
}


void MyAnalysisManager::SteppingAction(const G4Step* aStep)
{
    fMyRootBasedAnalysis->SteppingAction(aStep);
}



void MyAnalysisManager::SetFileName(G4String val)
{   
    fMyRootBasedAnalysis->SetFileName(val);
}


void MyAnalysisManager::SetBirks1(G4double val)
{
    fMyRootBasedAnalysis->SetBirks1(val);
}


void MyAnalysisManager::SetActivate(G4bool val) {
    if (val)
        fMyRootBasedAnalysis->Activated();
    else
        fMyRootBasedAnalysis->Deactivated();

}


