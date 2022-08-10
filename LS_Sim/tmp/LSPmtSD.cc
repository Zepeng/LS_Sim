#include "LSPmtSD.hh"

LSPmtSD::LSPmtSD(G4String name)
    : G4VSensitiveDetector(name), fHitsCollection(NULL),
    fPMTPositionsX(nullptr),
    fPMTPositionsY(nullptr),
    fPMTPositionsZ(nullptr)
{
    collectionName.insert("pmtHitCollection");
}


LSPmtSD::~LSPmtSD() {;}


void LSPmtSD::SetPmtPositions(const std::vector<G4ThreeVector&> positions)
{
    for (G4int i=0; i<G4int(positions.size()); ++i) {
        
    
    }


}












