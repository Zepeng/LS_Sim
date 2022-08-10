#ifndef ContinuousSpectrumMessenger_h
#define ContinuousSpectrumMessenger_h

#include "G4UImessenger.hh"
class ContinuousSpectrumManager;
class G4UImessenger;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class ContinuousSpectrumMessenger : public G4UImessenger
{
    public:
        ContinuousSpectrumMessenger();
        ~ContinuousSpectrumMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);

    private:
        ContinuousSpectrumManager*  manager;
        G4UIdirectory*              specDir;
        G4UIcmdWithAString*         inFileCmd;
        G4UIcmdWithAString*         inTreeCmd;
        G4UIcmdWithABool*           activateCmd;
        G4UIcmdWithADouble*         startEvtCmd;
    
};

#endif
