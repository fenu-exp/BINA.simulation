#include "Bina_SteppingAction.hh"
#include "Bina_EventAction.hh"

#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

//#include "G4TouchableHandle.hh"
#include "Bina_PhysicsMessenger.hh"
#include "Bina_PrimaryGeneratorAction.hh"
#include "G4UnitsTable.hh"

#include <cmath>
#include "CLHEP/Random/RandGauss.h"
#include <fstream>
#include "G4ios.hh"
std::ofstream file,file2,file3,file4,file5;
Bina_SteppingAction::Bina_SteppingAction(Bina_PhysicsList* myPL, Bina_EventAction* myEvt, bool rt)
  : myPhysicsList(myPL), myEvent(myEvt)
{
	root = rt;
  file_types=0;//myPL->GetFileOutputs();
    //file4.open("./Bina_outang.dat");
        file5.open("./Bina_output.dat");/*
  if (file_types&1)
    file.open("./Bina_out1.dat");
  if (file_types&2)
    file2.open("./Bina_out2.dat");
  if (file_types&4)
    file3.open("./Bina_out3.dat");*/
  int i;
  static double en_t[3],theta_t[3],phi_t[3],pos_t[3];

  for (i=0;i<3;i++)
  {
    en_t[i] = 0.;
    theta_t[i] = 0.;
    phi_t[i] = 0.;
    pos_t[i] = 0.;
  }
  theta = &theta_t[0];
  phi = &phi_t[0];
  energy = &en_t[0];
  position = &pos_t[0];
  energy_broadening=myPL->GetBroadening();
}

Bina_SteppingAction::~Bina_SteppingAction()
{
 // if (file)  file.close();
  //if (file2) file2.close();
  //if (file3) file3.close();
  //file4.close();
   file5.close();
}

