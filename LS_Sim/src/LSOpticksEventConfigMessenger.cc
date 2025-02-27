#include "LSOpticksEventConfigMessenger.hh"
#include "LSDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

#include <cassert>

LSOpticksEventConfigMessenger* LSOpticksEventConfigMessenger::instance = nullptr;

LSOpticksEventConfigMessenger* LSOpticksEventConfigMessenger::GetInstance(){
	if (!instance)
		instance = new LSOpticksEventConfigMessenger();
    	return instance;
}

LSOpticksEventConfigMessenger::LSOpticksEventConfigMessenger(LSDetectorConstruction* lsDet)
    : 
	m_LSDetector(lsDet),
	m_opticksMode(0)
{

    evtConfigDirectory = new G4UIdirectory("/Opticks/EventConfig/");
    evtConfigDirectory -> SetGuidance("Opticks Event configer register.");
	
    opticksModeCmd = new G4UIcmdWithAnInteger("/Opticks/EventConfig/OpticksMode", this);
    opticksModeCmd -> SetGuidance("Set Opticks mode of this simulation.");
    opticksModeCmd -> SetParameterName("opticksMode", true);
    opticksModeCmd -> SetDefaultValue(0);


    maxPhotonCmd = new G4UIcmdWithAnInteger("/Opticks/EventConfig/MaxPhoton", this);
    maxPhotonCmd -> SetGuidance("Set max optical photon of the simulation.");
    maxPhotonCmd -> SetParameterName("MaxPhoton", true);
    maxPhotonCmd -> SetDefaultValue(-1);
	
    maxGenstepCmd = new G4UIcmdWithAnInteger("/Opticks/EventConfig/MaxGenstep", this);
    maxGenstepCmd -> SetGuidance("Set max Gensteo of the simulation");
    maxGenstepCmd -> SetParameterName("MaxGenstep", true);
    maxGenstepCmd -> SetDefaultValue(-1);

}


LSOpticksEventConfigMessenger::LSOpticksEventConfigMessenger()

{
}
LSOpticksEventConfigMessenger::~LSOpticksEventConfigMessenger()
{
    delete maxPhotonCmd;
    delete maxGenstepCmd;
    delete opticksModeCmd;
    delete evtConfigDirectory;
}


void LSOpticksEventConfigMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {
    
    if (cmd == maxGenstepCmd){
        m_LSDetector->SetOpticksMaxGenstep(maxGenstepCmd->GetNewIntValue(newValues));}
	else if ( cmd == maxPhotonCmd){
		m_LSDetector->SetOpticksMaxPhoton(maxPhotonCmd->GetNewIntValue(newValues));}
	else if ( cmd == opticksModeCmd ){
		m_opticksMode = opticksModeCmd->GetNewIntValue(newValues);
		G4cout<<" m_opticksMode [ LSOpticksEventConfigMessenger::SetNewValue "<< m_opticksMode;}
    else{
        G4cout << "Error: Unknow Command !!! " << G4endl;}
}

