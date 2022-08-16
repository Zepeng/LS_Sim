#include "LSActionInitialization.hh"
#include "LSPrimaryGeneratorAction.hh"
#include "LSRunAction.hh"
#include "LSEventAction.hh"
#include "LSTrackingAction.hh"
#include "LSSteppingAction.hh"
#include "LSActionInitializationMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSActionInitialization::LSActionInitialization()
 : G4VUserActionInitialization()
{
   m_opticksMode = 0;
   theMessenger  = new LSActionInitializationMessenger(this);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSActionInitialization::~LSActionInitialization()
{
  delete theMessenger;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void LSActionInitialization::SetOpticksMode(int mode){
	m_opticksMode = mode;	
}

void LSActionInitialization::Build() const
{
  SetUserAction(new LSPrimaryGeneratorAction);

  LSRunAction* runAction = new LSRunAction();
  SetUserAction(runAction);
  
  LSEventAction* eventAction = new LSEventAction();
  eventAction->SetOpticksMode(m_opticksMode);
  SetUserAction(eventAction);

  SetUserAction(new LSTrackingAction);
  SetUserAction(new LSSteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
