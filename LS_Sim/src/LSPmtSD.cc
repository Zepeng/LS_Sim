#include "LSPmtSD.hh"
#include "LSDetectorHit.hh"

LSPmtSD::LSPmtSD(G4String name)
    : G4VSensitiveDetector(name),
    fPMTHitCollection(nullptr),
    fPMTPositionsX(nullptr),fPMTPositionsY(nullptr),fPMTPositionsZ(nullptr)
{
  collectionName.insert("pmtHitCollection");
}


LSPmtSD::~LSPmtSD()
{}

void LSPmtSD::SetPmtPositions(const std::vector<G4ThreeVector>& positions)
{
  for (G4int i=0; i<G4int(positions.size()); ++i) {
    if(fPMTPositionsX)fPMTPositionsX->push_back(positions[i].x());
    if(fPMTPositionsY)fPMTPositionsY->push_back(positions[i].y());
    if(fPMTPositionsZ)fPMTPositionsZ->push_back(positions[i].z());
  }
}


void LSPmtSD::Initialize(G4HCofThisEvent* hitsCE){
  fPMTHitCollection = new LSDetectorHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //Store collection with event and keep ID
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, fPMTHitCollection );
}



G4bool LSPmtSD::ProcessHits(G4Step*, G4TouchableHistory* ) {
    return false;
}



void LSPmtSD::EndOfEvent(G4HCofThisEvent*) {}

void LSPmtSD::clear() {}

void LSPmtSD::DrawAll() {}

void LSPmtSD::PrintAll() {}




