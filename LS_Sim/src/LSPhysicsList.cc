
#include "G4ProcessManager.hh"
#include "LSPhysicsList.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4SystemOfUnits.hh"

// particles

#include "G4OpticalPhoton.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"


LSPhysicsList::LSPhysicsList() : G4VModularPhysicsList()
{

    m_enableoptical = true;
    m_yield = 1.0;

    emPhysicsList = new G4EmLivermorePhysics();
    decayPhysicsList = new G4DecayPhysics();

    theMessenger = new LSPhysicsListMessenger(this);

}

LSPhysicsList::~LSPhysicsList() {
    delete emPhysicsList;
    delete decayPhysicsList;

    delete theMessenger;
}

void LSPhysicsList::SetCuts() {
    //SetCutsWithDefault();
    defaultCutValue = 1.0*mm;
    const G4double cutForGamma = defaultCutValue;
    defaultCutValue = 0.1*mm;
    const G4double cutForElectron = defaultCutValue;
    const G4double cutForPositron = defaultCutValue;

    SetCutValue(cutForGamma, "gamma");
    SetCutValue(cutForElectron, "e-");
    SetCutValue(cutForPositron, "e+");

}


void LSPhysicsList::ConstructParticle()
{

    G4OpticalPhoton::OpticalPhotonDefinition();

    G4Gamma::GammaDefinition();
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();

    decayPhysicsList->ConstructParticle();
    emPhysicsList->ConstructParticle();

}

void LSPhysicsList::ConstructProcess()
{
    AddTransportation();
    ConstructOpticalProcess();
    emPhysicsList->ConstructProcess();
    decayPhysicsList->ConstructProcess();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
//#include "G4Cerenkov.hh"
#include "LSCherenkov.hh"
#include "DsG4Scintillation.hh"

void LSPhysicsList::ConstructOpticalProcess()
{

    // Cerenkov Process :
    //G4Cerenkov* theCerProcess             = new G4Cerenkov();
    LSCherenkov* theCerProcess   = new LSCherenkov();
    theCerProcess->SetMaxNumPhotonsPerStep(300);
    theCerProcess->SetTrackSecondariesFirst(true);
    theCerProcess->SetScaleFactor(1.0);

    // Scintillation Process :
    DsG4Scintillation* scint = new DsG4Scintillation();
    scint->SetDoQuenching(true);
    scint->SetBirksConstant1(6.5e-3*g/cm2/MeV);
    scint->SetBirksConstant2(1.5e-6*(g/cm2/MeV)*(g/cm2/MeV));
    scint->SetGammaSlowerTimeConstant(190*ns);
    scint->SetGammaSlowerRatio(0.15);
    scint->SetDoReemission(true);
    scint->SetDoReemissionOnly(false);
    scint->SetDoBothProcess(true);
    scint->SetScintillationYieldFactor(m_yield); //1.);
    scint->SetTrackSecondariesFirst(true);
    scint->SetFlagDecayTimeFast(true);
    scint->SetFlagDecayTimeSlow(true);
    scint->SetVerboseLevel(0);



    G4OpAbsorption* theAbsProcess         = new G4OpAbsorption();
    G4OpRayleigh* theRayProcess           = new G4OpRayleigh();
    G4OpBoundaryProcess* theBdProcess     = new G4OpBoundaryProcess();
    theBdProcess->SetInvokeSD(false);
    auto particleIterator = GetParticleIterator();
    particleIterator->reset();
    while( (*particleIterator)() ){

        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (theAbsProcess->IsApplicable(*particle)) {
            pmanager -> AddDiscreteProcess(theAbsProcess);
            G4cout << " ===> Registered Absorption Process " << G4endl;
        }

        if(theRayProcess->IsApplicable(*particle)) {
            pmanager -> AddDiscreteProcess(theRayProcess);
            G4cout << " ===> Registered Rayleigh Scatterinng Process " << G4endl;
        }

        if(theBdProcess->IsApplicable(*particle)) {
            pmanager -> AddDiscreteProcess(theBdProcess);
            G4cout << " ===> Registered Boundary Process " << G4endl;
        }

        if (m_enableoptical) {
            if(theCerProcess->IsApplicable(*particle)) {
                pmanager -> AddProcess(theCerProcess);
                pmanager -> SetProcessOrdering(theCerProcess, idxPostStep);
            }
            if(scint -> IsApplicable(*particle)) {
                pmanager -> AddProcess(scint);
                pmanager -> SetProcessOrderingToLast( scint, idxAtRest);
                pmanager -> SetProcessOrderingToLast( scint, idxPostStep);
            }
        }
    }
}
