#ifndef LSSteppingAction_h
#define LSSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "LSTrackingAction.hh"

class LSEventAction;
class LSTrackingAction;

class LSSteppingAction : public G4UserSteppingAction
{
  private:
    unsigned int Photoncounter{ 0 };
    unsigned int GenStepcounter{ 0 };
  
  public:
    LSSteppingAction();
    ~LSSteppingAction() override;
    
    // method from the base class
    void UserSteppingAction(const G4Step*) override;
    inline void ResetPhotoncounter() { Photoncounter = 0; }
    inline void ResetGenStepcounter() { GenStepcounter = 0; }
};

#endif
