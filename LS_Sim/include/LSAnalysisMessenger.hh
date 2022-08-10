#ifndef LSAnalysisMessenger_h
#define LSAnalysisMessenger_h 1

#include "globals.hh"
#include "LSAnalysisManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UImessenger.hh"

class LSAnalysisManager;

class LSAnalysisMessenger : public G4UImessenger
{
    public:
        LSAnalysisMessenger(LSAnalysisManager*);
        ~LSAnalysisMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);

    private:
        LSAnalysisManager  *analysis;
        G4UIdirectory      *anaDir;
        G4UIcmdWithAString *outFileCmd;
};

#endif
