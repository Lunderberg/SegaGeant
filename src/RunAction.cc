#include "DataOutputFormat.hh"
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "PrimaryGeneratorAction.hh"
#include "DataOutput.hh"
#include "Doppler.hh"
#include "EventAction.hh"
#include "RunMessenger.hh"

#include <stdio.h>
#include <iostream>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DataOutput* dat, Doppler* dop)
{
	data = dat;
	doppler = dop;
	zRes = 0;
	rRes = 0;
	gaussianRand = new CLHEP::RandGauss((new CLHEP::RanecuEngine));
        binDir = "";
        binFile = "binOutput.bin";
        runMessenger = new RunMessenger(this);
}

RunAction::~RunAction()
{
	delete gaussianRand;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
       if (binDir == "")
        { 
         path = binFile;
        }

       else { 
        path = binDir + "/" + binFile;
       }
  
	binOutput = fopen(path.c_str(),"wb");
        if (!binOutput) 
         {
          printf("Error: Could not open file \"%s\"\n", path.c_str());
         }

        G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	for(i=0;i<CHANNELS;i++)
	{
		totalSpectrum[i] = 0;
		for(j=0;j<16;j++)
		{
			segmentSpectrum[j][i] = 0;
			totalEbySeg[j][i] = 0;
		}
	}
	for(i=0;i<16;i++)
	{
		totalCountsSeg[i] = 0;
	}
	totalCounts = 0;

	E_per_channel = 1.0*keV;
	mm_per_channel = 4.0*mm;
	rad_per_channel = 36*deg;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::fillPerEvent(std::map<int,CrystalEntry>& crystalInfo, G4int evtNb) {
  	
        JANUSData jOutput;
        SegaData sOutput;

        jOutput.num_entries = 0;
        sOutput.num_entries = 0;
       
	for(auto& entry : crystalInfo) {
	 if (entry.second.detector == JANUS) {
                jOutput.dataEntry[jOutput.num_entries] = {entry.second.energy/keV, 
		                        entry.second.x/mm, 
		                        entry.second.y/mm, 
		                        entry.second.z/mm};
                jOutput.num_entries++;
	 }

         if (entry.second.detector == SeGA) {
                sOutput.dataEntry[sOutput.num_entries] = {entry.second.energy/keV,
                                        entry.second.x/mm,
                                        entry.second.y/mm,
                                        entry.second.z/mm};
                sOutput.num_entries++;
         } 

        }

        if (jOutput.num_entries != 0) {
         Header jHeader;
         jHeader.type = JANUS;
         jHeader.size = jOutput.bytes();
         jHeader.evtNb = evtNb;

         fwrite(&jHeader, jHeader.bytes(), 1, binOutput);
         fwrite(&jOutput, jOutput.bytes(), 1, binOutput);
        }


        if (sOutput.num_entries != 0) {
         Header sHeader;
         sHeader.type = SeGA;
         sHeader.size = sOutput.bytes();
         sHeader.evtNb = evtNb;
       
         fwrite(&sHeader, sHeader.bytes(), 1, binOutput);
         fwrite(&sOutput, sOutput.bytes(), 1, binOutput);
        }
 }

void RunAction::fillPerEvent(G4double ECrys,G4double LCrys, G4int hits[], G4double energy[], G4double X[], G4double Y[], G4double Z[])
{
	i=0;
	maxSeg=0;
	maxSegE=0.0;

	// Increment the individual segment spectra.
	// Detector id is Ring,detector,zsegment,phisegment,rsegment.
	// Take segment numbers to be 0-7 for ring 1
	// and 8-15 for ring 2.
	while(hits[i]!=0)
	{
		segment = (hits[i]%10000)/1000;
		if((hits[i]/100000)==2)
		{
			segment+=8;
		}

		if(maxSegE<energy[i])
		{
			maxSegE=energy[i];
			maxSeg=segment;
			tagDetector=i;
		}
		segmentCounts[segment]++;

		j=0;
		while(energy[i]/keV > j*(E_per_channel/keV))
		{j++;}
		segmentSpectrum[segment][j]++;
		i++;
	}

	//increment the doppler-reconstructed summed spectrum
	totalCounts++;
	totalCountsSeg[maxSeg]++;
	doppler->SetDetNo(hits[tagDetector]);
	x = X[tagDetector];
	y = Y[tagDetector];
	z = Z[tagDetector];

	if(zRes<=0) // Standard segment spatial resolution
	{
		E_Doppler = doppler->Correct(ECrys);
		//E_Doppler = ECrys;
	}
	else  // Custom spatial resolution
	{
		zRand = gaussianRand->fire(z,zRes);
		rRand = gaussianRand->fire(sqrt(x*x+y*y),rRes);
		//    E_Doppler = doppler->Correct(ECrys,zRand,rRand);
		//    E_Doppler = doppler->Correct(ECrys,zRand);
	}

	//  i=0;
	//  while(E_Doppler > i*E_per_channel)
	//    {i++;}
	i=int(E_Doppler/E_per_channel);
	totalSpectrum[i]++;
	totalEbySeg[maxSeg][i]++;


	// Increment E vs R and E vs Phi matrices
	/* i=0;
	   while(ECrys > i*E_per_channel)
	   {i++;}
	   j=0;
	   while(sqrt(x*x+y*y)<j*mm_per_channel)
	   {j++;}
	   G4cout << "x,y: " << x << "," << y << " R: " << sqrt(x*x+y*y)/cm << "\tPhi: " << atan(y/x)/deg << G4endl;
	   E_vs_R[i][j]++;
	   j=0;
	   G4double phi = atan(x/y);
	   if (phi<0)
	   {phi+=360*deg;}
	   while(atan(y/x)>j*rad_per_channel)
	   {j++;}
	   E_vs_Phi[i][j]++;
	 */

	//create array of DataEntry structures, fill it
	//put array of DataEntry structures into a Data structure
	
        /*Data output;

	int k  = 0;
	while(X[k] != 0)
	{
		output.dataEntry[k] = {energy[k]/keV, X[k]/mm, Y[k]/mm, Z[k]/mm};
		k++;
	}

	//k = num_entries
	output.num_entries = k;

	fwrite(&output, output.bytes(), 1, binOutput);*/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	// if(data->GetTotalEVerbose()) // write one spectrum for all segments
	// {
	data->WriteSpectrum(totalSpectrum, CHANNELS, totalCounts, E_per_channel/keV, "sp");
	//  }
	//  data->Write2DSpectrum(E_vs_R, CHANNELS, 10, "evsr");
	//  data->Write2DSpectrum(E_vs_Phi, CHANNELS, 10, "evsp");

	//  if(data->GetSegVerbose())
	//  {
	for(i=0;i<16;i++)
	{
		std::stringstream num;
		num << i;
		//    name = "seg" + num.str();
		// write uncorrected spectra for each segment
		//    data->WriteSpectrum(segmentSpectrum[i], CHANNELS, segmentCounts[i], E_per_channel/keV, name);
		name = "tot_seg" + num.str();
		// write total energy spectra gated on segment
		data->WriteSpectrum(totalEbySeg[i], CHANNELS, totalCountsSeg[i], E_per_channel/keV, name);
	}
	//  }

	G4int NbOfEvents = aRun->GetNumberOfEvent();
	if (NbOfEvents == 0) return;

        fclose(binOutput);
}

void RunAction::SetBinFile(G4String f)
{
 binFile = f;
 G4cout << "Setting binary output file name to: " << f << G4endl; 
}

void RunAction::SetBinDir(G4String d)
{
 binDir = d;
 G4cout << "Setting binary output directory to: " << d << G4endl;
}



