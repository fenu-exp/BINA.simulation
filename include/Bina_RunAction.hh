#ifndef Bina_RunAction_h
#define Bina_RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "time.h"
#include "g4root.hh"

class G4Run;
class Bina_RunAction : public G4UserRunAction
{
  public:
    Bina_RunAction(bool);
    virtual ~Bina_RunAction();

    virtual void BeginOfRunAction(const G4Run* );
    virtual void   EndOfRunAction(const G4Run* );
    bool root=false;

    private:
    clock_t fbegin;
    clock_t fend;

};
#endif
