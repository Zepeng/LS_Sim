#include "LSAnalysisMessenger.hh"

LSAnalysisMessenger::LSAnalysisMessenger(
        LSAnalysisManager* LSAnalysis)
    : analysis(LSAnalysis)
{
    anaDir = new G4UIdirectory("/LS/ana/") ;
    anaDir -> SetGuidance("Analysis control.");
    
    outFileCmd = new G4UIcmdWithAString("/LS/ana/output", this);
    outFileCmd -> SetGuidance("Set output file name.");
    outFileCmd -> SetParameterName("filename", true);
    outFileCmd -> SetDefaultValue("test");

}


LSAnalysisMessenger::~LSAnalysisMessenger()
{
    delete outFileCmd;
    delete anaDir;
}

void LSAnalysisMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
    if (cmd == outFileCmd) {
        analysis->SetOutputName(newValue);
    }
    
    else
        G4cout << "Error: Unknown Command !" << G4endl;
}


