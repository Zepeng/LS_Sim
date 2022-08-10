#ifndef MyRootBasedAnalysis_h
#define MyRootBasedAnalysis_h 1

#include <iomanip>
#include "TFile.h"
#include "G4Event.hh"
#include "G4Track.hh"

class TFile;
class TTree;

class MyRootBasedAnalysis
{
    public:
        MyRootBasedAnalysis();
        ~MyRootBasedAnalysis();

        void BeginOfEventAction(const G4Event *evt);
        void EndOfEventAction(const G4Event *evt);

        void BeginOfRunAction();
        void EndOfRunAction();

        void SteppingAction(const G4Step *aStep);
        void PreTrackingAction(const G4Track *aTrack);
        void PostTrackingAction(const G4Track *aTrack);

        inline void SetFileName(G4String fname) { fFileName = fname; }
        inline void SetBirks1(G4double val)     { m_BirksConstant1 = val;}
        inline void Activated() { active = true; }
        inline void Deactivated() { active = false; }
        inline bool IsActivated() { return active; }


        double calculateQuenched(const G4Step* aStep);

    private:
        G4String fFileName;

        bool active = false;

        TFile* fRootFp;
        TTree* fTree;

        double m_BirksConstant1;
        double m_BirksConstant2;

        Int_t           pdf_id;
        Float_t         pre_x;
        Float_t         pre_y;
        Float_t         pre_z;
        Float_t         edep;
        Float_t         qedep;
        Float_t         track_length;

};



#endif
