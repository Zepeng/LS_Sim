#ifndef LSDetectorMessenger_h
#define LSDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
class LSDetectorConstruction;

class G4UIcommand;
//class G4UIcmdWithABool;
//class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class LSDetectorMessenger : public G4UImessenger {

    public:
        LSDetectorMessenger(LSDetectorConstruction* lsDet);
        ~LSDetectorMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);


    private:
        LSDetectorConstruction* m_LSDetector;

        G4UIdirectory     	  	*lsDetDirectory;
       	G4UIcmdWithAnInteger	*opticksModeCmd;
};

#endif


