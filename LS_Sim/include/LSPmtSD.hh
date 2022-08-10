#ifndef LSPmtSD_h
#define LSPmtSD_h

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"
#include "LSDetectorHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class LSPmtSD : public G4VSensitiveDetector
{
    public:
        LSPmtSD(G4String name);
        virtual ~LSPmtSD();

        virtual void Initialize(G4HCofThisEvent* );
        virtual G4bool ProcessHits(G4Step*, 
                                   G4TouchableHistory* );

        virtual void EndOfEvent(G4HCofThisEvent* );
        virtual void clear();

        void DrawAll();
        void PrintAll();

    //Initialize the arrays to store pmt possitions
    inline void InitPMTs(){
      if(fPMTPositionsX)delete fPMTPositionsX;
      if(fPMTPositionsY)delete fPMTPositionsY;
      if(fPMTPositionsZ)delete fPMTPositionsZ;
      fPMTPositionsX = new G4DataVector();
      fPMTPositionsY = new G4DataVector();
      fPMTPositionsZ = new G4DataVector();
    }

    //Store a pmt position
    void SetPmtPositions(const std::vector<G4ThreeVector>& positions);

  private:

    LSDetectorHitsCollection* fPMTHitCollection;

    G4DataVector* fPMTPositionsX;
    G4DataVector* fPMTPositionsY;
    G4DataVector* fPMTPositionsZ;
        
};

#endif






