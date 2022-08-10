#include "LSParticleSource.hh"
#include "ContinuousSpectrumManager.hh"

#include "G4Event.hh"
#include "Randomize.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4PrimaryParticle.hh"
#include "G4PhysicalConstants.hh"

LSParticleSource::LSParticleSource() {

    me = 0.511*MeV;

    m_parNum = 1;

    m_particle1         = NULL;
    m_energy1           = 1.0*MeV;
    m_MomType1          = "iso";    // candidates: iso, direction, range
    m_Mom1              = G4ThreeVector(1., 0., 0.);
    m_pos1              = G4ThreeVector(0, 0, 0);

    m_particle2         = NULL;
    m_energy2           = 1.0*MeV;
    m_MomType2          = "iso";    // candidates: iso, direction, range
    m_Mom2              = G4ThreeVector(1., 0., 0.);
    m_pos2              = G4ThreeVector(0, 0, 0);

    theMessenger        = new LSParticleSourceMessenger(this);
}

LSParticleSource::~LSParticleSource()
{
    delete theMessenger;
}

void LSParticleSource::SetParticleNumber(G4int number)
{
    m_parNum = number;
}


void LSParticleSource::SetParticleDefinition1(G4ParticleDefinition* aParticleDefinition) 
{
    m_particle1 = aParticleDefinition;
}


void LSParticleSource::SetKineticEnergy1(G4double energy)
{
    m_energy1 = energy;
}

void LSParticleSource::SetMomentumType1(G4String type)
{
    m_MomType1 = type;
}

void LSParticleSource::SetMomentum1(G4ThreeVector mom)
{
    m_Mom1 = mom;
}

void LSParticleSource::SetPosition1(G4ThreeVector pos)
{
    m_pos1 = pos;
}

void LSParticleSource::SetParticleDefinition2(G4ParticleDefinition* aParticleDefinition) 
{
    m_particle2 = aParticleDefinition;
}


void LSParticleSource::SetKineticEnergy2(G4double energy)
{
    m_energy2 = energy;
}

void LSParticleSource::SetMomentumType2(G4String type)
{
    m_MomType2 = type;
}

void LSParticleSource::SetMomentum2(G4ThreeVector mom)
{
    m_Mom2 = mom;
}

void LSParticleSource::SetPosition2(G4ThreeVector pos)
{
    m_pos2 = pos;
}


void LSParticleSource::GenerateIsotropicFlux1()
{

    G4double p =sqrt(m_energy1*m_energy1 + 2*me*m_energy1);

    G4double rndm, rndm2;
    G4double px, py, pz;

    G4double sintheta, sinphi, costheta, cosphi;
    rndm = G4UniformRand();
    costheta = -1 + rndm * 2;
    sintheta = std::sqrt(1. - costheta*costheta);

    rndm2 = G4UniformRand();
    G4double Phi = 2 * pi * rndm2; 
    sinphi = std::sin(Phi);
    cosphi = std::cos(Phi);

    px = -sintheta * cosphi * p;
    py = -sintheta * sinphi * p;
    pz = -costheta * p;

    G4double ResMag = std::sqrt((px*px) + (py*py) + (pz*pz));
    px = px/ResMag;
    py = py/ResMag;
    pz = pz/ResMag;

    m_Mom1.setX(px);
    m_Mom1.setY(py);
    m_Mom1.setZ(pz);

}


void LSParticleSource::GenerateIsotropicFlux2()
{

    G4double p =sqrt(m_energy2*m_energy2 + 2*me*m_energy2);

    G4double rndm, rndm2;
    G4double px, py, pz;

    G4double sintheta, sinphi, costheta, cosphi;
    rndm = G4UniformRand();
    costheta = -1 + rndm * 2;
    sintheta = std::sqrt(1. - costheta*costheta);

    rndm2 = G4UniformRand();
    G4double Phi = 2 * pi * rndm2; 
    sinphi = std::sin(Phi);
    cosphi = std::cos(Phi);

    px = -sintheta * cosphi * p;
    py = -sintheta * sinphi * p;
    pz = -costheta * p;

    G4double ResMag = std::sqrt((px*px) + (py*py) + (pz*pz));
    px = px/ResMag;
    py = py/ResMag;
    pz = pz/ResMag;

    m_Mom2.setX(px);
    m_Mom2.setY(py);
    m_Mom2.setZ(pz);

}



void LSParticleSource::GeneratePrimaryVertex(G4Event* event)
{
    G4double m_load_edep = 0;
    if (m_parNum == 1 and ContinuousSpectrumManager::GetInstance()->GetActivate()) {
        // load continuous spectrum file
        if (event->GetEventID() < ContinuousSpectrumManager::GetInstance()->GetMaxEvt()) {
            m_load_edep = ContinuousSpectrumManager::GetInstance()->GetEdep(event->GetEventID());
        } else {
            m_load_edep = ContinuousSpectrumManager::GetInstance()->GetEdep(0);
        }
    }


    if(m_parNum == 1 and m_particle1 == NULL) {
        G4cout << "No particle has been defined !" << G4endl;
        return;
    }
    else if(m_parNum == 2 and (m_particle1 == NULL or m_particle2 == NULL) ) {
        G4cout << "No enough particles have been defined !" << G4endl;
        return;
    }

    if (m_parNum == 1) { // single particles
        G4PrimaryVertex* vertex = new G4PrimaryVertex(m_pos1, 0);
        G4PrimaryParticle* particle = new G4PrimaryParticle(m_particle1, m_Mom1.x(), m_Mom1.y(), m_Mom1.z());
        particle->SetKineticEnergy(m_energy1);
        if (ContinuousSpectrumManager::GetInstance()->GetActivate()) {
            particle -> SetKineticEnergy(m_load_edep);
        }

        if(m_MomType1 == "iso") {
            GenerateIsotropicFlux1();
        }

        vertex->SetPrimary(particle);
        event->AddPrimaryVertex(vertex);
    }


    if (m_parNum == 2) { // single particles
        G4PrimaryVertex* vertex1 = new G4PrimaryVertex(m_pos1, 0);
        G4PrimaryParticle* particle1 = new G4PrimaryParticle(m_particle1, m_Mom1.x(), m_Mom1.y(), m_Mom1.z());
        particle1->SetKineticEnergy(m_energy1);

        if(m_MomType1 == "iso") {
            GenerateIsotropicFlux1();
        }

        vertex1->SetPrimary(particle1);
        event->AddPrimaryVertex(vertex1);


        G4PrimaryVertex* vertex2 = new G4PrimaryVertex(m_pos2, 0);
        G4PrimaryParticle* particle2 = new G4PrimaryParticle(m_particle2, m_Mom2.x(), m_Mom2.y(), m_Mom2.z());
        particle2->SetKineticEnergy(m_energy2);

        if(m_MomType2 == "iso") {
            GenerateIsotropicFlux2();
        }

        vertex2->SetPrimary(particle2);
        event->AddPrimaryVertex(vertex2);
    }


}





