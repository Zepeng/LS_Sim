#include "LSOpticksEventConfigMessenger.hh"
#include "LSDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

LSOpticksEventConfigMessenger::LSOpticksEventConfigMessenger(LSDetectorConstruction* lsDet)
    : 
	m_LSDetector(lsDet)

{

    evtConfigDirectory = new G4UIdirectory("/Opticks/EventConfig");
    evtConfigDirectory -> SetGuidance("Opticks Event configer register.");

    maxPhotonCmd = new G4UIcmdWithAnInteger("/Opticks/EventConfig/MaxPhoton", this);
    maxPhotonCmd -> SetGuidance("Set max optical photon of the simulation.");
    maxPhotonCmd -> SetParameterName("MaxPhoton", true);
    maxPhotonCmd -> SetDefaultValue(-1);
	
	maxGenstepCmd = new G4UIcmdWithAnInteger("/Opticks/EventConfig/MaxGenstep", this);
	maxGenstepCmd -> SetGuidance("Set max Gensteo of the simulation");
	maxGenstepCmd -> SetParameterName("MaxGenstep", true);
	maxGenstepCmd -> SetDefaultValue(-1);

}



LSOpticksEventConfigMessenger::~LSOpticksEventConfigMessenger()
{
    delete maxPhotonCmd;
	delete maxGenstepCmd;
    delete evtConfigDirectory;
}


void LSOpticksEventConfigMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {
    
    if (cmd == maxGenstepCmd) 
        m_LSDetector->SetOpticksMaxGenstep(maxGenstepCmd->GetNewIntValue(newValues));
	else if ( cmd == maxPhotonCmd)
		m_LSDetector->SetOpticksMaxPhoton(maxPhotonCmd->GetNewIntValue(newValues));
    else
        G4cout << "Error: Unknow Command !!! " << G4endl;
}

