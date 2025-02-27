#ifndef LSRunAction_h
#define LSRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class LSRunAction : public G4UserRunAction
{
    public:
        LSRunAction();
        virtual ~LSRunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void   EndOfRunAction(const G4Run*);

        void SetRndmFreq(G4int val) { fSaveRndm = val; }
        G4int GetRndmFreq()         { return fSaveRndm;}

        inline void SetAutoSeed(const G4bool val) {fAutoSeed = val;}
        
    private:
        G4int  fSaveRndm;
        G4bool fAutoSeed;

};

#endif
