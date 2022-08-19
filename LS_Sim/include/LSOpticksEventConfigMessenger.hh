#ifndef LSOpticksEventConfigMessenger_h
#define LSOpticksEventConfigMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
class LSDetectorConstruction;

class G4UIcommand;
//class G4UIcmdWithABool;
//class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class LSOpticksEventConfigMessenger : public G4UImessenger {

    public:
        LSOpticksEventConfigMessenger(LSDetectorConstruction* lsDet);
        ~LSOpticksEventConfigMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);


    private:
        LSDetectorConstruction*  m_LSDetector;

        G4UIdirectory     	  	*evtConfigDirectory;
       	G4UIcmdWithAnInteger	*maxPhotonCmd;
		G4UIcmdWithAnInteger    *maxGenstepCmd;
};

#endif


