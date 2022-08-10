#ifndef LSPhysicsList_h
#define LSPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "LSPhysicsListMessenger.hh"

class G4VPhysicsConstructor;
class G4ProductionCuts;

class LSPhysicsList : public G4VModularPhysicsList
{
    public:
        LSPhysicsList();
        virtual ~LSPhysicsList();

    public:
        virtual void SetCuts();

        virtual void ConstructParticle();
        virtual void ConstructProcess();
        void ConstructOpticalProcess();

        void DoOpticalSim(G4bool enableoptical)     { m_enableoptical = enableoptical; }
        void SetYieldRatio(G4double yield)          { m_yield = yield; }

    private:
        G4VPhysicsConstructor* emPhysicsList;
        G4VPhysicsConstructor* decayPhysicsList;


    private:
        G4bool m_enableoptical;
        G4double m_yield;

        LSPhysicsListMessenger* theMessenger;
};

#endif
