#include "LSDetectorSD.hh"
#include "NormalTrackInfo.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"

LSDetectorSD::LSDetectorSD( const G4String& name, 
                  const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name),
    fHitsCollection(NULL),
    efficiency(0.54)
{
    collectionName.insert(hitsCollectionName);

    analysis = LSAnalysisManager::getInstance();

    theMessenger = new LSDetectorSDMessenger(this); 

}

LSDetectorSD::~LSDetectorSD()
{
    delete theMessenger;
}


void LSDetectorSD::Initialize(G4HCofThisEvent* hce)
{
    // Create hits collection
    fHitsCollection
        = new LSDetectorHitsCollection( SensitiveDetectorName, collectionName[0]);
    
    // Add this collection in hce
    G4int hcID 
        = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection );


}

G4bool LSDetectorSD::ProcessHits( G4Step* aStep, G4TouchableHistory*)
{
    G4Track* track = aStep->GetTrack();
    G4int trackID = track->GetTrackID();
    if (track->GetDefinition() != G4OpticalPhoton::Definition()) {
        return false;
    }
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

    G4double edep = aStep->GetTotalEnergyDeposit();
    G4double stepLength = aStep->GetStepLength();
    if(0) {
        G4cout << "physDetTrack ID " << trackID 
               << ", PreStep radius "  << preStepPoint->GetPosition().mag()  << " " << preStepPoint->GetStepStatus()  << " "<< preStepPoint->GetProcessDefinedStep()->GetProcessName()
               << ", PostStep radius " << postStepPoint->GetPosition().mag() << " " << postStepPoint->GetStepStatus() << " "<< postStepPoint->GetProcessDefinedStep()->GetProcessName()
               << ", edep = " << edep 
               << ", stepLength = " << stepLength
               << G4endl;
    }
    if(edep == 0. ) return false;


    //auto touchable = (aStep->GetPreStepPoint()->GetTouchable());

    G4double wavelength = 1240. * 1e6 / edep;
    G4double time = postStepPoint->GetGlobalTime();
    G4bool is_from_cerenkov = false;
    G4bool is_reemission = false;
    G4bool is_original_op = false;
    G4VUserTrackInformation* trkinfo = track->GetUserInformation();
    if(trkinfo) {
        NormalTrackInfo* normaltrk = dynamic_cast<NormalTrackInfo*>(trkinfo);
        if(normaltrk) {
            is_from_cerenkov = normaltrk->isFromCerenkov();
            is_reemission = normaltrk->isReemission();
            is_original_op = normaltrk->isOriginalOP();
        }
    }

    // consider PMT PDE :  no wavelength response and angular response yet
    G4double pde = G4UniformRand();
    if (pde > efficiency) {
        return false;
    }

    LSDetectorHit* hit = new LSDetectorHit();
    hit->SetTrackID(trackID);
    hit->SetTime(time);
    hit->SetEdep(edep);
    hit->SetWavelength(wavelength);
    hit->SetFromCerenkov(is_from_cerenkov);
    hit->SetReemission(is_reemission);
    hit->SetOriginalOP(is_original_op);

    fHitsCollection->insert(hit);


    return true;
}



void LSDetectorSD::EndOfEvent(G4HCofThisEvent*)
{
    G4int nofHits = fHitsCollection->entries();
    G4int nofCerHits = 0;
    G4int nofSctHits = 0;
    G4int nofOP = 0;
    for ( G4int i=0; i<nofHits; i++ ) {
       if ((*fHitsCollection)[i]->IsFromCerenkov()) nofCerHits += 1;
       if ((*fHitsCollection)[i]->IsReemission())   nofSctHits += 1;
       if ((*fHitsCollection)[i]->IsOriginalOP())   nofOP += 1;
       //(*fHitsCollection)[i] -> Print();
    }
         //for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();

    //G4cout << "======= Total " << nofHits << " hits have been detected with " 
    //       << nofCerHits << " Cerenkov hits and " 
    //       << nofSctHits << " Scintillation hits, the original photons are "  
    //       << nofOP << " ======= !"
    //       << G4endl;

    analysis -> analyseTotNPE(nofHits);
    analysis -> analyseCerNPE(nofCerHits);
    analysis -> analyseSctNPE(nofSctHits);

}

