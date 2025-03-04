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

#ifdef WITH_G4CXOPTICKS
#include <cuda_runtime.h>
#include "OPTICKS_LOG.hh"
#include "G4CXOpticks.hh"
#include "PLOG.hh"
#include "scuda.h"
#include "SEvt.hh"
#include "G4CXOpticks.hh"
#include "NP.hh"
#endif
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
	const G4ThreeVector& global_pos = postStepPoint->GetPosition();
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
    hit->SetGlobalPosX(global_pos.x());
    hit->SetGlobalPosY(global_pos.y());
    hit->SetGlobalPosZ(global_pos.z());

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

    analysis -> analyseTotNPE(nofHits);
    analysis -> analyseCerNPE(nofCerHits);
    analysis -> analyseSctNPE(nofSctHits);

}

#ifdef WITH_G4CXOPTICKS
void LSDetectorSD::AddOpticksHits()
{
  SEvt* sev             = SEvt::Get_EGPU();
  unsigned int num_hits = sev->GetNumHit(0);
  std::cout << "SD sev num_hits" << num_hits << std::endl;
  for(int idx = 0; idx < int(num_hits); idx++)
  {
    sphoton hit;
    sev->getHit(hit, idx);
    G4ThreeVector position     = G4ThreeVector(hit.pos.x, hit.pos.y, hit.pos.z);
    G4ThreeVector direction    = G4ThreeVector(hit.mom.x, hit.mom.y, hit.mom.z);
    G4ThreeVector polarization = G4ThreeVector(hit.pol.x, hit.pol.y, hit.pol.z);
    int theCreationProcessid;
    if(OpticksPhoton::HasCerenkovFlag(hit.flagmask))
    {
      theCreationProcessid = 0;
    }
    else if(OpticksPhoton::HasScintillationFlag(hit.flagmask))
    {
      theCreationProcessid = 1;
    }
    else
    {
      theCreationProcessid = -1;
    }
    LSDetectorHit* newHit = new LSDetectorHit();
    //hit->SetTrackID(trackID);
    newHit->SetTime(hit.time);
    newHit->SetWavelength(hit.wavelength);
    newHit->SetFromCerenkov(theCreationProcessid);
    newHit->SetReemission(theCreationProcessid);
    newHit->SetOriginalOP(theCreationProcessid);
    newHit->SetGlobalPosX(hit.pos.x);
    newHit->SetGlobalPosY(hit.pos.y);
    newHit->SetGlobalPosZ(hit.pos.z);

    fHitsCollection->insert(newHit);
    bool fDebug = false;
    if(fDebug)
    {
      G4cout << " Process ID: " << theCreationProcessid << " PhotonSD  pos.:" << hit.pos.x << "  "
             << hit.pos.y << "  "
             << "  " << hit.pos.z << "  mom.:  " << hit.mom.x << "  " << hit.mom.y << "  "
             << hit.mom.z << "  pol.:  " << hit.pol.x << "  "
             << "  " << hit.pol.y << "  " << hit.pol.z << " iiindex: " << hit.iindex << "  "
             << "  wavel.:  " << hit.wavelength << "  time:  " << hit.time
             << "  boundary flag:  " << hit.boundary_flag << "  identy:  " << hit.identity
             << "  orient_idx: " << hit.orient_idx << "  flagmask:  " << hit.flagmask << G4endl;
    }
  }
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

    analysis -> analyseTotNPE(nofHits);
    analysis -> analyseCerNPE(nofCerHits);
    analysis -> analyseSctNPE(nofSctHits);
}
#endif
