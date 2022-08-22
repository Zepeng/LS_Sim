#include "LSDetectorConstructionMessenger.hh"
#include "LSDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

LSDetectorConstructionMessenger::LSDetectorConstructionMessenger(LSDetectorConstruction* lsDet)
    : 
	m_LSDetector(lsDet)

{

    lsDetDirectory = new G4UIdirectory("/LS/detConstruction");
    lsDetDirectory -> SetGuidance("LS detector construction register.");

    opticksModeCmd = new G4UIcmdWithAnInteger("/LS/detConstruction/opticksMode", this);
    opticksModeCmd -> SetGuidance("Set opticks mode to control optical simulation.");
    opticksModeCmd -> SetParameterName("opticksMode", true);
    opticksModeCmd -> SetDefaultValue(0);
}



LSDetectorConstructionMessenger::~LSDetectorConstructionMessenger()
{
    delete opticksModeCmd;
    delete lsDetDirectory;
}


void LSDetectorConstructionMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {
   /* 
    if (cmd == opticksModeCmd) 
        m_LSDetector -> SetOpticksMode(opticksModeCmd->GetNewIntValue(newValues));
    else
        G4cout << "Error: Unknow Command !!! " << G4endl;*/
}

