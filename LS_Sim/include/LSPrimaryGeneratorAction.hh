
//
/// \file LSPrimaryGeneratorAction.hh
/// \brief Definition of the LSPrimaryGeneratorAction class

#ifndef LSPrimaryGeneratorAction_h
#define LSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "LSParticleSource.hh"

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class LSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    LSPrimaryGeneratorAction();    
    virtual ~LSPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const LSParticleSource* GetParticleGun() const { return fParticleGun; }
    //const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    //G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    LSParticleSource* fParticleGun;
    G4int NumberOfParticlesToBeGenerated;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
