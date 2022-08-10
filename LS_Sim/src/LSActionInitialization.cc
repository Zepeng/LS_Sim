#include "LSActionInitialization.hh"
#include "LSPrimaryGeneratorAction.hh"
#include "LSRunAction.hh"
#include "LSEventAction.hh"
#include "LSTrackingAction.hh"
#include "LSSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSActionInitialization::LSActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSActionInitialization::~LSActionInitialization()
{}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSActionInitialization::Build() const
{
  SetUserAction(new LSPrimaryGeneratorAction);

  LSRunAction* runAction = new LSRunAction();
  SetUserAction(runAction);
  
  LSEventAction* eventAction = new LSEventAction();
  SetUserAction(eventAction);

  SetUserAction(new LSTrackingAction);
  SetUserAction(new LSSteppingAction(eventAction));

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
