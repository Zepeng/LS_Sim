
#ifndef LSDetectorSD_h
#define LSDetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "LSDetectorHit.hh"
#include "LSAnalysisManager.hh"
#include "LSDetectorSDMessenger.hh"

class G4Step;
class G4HCofThisEvent;

class LSDetectorSD : public G4VSensitiveDetector
{
    public:
        LSDetectorSD(const G4String& name,
                const G4String& hitsCollectionName);
        virtual ~LSDetectorSD();

        // methods from base class
        virtual void   Initialize  (G4HCofThisEvent* hitCollection);
        virtual G4bool ProcessHits (G4Step* step, G4TouchableHistory* history);
        virtual void   EndOfEvent  (G4HCofThisEvent* hitCollection);

        void SetEfficiency(G4double eff)    { efficiency = eff;}

    private:
        LSDetectorHitsCollection* fHitsCollection;
        LSAnalysisManager* analysis;

        LSDetectorSDMessenger* theMessenger;

    private:
        G4double efficiency;
};

#endif
