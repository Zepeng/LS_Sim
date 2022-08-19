#ifndef LSDetectorConstructionMessenger_h
#define LSDetectorConstructionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
class LSDetectorConstruction;

class G4UIcommand;
//class G4UIcmdWithABool;
//class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class LSDetectorConstructionMessenger : public G4UImessenger {

    public:
        LSDetectorConstructionMessenger(LSDetectorConstruction* lsDet);
        ~LSDetectorConstructionMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);


    private:
        LSDetectorConstruction* m_LSDetector;

        G4UIdirectory     	  	*lsDetDirectory;
       	G4UIcmdWithAnInteger	*opticksModeCmd;
};

#endif


