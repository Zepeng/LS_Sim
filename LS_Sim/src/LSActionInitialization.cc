#include "LSActionInitialization.hh"
#include "LSPrimaryGeneratorAction.hh"
#include "LSRunAction.hh"
#include "LSEventAction.hh"
#include "LSTrackingAction.hh"
#include "LSSteppingAction.hh"
#include "LSActionInitializationMessenger.hh"
#include "LSOpticksEventConfigMessenger.hh"
#include <cassert>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSActionInitialization::LSActionInitialization()
 : G4VUserActionInitialization()
{
   //eventAction = new LSEventAction();  
   //theMessenger  = new LSActionInitializationMessenger(this);
	
  LSOpticksEventConfigMessenger* mes = new LSOpticksEventConfigMessenger();
 // assert(mes); 
  //SetOpticksMode(mes->GetOpticksMode());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSActionInitialization::~LSActionInitialization()
{
  //delete theMessenger;
  //delete eventAction; //don't need delete
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*void LSActionInitialization::SetOpticksMode(int mode){

	G4cout<<"LSActionInitialization::SetOpticksMode"<< mode;	
	eventAction->SetOpticksMode(mode);	
}
*/
void LSActionInitialization::Build() const
{
  SetUserAction(new LSPrimaryGeneratorAction);

  LSRunAction* runAction = new LSRunAction();
  SetUserAction(runAction);
  
  LSEventAction* eventAction = new LSEventAction();
  SetUserAction(eventAction);
  SetUserAction(new LSTrackingAction);
  SetUserAction(new LSSteppingAction());
  
  //LSOpticksEventConfigMessenger* mes = LSOpticksEventConfigMessenger::Get();
  //assert(mes); 
  //SetOpticksMode(mes->GetOpticksMode());
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
