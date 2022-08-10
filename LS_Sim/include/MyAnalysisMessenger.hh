#ifndef MyAnalysisMessenger_h
#define MyAnalysisMessenger_h 
#include "G4UImessenger.hh"

class MyAnalysisManager;
class G4UImessenger;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

class MyAnalysisMessenger : public G4UImessenger
{
    public:
        MyAnalysisMessenger();
        ~MyAnalysisMessenger();

        void SetNewValue(G4UIcommand* cmd, G4String newValues);

    private:
        MyAnalysisManager*          analysis;
        G4UIdirectory*              anaDir;
        G4UIcmdWithAString*         outFileCmd;
        G4UIcmdWithABool*           activateCmd;
        G4UIcmdWithADoubleAndUnit*  birks1Cmd;

};

#endif
