#ifndef Bina_EventAction_h
#define Bina_EventAction_h 1

#include "G4UserEventAction.hh"
#include "g4root.hh"
#include "globals.hh"
#include <vector>

class Bina_EventAction : public G4UserEventAction
{
  private:
    std::vector <G4double> fX1vec;
    G4double  fX1, fX2, fX3, fX4;	         // Coord. on MWPC
    G4double  fY1, fY2, fY3, fY4;             // -||-
    G4int    fP1Type,fP2Type,fP3Type, fP4Type;      // Kind of particle
    G4int    fE1, fE2, fE3, fE4;             // Which E det
    G4int    fdE1, fdE2, fdE3, fdE4;           // Which dE det
    G4int    fN;                   // number of particles in event
    G4double fEn1, fEn2, fEn3, fEn4;            // Energies from vert
    G4double fEd1, fEd2, fEd3, fEd4;            // Energies dep. in E
    G4double fTh1, fTh2, fTh3, fTh4;            // Theta angles of particles (vert)
    G4double fPhi1, fPhi2, fPhi3, fPhi4;          // Phi angles of particles (vert)
    G4double  fXv, fYv, fZv;			// Vertex position
    G4double fEddE1, fEddE2, fEddE3, fEddE4; //Energy dep in dE
    G4int fFlagMWPC1, fFlagE1,fFlagdE1;
    G4int fFlagMWPC2, fFlagE2,fFlagdE2;
    G4int fFlagMWPC3, fFlagE3,fFlagdE3;
    G4int fFlagMWPC4, fFlagE4,fFlagdE4;
    G4int Part_num_n, Part_num_p;
    G4double fTOF_E1, fTOF_E2,fTOF_E3,fTOF_E4;
    G4double fTOF_dE1, fTOF_dE2,fTOF_dE3,fTOF_dE4;

  public:
    Bina_EventAction(bool);
    virtual ~Bina_EventAction();

  public:
    bool root=false;
    virtual void   BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    void AddHits(G4int Ptype, G4double X, G4double Y, G4double Th, G4double phi, G4double En, G4double Ed, G4double EddE, G4double E, G4double dE, G4double Xv, G4double Yv, G4double Zv, G4int FlagMWPC, G4int FlagE, G4int FlagdE, G4double TOF_E, G4double TOF_dE);
    std::vector<G4double>& GetfX1vec() { return fX1vec; }
    inline static int getNb(int num = -1 )
    {
      static int temp;
      if (num != -1) temp = num;
      return temp;
    };

};
#endif
