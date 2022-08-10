#include "LSDetectorSD.hh"
#include "LSDetectorSDMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"

LSDetectorSDMessenger::LSDetectorSDMessenger(LSDetectorSD* sd)
    : LSDetSD(sd)
{

    sdDirectory = new G4UIdirectory("/LS/sd/");
    sdDirectory -> SetGuidance("Sensitive Detector Control Commands.");

    effCmd = new G4UIcmdWithADouble("/LS/sd/efficiency", this);
    effCmd -> SetGuidance("Set efficiency for the sensitive detector.");
    effCmd -> SetParameterName("PDE", true);
    effCmd -> SetDefaultValue(0.18);
}


LSDetectorSDMessenger::~LSDetectorSDMessenger()
{
    delete effCmd;
    delete sdDirectory;
}


void LSDetectorSDMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {

    if (cmd == effCmd)
        LSDetSD -> SetEfficiency(effCmd->GetNewDoubleValue(newValues));

    else
        G4cout << "Error: Unknow Command !!! " << G4endl;
}



