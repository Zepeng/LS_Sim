#ifndef ContinuousSpectrumManager_h
#define ContinuousSpectrumManager_h

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class ContinuousSpectrumManager
{
    public:
        ContinuousSpectrumManager();
        virtual ~ContinuousSpectrumManager();


        static ContinuousSpectrumManager* getInstance();

        static ContinuousSpectrumManager* GetInstance()
        {
            if (ContinuousSpectrumManager::fInstance == NULL)
                ContinuousSpectrumManager::fInstance = new ContinuousSpectrumManager();
            return ContinuousSpectrumManager::fInstance;
        }

        void BeginOfEventAction(const G4Event *evt);
        void EndOfEventAction(const G4Event *evt);

        void BeginOfRunAction();
        void EndOfRunAction();

        void SetFilename(G4String val)    { filename = val;}
        void SetTreename(G4String val)    { treename = val;}
        void SetActivate(G4bool val)      { is_activated = val; } 
        G4bool GetActivate()              { return is_activated; }
        void SetMaxEvt(G4int val)         { nMaxEvt = val; }
        G4int GetMaxEvt()                 { return nMaxEvt; }
        void SetStartEvtId(G4double val)  { nStartEvt = int(val);}

        void LoadContinuousSpectrum();


        G4double GetEdep(G4int evtId)         { return edep[evtId];}

    private:
       
        G4bool   is_activated;
        G4String filename;
        G4String treename;
        
        G4int nStartEvt = 0;
        G4int nMaxEvt = 100;
        G4double edep[100];
        
        static ContinuousSpectrumManager* fInstance;

};


#endif
