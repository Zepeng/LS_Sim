#include "LSEventAction.hh"
#include "LSAnalysisManager.hh"
#include "MyAnalysisManager.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"

#ifdef WITH_G4CXOPTICKS
#include "PLOG.hh"
#include "G4CXOpticks.hh"
#include <cuda_runtime.h>
#endif

LSEventAction::LSEventAction()
: G4UserEventAction()
{;}

LSEventAction::~LSEventAction()
{;}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void LSEventAction::BeginOfEventAction(const G4Event* evt)
{
    G4cout << "Begin of Event " << evt->GetEventID() << G4endl;
    MyAnalysisManager::GetInstance()->BeginOfEventAction(evt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSEventAction::EndOfEventAction(const G4Event* event)
{

#ifdef WITH_G4CXOPTICKS
    G4CXOpticks* gx = G4CXOpticks::Get();
    LOG(info)<<"gx->simulate()";
    LOG(info)<< gx->desc();
    gx->simulate();
    cudaDeviceSynchronize();
    //gx->save();
#endif
    //
    LSAnalysisManager* analysis = LSAnalysisManager::getInstance();
    G4int evtId = event->GetEventID();
    analysis -> analyseEventID(evtId);

    analysis -> analyseAddNtupleRow();

    MyAnalysisManager::GetInstance()->EndOfEventAction(event);
    G4cout << G4endl;
}
