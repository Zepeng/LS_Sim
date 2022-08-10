#include "LSPhysicsListMessenger.hh"
#include "LSPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"

LSPhysicsListMessenger::LSPhysicsListMessenger(LSPhysicsList* phyList)
    : LSPhyList(phyList)

{

    phyDirectory = new G4UIdirectory("/LS/phys/");
    phyDirectory -> SetGuidance("Physics List Registeration.");

    opticalCmd = new G4UIcmdWithABool("/LS/phys/enableoptical", this);
    opticalCmd -> SetGuidance("If Enable Optical Simulation.");
    opticalCmd -> SetParameterName("enableoptical", true);
    opticalCmd -> SetDefaultValue(true);

    yieldCmd = new G4UIcmdWithADouble("/LS/phys/yield", this);
    yieldCmd -> SetGuidance("Set scintillation yield ratio.");
    yieldCmd -> SetParameterName("yieldratio", true);
    yieldCmd -> SetDefaultValue(1.0);

}



LSPhysicsListMessenger::~LSPhysicsListMessenger()
{
    delete opticalCmd;
    delete yieldCmd;
    delete phyDirectory;
}


void LSPhysicsListMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {
    
    if (cmd == opticalCmd) 
        LSPhyList -> DoOpticalSim(opticalCmd->GetNewBoolValue(newValues));
    else if (cmd == yieldCmd) 
        LSPhyList -> SetYieldRatio(yieldCmd->GetNewDoubleValue(newValues));

    else
        G4cout << "Error: Unknow Command !!! " << G4endl;
}

