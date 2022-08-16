#include "LSActionInitializationMessenger.hh"
#include "LSActionInitialization.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

LSActionInitializationMessenger::LSActionInitializationMessenger(LSActionInitialization* lsAction)
    : m_LSAction(lsAction)

{

    actDirectory = new G4UIdirectory("/LS/action");
    actDirectory -> SetGuidance("ActionInitialization register.");

    opticksModeCmd = new G4UIcmdWithAnInteger("/LS/action/opticksMode", this);
    opticksModeCmd -> SetGuidance("Set opticks mode to control optical simulation.");
    opticksModeCmd -> SetParameterName("opticksMode", true);
    opticksModeCmd -> SetDefaultValue(0);
}



LSActionInitializationMessenger::~LSActionInitializationMessenger()
{
    delete opticksModeCmd;
    delete actDirectory;
}


void LSActionInitializationMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {
    
    if (cmd == opticksModeCmd) 
       m_LSAction -> SetOpticksMode(opticksModeCmd->GetNewIntValue(newValues));
    else
        G4cout << "Error: Unknow Command !!! " << G4endl;
}

