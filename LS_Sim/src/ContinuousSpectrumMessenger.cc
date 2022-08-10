#include "ContinuousSpectrumMessenger.hh"
#include "ContinuousSpectrumManager.hh"

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"

ContinuousSpectrumMessenger::ContinuousSpectrumMessenger()
    : G4UImessenger()
{
    specDir = new G4UIdirectory("/LS/spec/");
    specDir -> SetGuidance("Read continuous beta spectrum for simulation.");

    inFileCmd = new G4UIcmdWithAString("/LS/spec/input", this);
    inFileCmd -> SetGuidance("Set input spectrum file name.");
    inFileCmd -> SetParameterName("filename", true);
    inFileCmd -> SetDefaultValue("/junofs/users/miaoyu/energy_model/production/J19v1r0-Pre4/michel/michel_totpe_LS_v7.root");
    //inFileCmd -> SetDefaultValue("/junofs/users/miaoyu/energy_model/production/J19v1r0-Pre4/B12/B12_edep_0.root");     // B12 generator

    inTreeCmd = new G4UIcmdWithAString("/LS/spec/tree", this);
    inTreeCmd -> SetGuidance("Set input spectrum tree name.");
    inTreeCmd -> SetParameterName("treename", true);
    inTreeCmd -> SetDefaultValue("sim");

    activateCmd = new G4UIcmdWithABool("/LS/spec/activate", this);
    activateCmd->SetGuidance("Set if read continuous spectrum for simulation.");
    activateCmd->SetParameterName("activate", true);
    activateCmd->SetDefaultValue(false);

    startEvtCmd = new G4UIcmdWithADouble("/LS/spec/startid", this);
    startEvtCmd->SetGuidance("Set the start evtid.");
    startEvtCmd->SetParameterName("startid", true);
    startEvtCmd->SetDefaultValue(0);

}

ContinuousSpectrumMessenger::~ContinuousSpectrumMessenger()
{
    delete activateCmd;
    delete inTreeCmd;
    delete inFileCmd;
    delete startEvtCmd;
    delete specDir;
}


void ContinuousSpectrumMessenger::SetNewValue(G4UIcommand* cmd, G4String newValues)
{
    if (cmd == inFileCmd) {
        ContinuousSpectrumManager::GetInstance()->SetFilename(newValues);
    }

    else if (cmd == inTreeCmd) {
        ContinuousSpectrumManager::GetInstance()->SetTreename(newValues);
    }

    else if (cmd == activateCmd) {
        ContinuousSpectrumManager::GetInstance()->SetActivate(activateCmd->GetNewBoolValue(newValues));
    }

    else if (cmd == startEvtCmd) {
        ContinuousSpectrumManager::GetInstance()->SetStartEvtId(startEvtCmd->GetNewDoubleValue(newValues));
    }

    else
        G4cout << "Error: Unknow Command !!! " << G4endl;

}






