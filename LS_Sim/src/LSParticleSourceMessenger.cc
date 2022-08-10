#include "LSParticleSourceMessenger.hh"
#include "LSParticleSource.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4ios.hh"

LSParticleSourceMessenger::LSParticleSourceMessenger(LSParticleSource* fPtclGun)
    : fParticleGun(fPtclGun)
{
    gunDirectory = new G4UIdirectory("/LS/gun/");
    gunDirectory -> SetGuidance("Particle Source Control Commands");

    numCmd = new G4UIcmdWithAnInteger("/LS/gun/parNum", this);
    numCmd -> SetGuidance("Set initial particle number.");
    numCmd -> SetParameterName("particleNumber", true);
    numCmd -> SetDefaultValue(1);

    // particle type :
    particle1Cmd = new G4UIcmdWithAString("/LS/gun/particle1", this);
    particle1Cmd->SetGuidance("Set particle type.");
    particle1Cmd->SetParameterName("particleName", true);
    particle1Cmd->SetDefaultValue("e-");

    // momentum type :
    momType1Cmd = new G4UIcmdWithAString("/LS/gun/momtype1", this);
    momType1Cmd->SetGuidance("Set momentum type.");
    momType1Cmd->SetParameterName("momType", true);
    momType1Cmd->SetDefaultValue("iso");

    // momentum vector :
    mom1Cmd = new G4UIcmdWith3VectorAndUnit("/LS/gun/mom1", this);
    mom1Cmd->SetGuidance("Set momentum of the particle.");
    mom1Cmd->SetParameterName("PX", "PY", "PZ", true, true);
    mom1Cmd->SetDefaultUnit("MeV");
    mom1Cmd->SetUnitCandidates("eV MeV GeV");

    // kinetic energy :
    energy1Cmd = new G4UIcmdWithADoubleAndUnit("/LS/gun/energy1", this);
    energy1Cmd->SetGuidance("Set kinetic energy.");
    energy1Cmd->SetParameterName("KE", true);
    energy1Cmd->SetDefaultValue(1);
    energy1Cmd->SetDefaultUnit("MeV");
    energy1Cmd->SetUnitCandidates("eV MeV GeV");

    // position :
    pos1Cmd = new G4UIcmdWith3VectorAndUnit("/LS/gun/position1", this);
    pos1Cmd->SetGuidance("Set particle position.");
    pos1Cmd->SetParameterName("X", "Y", "Z", true, true);
    pos1Cmd->SetDefaultUnit("mm");
    pos1Cmd->SetUnitCandidates("mm cm m");

    // particle type :
    particle2Cmd = new G4UIcmdWithAString("/LS/gun/particle2", this);
    particle2Cmd->SetGuidance("Set particle type.");
    particle2Cmd->SetParameterName("particleName", true);
    particle2Cmd->SetDefaultValue("e-");

    // momentum type :
    momType2Cmd = new G4UIcmdWithAString("/LS/gun/momtype2", this);
    momType2Cmd->SetGuidance("Set momentum type.");
    momType2Cmd->SetParameterName("momType", true);
    momType2Cmd->SetDefaultValue("iso");

    // momentum vector :
    mom2Cmd = new G4UIcmdWith3VectorAndUnit("/LS/gun/mom2", this);
    mom2Cmd->SetGuidance("Set momentum of the particle.");
    mom2Cmd->SetParameterName("PX", "PY", "PZ", true, true);
    mom2Cmd->SetDefaultUnit("MeV");
    mom2Cmd->SetUnitCandidates("eV MeV GeV");

    // kinetic energy :
    energy2Cmd = new G4UIcmdWithADoubleAndUnit("/LS/gun/energy2", this);
    energy2Cmd->SetGuidance("Set kinetic energy.");
    energy2Cmd->SetParameterName("KE", true);
    energy2Cmd->SetDefaultValue(2);
    energy2Cmd->SetDefaultUnit("MeV");
    energy2Cmd->SetUnitCandidates("eV MeV GeV");

    // position :
    pos2Cmd = new G4UIcmdWith3VectorAndUnit("/LS/gun/position2", this);
    pos2Cmd->SetGuidance("Set particle position.");
    pos2Cmd->SetParameterName("X", "Y", "Z", true, true);
    pos2Cmd->SetDefaultUnit("mm");
    pos2Cmd->SetUnitCandidates("mm cm m");


}

LSParticleSourceMessenger::~LSParticleSourceMessenger()
{
    delete numCmd;

    delete particle1Cmd;
    delete momType1Cmd;
    delete mom1Cmd;
    delete pos1Cmd;
    delete energy1Cmd;

    delete particle2Cmd;
    delete momType2Cmd;
    delete mom2Cmd;
    delete pos2Cmd;
    delete energy2Cmd;

    delete gunDirectory;
}




void LSParticleSourceMessenger::SetNewValue
(G4UIcommand* cmd, G4String newValues) {

    if (cmd == numCmd) 
        fParticleGun->SetParticleNumber(numCmd->GetNewIntValue(newValues));
    
    else if (cmd == particle1Cmd) {
        G4ParticleDefinition* pd = particleTable->FindParticle(newValues);
        if (pd != NULL)
            fParticleGun->SetParticleDefinition1(pd);
    }

    else if (cmd == momType1Cmd)
        fParticleGun->SetMomentumType1(newValues);

    else if (cmd == mom1Cmd)
        fParticleGun->SetMomentum1(mom1Cmd->GetNew3VectorValue(newValues));

    else if (cmd == energy1Cmd) 
        fParticleGun->SetKineticEnergy1(energy1Cmd->GetNewDoubleValue(newValues));

    else if (cmd == pos1Cmd)
        fParticleGun->SetPosition1(pos1Cmd->GetNew3VectorValue(newValues));

    else if (cmd == particle2Cmd) {
        G4ParticleDefinition* pd = particleTable->FindParticle(newValues);
        if (pd != NULL)
            fParticleGun->SetParticleDefinition2(pd);
    }

    else if (cmd == momType2Cmd)
        fParticleGun->SetMomentumType2(newValues);

    else if (cmd == mom2Cmd)
        fParticleGun->SetMomentum2(mom2Cmd->GetNew3VectorValue(newValues));

    else if (cmd == energy2Cmd) 
        fParticleGun->SetKineticEnergy2(energy2Cmd->GetNewDoubleValue(newValues));

    else if (cmd == pos2Cmd)
        fParticleGun->SetPosition2(pos2Cmd->GetNew3VectorValue(newValues));

    else
        G4cout << "Error: Unknow Command !!! " << G4endl;

}





