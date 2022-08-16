#ifndef LSActionInitializationMessenger_h
#define LSActionInitializationMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
//class LSDetectorConstruction;
class LSActionInitialization;
class G4UIcommand;
//class G4UIcmdWithABool;
//class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class LSActionInitializationMessenger : public G4UImessenger {

    public:
        LSActionInitializationMessenger(LSActionInitialization* lsAction);
        ~LSActionInitializationMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);


    private:
        LSActionInitialization* m_LSAction;

        G4UIdirectory     	  	*actDirectory;
       	G4UIcmdWithAnInteger	*opticksModeCmd;
};

#endif


