#ifndef LSParticleSource_h
#define LSParticleSource_h

#include "G4ParticleDefinition.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4ThreeVector.hh"
#include "LSParticleSourceMessenger.hh"

class LSParticleSource : public G4VPrimaryGenerator 
{
    
    public:
        LSParticleSource();
        ~LSParticleSource();

        void GeneratePrimaryVertex(G4Event* event);

    public:
        void SetParticleDefinition1(G4ParticleDefinition* aParticleDefinition);
        void SetParticleDefinition2(G4ParticleDefinition* aParticleDefinition);
        
        void SetParticleNumber(G4int parNum);

        void GenerateIsotropicFlux1();
        void GenerateIsotropicFlux2();
        void SetKineticEnergy1(G4double energy);
        void SetMomentumType1(G4String type);
        void SetMomentum1(G4ThreeVector mom);
        void SetPosition1(G4ThreeVector pos);
        void SetKineticEnergy2(G4double energy);
        void SetMomentumType2(G4String type);
        void SetMomentum2(G4ThreeVector mom);
        void SetPosition2(G4ThreeVector pos);
        
    private:
        G4double                me;

        G4int                   m_parNum;

        G4String                m_MomType1;
        G4ThreeVector           m_Mom1;
        G4double                m_energy1;
        G4ThreeVector           m_pos1;
        G4ParticleDefinition*   m_particle1;

        G4String                m_MomType2;
        G4ThreeVector           m_Mom2;
        G4double                m_energy2;
        G4ThreeVector           m_pos2;
        G4ParticleDefinition*   m_particle2;
    
    private:
        LSParticleSourceMessenger* theMessenger;

};


#endif


