#ifndef LSEventAction_h
#define LSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <ctime>

class LSEventAction : public G4UserEventAction
{
    public:
        LSEventAction();
        virtual ~LSEventAction();

        virtual void BeginOfEventAction(const G4Event* event);
        virtual void   EndOfEventAction(const G4Event* event);
	
    private:
	clock_t m_end_t;
	clock_t m_start_t;

};

#endif
