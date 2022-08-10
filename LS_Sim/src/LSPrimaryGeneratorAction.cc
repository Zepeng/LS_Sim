#include "LSPrimaryGeneratorAction.hh"
#include "LSParticleSource.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSPrimaryGeneratorAction::LSPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
    //fParticleGun      = new G4ParticleGun();

    fParticleGun    = new LSParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSPrimaryGeneratorAction::~LSPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    
    //fParticleGun = new LSParticleSource();
    //G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    //G4String particleName;
    //G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
    //fParticleGun->SetParticleDefinition(particle);
    //fParticleGun->SetKineticEnergy(1.0*MeV);
    //fParticleGun->SetPosition(G4ThreeVector(0, 0, 0));
    fParticleGun->GeneratePrimaryVertex( anEvent );


    /*
    // Generate more than one particle each time
    G4int NumberOfParticlesToBeGenerated = 1;
    fParticleGun = new G4ParticleGun(NumberOfParticlesToBeGenerated);

    // particle definition
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
    //G4ParticleDefinition* particle = particleTable->FindParticle(particleName="opticalphoton");


    // set particle type
    fParticleGun->SetParticleDefinition(particle);

    // set optical photon energy/wavelength
    fParticleGun->SetParticleEnergy(0.2*MeV);

    // set momentum direction
    G4double mom_x, mom_y, mom_z ;
    mom_x = 0;
    mom_y = 0;
    mom_z = 1*MeV;
    fParticleGun->SetParticleMomentumDirection( G4ThreeVector(mom_x, mom_y, mom_z));

    //G4double pol_x, pol_y, pol_z;
    //pol_x = 1;
    //pol_y = 0;
    //pol_z = 0;
    //fParticleGun->SetParticlePolarization( G4ThreeVector(pol_x, pol_y, pol_z) );

    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));

    fParticleGun->GeneratePrimaryVertex( anEvent );
    */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

