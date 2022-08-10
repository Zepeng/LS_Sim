#include "MyRootBasedAnalysis.hh"

#include "G4Material.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4LossTableManager.hh"
#include "G4SystemOfUnits.hh"

#include "TFile.h"
#include "TTree.h"


MyRootBasedAnalysis::MyRootBasedAnalysis()
{
    SetFileName("output.root");

    m_BirksConstant1 = 6.5e-3*g/cm2/MeV;
    m_BirksConstant2 = 1.5e-6*(g/cm2/MeV)*(g/cm2/MeV);
}

MyRootBasedAnalysis::~MyRootBasedAnalysis()
{
}

void MyRootBasedAnalysis::BeginOfRunAction()
{
    if (not active)
        return;

    fRootFp = new TFile(fFileName, "recreate");
    if (!fRootFp)
    {
        G4cout << "\n====>MyRootBasedAnalysis::BeginOfRunAction(): "
               << "cannot open " << fFileName << G4endl;
        return;
    }

    fTree = new TTree("sim", "Tree of data events");
    fTree->Branch("edep", &edep, "edep/F");
    fTree->Branch("Qedep", &qedep, "qedep/F");
    fTree->Branch("trackLength", &track_length, "track_length/F");

    return;

}

void MyRootBasedAnalysis::EndOfRunAction()
{
    if (not active)
        return;
    if (!fRootFp)
    {
        G4cout << "\n====>MyRootBasedAnalysis::EndOfRunAction(): "
               << "cannot open " << fFileName << G4endl;
        return;
    }

    //------- add your codes down here
    //

    G4cout << "\n====>In total " << fTree->GetEntries() << " Events have been stored." << G4endl;
    fRootFp->Write();
    fRootFp->Close();
    return;
}


void MyRootBasedAnalysis::BeginOfEventAction(const G4Event* )
{
    if (!active)
        return;

    // Initialization :
    edep = 0.;
    qedep = 0.;
    track_length = 0.;

    //------- add your codes down here
    //
    return;
}


void MyRootBasedAnalysis::EndOfEventAction(const G4Event* )
{
    if (!active)
        return;


    fTree->Fill();

    //------- add your codes down here
    //
    return;
}


void MyRootBasedAnalysis::PreTrackingAction(const G4Track* aTrack)
{
    if (!active)
        return;

    //------- add your codes down here
    return;
}


void MyRootBasedAnalysis::PostTrackingAction(const G4Track* aTrack)
{
    if (!active)
        return;

    if (aTrack->GetTrackID() == 1) {
        track_length = aTrack->GetTrackLength() *mm;
    }
    //------- add your codes down here
    //
    return;
}


void MyRootBasedAnalysis::SteppingAction(const G4Step* aStep)
{
    if (!active)
        return;

    //------- add your codes down here
    G4Track* aTrack = aStep->GetTrack();
    G4double m_edep = aStep->GetTotalEnergyDeposit();

    if (m_edep > 0 and aTrack->GetDefinition()!=G4OpticalPhoton::Definition()){
        edep += m_edep; 

        double m_qedep = calculateQuenched(aStep);
        qedep += m_qedep;
    }

    return;
}


double MyRootBasedAnalysis::calculateQuenched(const G4Step* aStep) 
{

    double QuenchedTotalEnergyDeposit = 0.;
    double dE = aStep->GetTotalEnergyDeposit();
    double dx = aStep->GetStepLength();

    G4Track* aTrack = aStep->GetTrack();
    G4ParticleDefinition* aParticle = aTrack->GetDefinition();
    
    if (dE > 0) {
        if (aParticle == G4Gamma::Gamma()) {
            G4LossTableManager* manager = G4LossTableManager::Instance();
            dx = manager->GetRange(G4Electron::Electron(), dE, aTrack->GetMaterialCutsCouple());
        }
        G4Material* aMaterial = aStep->GetPreStepPoint()->GetMaterial();
        G4MaterialPropertiesTable* aMaterialPropertiesTable = aMaterial->GetMaterialPropertiesTable();
        if(aMaterialPropertiesTable) {
            const G4MaterialPropertyVector* Fast_Intensity = aMaterialPropertiesTable->GetProperty("FASTCOMPONENT");
            const G4MaterialPropertyVector* Slow_Intensity = aMaterialPropertiesTable->GetProperty("SLOWCOMPONENT");

            if (Fast_Intensity || Slow_Intensity) {
                double delta = dE/dx/aMaterial->GetDensity();
                double birk1 = m_BirksConstant1;
                if(aTrack->GetDefinition()->GetPDGCharge()>1.1)//for particle charge greater than 1.
                    birk1 = 0.57*birk1;
                //double birk2 = (0.0031*g/MeV/cm2)*(0.0031*g/MeV/cm2);
                double birk2 = m_BirksConstant2;
                QuenchedTotalEnergyDeposit = dE /(1+birk1*delta+birk2*delta*delta);

            }
        }
    }

    return QuenchedTotalEnergyDeposit;
}





