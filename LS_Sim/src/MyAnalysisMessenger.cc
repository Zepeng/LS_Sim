#include "MyAnalysisMessenger.hh"
#include "MyAnalysisManager.hh"

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

MyAnalysisMessenger::MyAnalysisMessenger()
    : G4UImessenger()
{
    anaDir = new G4UIdirectory("/My/ana/");
    anaDir -> SetGuidance("Root Based Analysis Control.");

    outFileCmd = new G4UIcmdWithAString("/My/ana/output", this);
    outFileCmd -> SetGuidance("Set output file name.");
    outFileCmd -> SetParameterName("filename", true);
    outFileCmd -> SetDefaultValue("rootbased");

    //birks1Cmd = new G4UIcmdWithADoubleAndUnit("/My/ana/birks1", this);
    //birks1Cmd->SetGuidance("Set Birks1 Constant.");
    //birks1Cmd->SetParameterName("birks1", true);
    //birks1Cmd->SetDefaultValue(1);
    //birks1Cmd->SetDefaultUnit("g/cm2/MeV");
    //birks1Cmd->SetUnitCandidates("g/cm2/MeV");

    activateCmd = new G4UIcmdWithABool("/My/ana/activate", this);
    activateCmd->SetGuidance("Set activate status of root-based analysis.");
    activateCmd->SetParameterName("activate", true);
    activateCmd->SetDefaultValue(true);

}

MyAnalysisMessenger::~MyAnalysisMessenger()
{
    delete outFileCmd;
    delete birks1Cmd;
    delete activateCmd;
    delete anaDir;

}

void MyAnalysisMessenger::SetNewValue(G4UIcommand* cmd, G4String newValues)
{
    if (cmd == outFileCmd) {
        G4cout << "\n --> Set root-based output filename: " << newValues << G4endl;
        MyAnalysisManager::GetInstance()->SetFileName(newValues);
    }

    //else if (cmd == birks1Cmd) {
    //    MyAnalysisManager::GetInstance()->SetBirks1(birks1Cmd->GetNewDoubleValue(newValues));
    //}
    
    else if (cmd == activateCmd) {
        MyAnalysisManager::GetInstance()->SetActivate(activateCmd->GetNewBoolValue(newValues));
    }

    else
        G4cout << "Error: Unknow Command !!! " << G4endl;

}





