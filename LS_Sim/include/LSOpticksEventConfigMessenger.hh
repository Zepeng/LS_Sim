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
	static LSOpticksEventConfigMessenger* GetInstance();
        LSOpticksEventConfigMessenger();
        ~LSOpticksEventConfigMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);
	G4int GetOpticksMode() const{ return m_opticksMode ; }


    private:
	static LSOpticksEventConfigMessenger* instance;

        G4UIdirectory     	*evtConfigDirectory;
       	G4UIcmdWithAnInteger	*maxPhotonCmd;
	G4UIcmdWithAnInteger    *maxGenstepCmd;
	G4UIcmdWithAnInteger    *opticksModeCmd;
	G4int                    m_opticksMode;
};

#endif