void Bina_SteppingAction::UserSteppingAction(const G4Step * theStep)
{
  G4Track * theTrack = theStep->GetTrack();

  static int once = 1, ilosc = 0, bound=0;//,ilosc2 = 0,
//          first=1,   firsts = 1, bound = 0, next = 0,;//, ebound = 0;
  static G4double tab[25], tab2[25], tab3[10], secProtEnergy,secProtDetNr;
  int i;
  double startEnergy, npd_choice;
  static G4String prevParentName;
  static int index0=0, index1=0;

  G4ParticleDefinition * ParticleType = theTrack->GetDefinition();
  G4StepPoint * thePrePoint = theStep->GetPreStepPoint();
  G4VPhysicalVolume * thePrePV = thePrePoint->GetPhysicalVolume();
  G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume * thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePrePVname = thePrePV->GetName();
  G4int thePreCopyNo = thePrePV->GetCopyNo();
  G4String thePostPVname;
  G4int thePostCopyNo = 0;
    if(thePostPV !=0 ){
      thePostPVname = thePostPV->GetName();
      thePostCopyNo = thePostPV->GetCopyNo();
    }

  G4int SNumber = theTrack->GetCurrentStepNumber();  // step number

  static G4int SNumberPrev = 0;


  static G4String theLastPVname = "0";
  static G4int theLastCopyNo = 0;
/*  if(thePrePVname(0,12)=="Delta_E_BC40"||thePrePVname(0,7)=="Salad_B"){
  thePreCopyNo = renumer[thePreCopyNo]; }
  if(thePostPVname(0,12)=="Delta_E_BC40"||thePostPVname(0,7)=="Salad_B"){
  thePostCopyNo = renumer[thePostCopyNo]; }
  *//*
   tab[0] - event number
   tab[1] - particle type
   tab[2] - number of particles crossing MWPC (per 1 primary)
   tab[3] - number of particles in DeltaE (per 1 primary)
   tab[4] - number of particles in E (per 1 primary)
   tab[5] - x position on MWPC
   tab[6] - y position on MWPC
   tab[7] - energy deposited in DeltaE1
   */
  // check if it is alive and deposits still some energy
  if (theTrack->GetTrackStatus()!=fAlive&& theStep->GetTotalEnergyDeposit()==0)
  {
    return;
  }
  if(theTrack->GetParentID()==0)
  {

      if ((SNumber < SNumberPrev))
    {
//      first = 1;
//      firsts = 1;
      bound = 0;
//      ebound = 0;

      if (tab[2] || tab[3] || tab[4]) //tutaj chyba sprawdzana jest akceptancja?
      {
      // rozmycie energii deponowanej w Saladzie (uwzglednienie zdolnosci rozdzielczej)

        G4double sigma_res, edet;

        if (tab[11]>=0.001&&energy_broadening>0)
        {
          sigma_res = 0.14*sqrt(tab[11])+0.07;
          edet = CLHEP::RandGauss::shoot(tab[11],sigma_res);
          //edet = RandomGauss(aj1r1,tab[11],sigma_res);

          if (edet<0) edet = 0;
          tab[11] = edet;
        }


        if (tab[13]>=0.001&&energy_broadening>0){
          sigma_res = 0.14*sqrt(tab[13])+0.07;
          edet = CLHEP::RandGauss::shoot(tab[13],sigma_res);
          //edet = RandomGauss(aj1r2,tab[13],sigma_res);
          if (edet<0) edet = 0;
          tab[13] = edet;
        }

    }
    //TEST

        if(tab[1]>0){
		 file5<<tab[0]<<" "<<tab[1]<<" "<<tab[5]<<" "<<tab[6]<<" "<<tab3[5]<<" "<<tab3[4]<<" "<<tab3[3]<<" "<<tab[11]<<" "<<tab[7]<<" "<<tab[12]<<" "<<tab[8]<<" "<<Bina_PrimaryGeneratorAction::Geta1()<<" "<<Bina_PrimaryGeneratorAction::Getb1()<<" "<<Bina_PrimaryGeneratorAction::Getb2()<<" "<<tab[2]<<" "<<tab[4]<<" "<<tab[3]<<"   ";
		 if (root)
		 {
			 myEvent->AddHits(tab[1],tab[5],tab[6],tab3[5],tab3[4],tab3[3],tab[11],tab[7],tab[12],tab[8],position[0],position[1],position[2],tab[2],tab[4],tab[3],tof_e,tof_de);
		 }
		 counter++;
		 if (true)//(counter==3)
		 {
			 file5<<G4endl;
			 counter = 0;
		 }
		}
    //TEST
  SNumberPrev = 0;
  tab[2]=tab[3]=tab[4]=0;
  tof_e=0;
tof_de=0;
  }

  else SNumberPrev = SNumber;

  if (ParticleType == G4Neutron::NeutronDefinition()) {tab[1] = 1.;}//test[1] = 1.;}		//neutron
  else if (ParticleType == G4Deuteron::DeuteronDefinition()) {tab[1] = 3.;}//test[1] = 3.;}	//deuteron
  else if (ParticleType == G4Proton::ProtonDefinition()) {tab[1] = 2.;}//test[1] = 2.;}		//proton
  else tab[1] = 0.;
  }



  if (tab[1])
  {

    if(thePrePVname(0,8)=="Target_p"&&theTrack->GetParentID()==0)
    {
      theLastPVname = thePrePVname;

      npd_choice = Bina_PrimaryGeneratorAction::GetChoice();
      startEnergy = theTrack->GetVertexKineticEnergy();

      energy = Bina_PrimaryGeneratorAction::GetStartEnergy();
      phi = Bina_PrimaryGeneratorAction::GetStartAnglePhi();
      theta = Bina_PrimaryGeneratorAction::GetStartAngleTheta();
      position = Bina_PrimaryGeneratorAction::GetStartPosition();

      i = 0; // keep the compiler quiet
      if (npd_choice < 0) i=0;
      else if (npd_choice < 2)
      {
        if (tab[1]==2) i=0;
	else if (tab[1]==3) i=1;
	else G4cout << "Unknown particle in StepingAction ... tab[1] = "<<tab[1]<<G4endl;
      }
      else if (npd_choice == 2 || npd_choice == 3)
      {
        if      (fabs(energy[0] - startEnergy)<0.001) i = 0;
	else if (fabs(energy[1] - startEnergy)<0.001) i = 1;
        else if (fabs(energy[2] - startEnergy)<0.001) i = 2;
	else G4cout << std::setw(24) <<"error -> " <<startEnergy<<G4endl;
      }
      else G4cout << std::setw(14) <<"error ***"<<G4endl;

      for (int j=0;j<3;j++) tab3[j] = position[j];
      tab3[3] = energy[i];
      tab3[4] = phi[i];
      tab3[5] = theta[i];
    }
    if(thePrePVname(0,8)!="Target_p"&& theLastPVname(0,8)=="Target_p"&&
       theTrack->GetParentID()==0)
    {
      for (i=0;i<17;i++) tab[i] = 0.;
      for (i=0;i<10;i++) tab2[i] = 0.;
      secProtEnergy=0.;
      secProtDetNr=0;
      once = 1;
      ilosc = 0;
//      ilosc2 = 0;
//      next = 1;			 // jesli utknelo w tarczy !!!!!!!!!!!!!!!!!!!
      tab[0] = Bina_EventAction::getNb();
      theLastPVname = thePrePVname;
    }
  // check if it is entering to the MWPC1 volume
    if(thePrePVname(0,11)=="Mwpc1_slice"&&theTrack->GetParentID()==0)
      {
       if ((thePreCopyNo == 5)&&(once == 1)) //plane Y
	 {
	tab[5] = theTrack->GetPosition().x();
	tab[6] = theTrack->GetPosition().y();
        //cout << theTrack->GetPosition().z() << endl;  // wypis na ekran pozycji z
	tab2[0] = theTrack->GetKineticEnergy(); //energy  in
	theLastPVname = thePrePVname;
      }
      tab2[1] = theTrack->GetKineticEnergy();  //energy  out
    }
    if(thePrePVname(0,11)!="Mwpc1_slice"&&theTrack->GetParentID()==0&&
       theLastPVname(0,11)=="Mwpc1_slice")
    {
      once = 0;
      tab[2]++;
      theLastPVname = thePrePVname;
    }

///////////////////////
///   Find boundary!
///////////////////////

    if(thePostPoint->GetStepStatus() == fGeomBoundary) bound = 1;

///////////////////////
///   dE   dE   dE   dE
///////////////////////

  if(thePostPVname(0,7)=="DeltaE_" || thePrePVname(0,7)=="DeltaE_") // step in DeltaE
    //    if(thePrePVname(0,7)=="DeltaE_") // full step in DeltaE
    {
      if (bound==1
		  &&   (thePrePVname(0,7)!="DeltaE_" // first step in this DeltaE
		||thePreCopyNo != theLastCopyNo ))
	{
	  if(theTrack->GetParentID()==0)  //only primaries!
	    {
		{
		  if(tab[3]==0||(tab[3]==1&&thePreCopyNo!= theLastCopyNo ))tab[3]++;
		  if(tab[3]==2)ilosc=1;
		}
	      if(thePrePVname(0,7)=="DeltaE_")tab[8 + 2*ilosc] = thePreCopyNo;
	      if(thePrePVname(0,7)!="DeltaE_")tab[8 + 2*ilosc] = thePostCopyNo;
	      tab2[2 + 2*ilosc] = theTrack->GetKineticEnergy();
	      bound = 0;
		  if(ilosc==0) tof_de=thePostPoint->GetLocalTime();

	    }
	}
      //      if(thePostPoint->GetStepStatus() == fGeomBoundary)
      if(theTrack->GetParentID()==0)
	{       //only primaries!
	  tab2[3 + 2*ilosc] = theTrack->GetKineticEnergy();
	  theLastPVname = thePrePVname;
	  theLastCopyNo = thePreCopyNo;
	}
      if(thePrePVname(0,7)=="DeltaE_")tab[7 + 2*ilosc] += theStep->GetTotalEnergyDeposit();

    }
///////////////////////
///   E   E   E   E
///////////////////////

/*   if(thePrePVname(0,5)=="EDet_"&&theTrack->GetParentID()==0) {
     if(thePostPVname(0,5)!="EDet_") {
        tab[12]=thePreCopyNo;
        tab2[6]=theTrack->GetKineticEnergy();}
     else {
        tab[13]=thePreCopyNo;
        tab2[7]=theTrack->GetKineticEnergy();}}
*/
/*if(thePrePVname!=thePostPVname) {G4cout<<thePrePVname<<" kopia nr "<<thePreCopyNo<<' '<<thePostPVname<<" kopianr"<<thePostCopyNo
<<' '<<SNumber<<'\n';}*/

    if(thePostPVname(0,5)=="EDet_" || thePrePVname(0,5)=="EDet_") // step in E
      {
	// first step in the given E
	if (bound==1
	  && (thePrePVname(0,5)!="EDet_" ||thePreCopyNo != theLastCopyNo))
	  {
	    if(theTrack->GetParentID()==0)   //only primaries!
	      {
		{
		  if(tab[4]==0||(tab[4]==1&&thePreCopyNo!= theLastCopyNo ))
		    tab[4]++;
		  if(tab[4]==2)ilosc=1;
		}
		if(thePrePVname(0,5)=="EDet_")tab[12 + 2*ilosc] = thePreCopyNo;
		if(thePrePVname(0,5)!="EDet_")tab[12 + 2*ilosc] = thePostCopyNo;
		tab2[6 + 2*ilosc] = theTrack->GetKineticEnergy();
		bound = 0;
		if(ilosc==0) tof_e=thePostPoint->GetLocalTime();

	      }
	  }
	if(theTrack->GetParentID()==0) //only primaries!
	  {
	    tab2[7 + 2*ilosc] = theTrack->GetKineticEnergy();
            theLastPVname = thePrePVname;
            theLastCopyNo = thePreCopyNo;
	  }
        if(thePrePVname(0,5)=="EDet_")tab[11 + 2*ilosc] += theStep->GetTotalEnergyDeposit();

        //G4cout<<ParticleType->GetParticleName()<<G4endl;
        if(theTrack->GetParentID()>0 && prevParentName(0,7)=="neutron" && ParticleType == G4Proton::ProtonDefinition())
          {
	     if(thePrePVname(0,5)=="EDet_")secProtDetNr = thePreCopyNo;
             if(thePrePVname(0,5)!="EDet_")secProtDetNr = thePostCopyNo;
             secProtEnergy+=theStep->GetTotalEnergyDeposit();
          }
      }
    //////////////////////////////////////////////////////////////

    // then suspend the track
    theTrack->SetTrackStatus(fSuspend);

  }
 //  G4cout<<tab[11]<<' '<<secProtEnergy<<G4endl;
  //  tab[11+2*ilosc]+=secProtEnergy;
  if (theTrack->GetParentID()==0) prevParentName=ParticleType->GetParticleName();
  theTrack->SetTrackStatus(fSuspend);
}
