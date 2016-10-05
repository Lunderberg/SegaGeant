#include <iostream>
#include <cstdio>

#include "TFile.h"
#include "TH1.h"
#include "TRandom.h"

#include "DataOutputFormat.hh"

void print_sega_data(SegaData& data) {
  std::cout << "---------------------------------\n";
  std::cout << "SeGa Event:\n";
  std::cout << "Num entries: " << data.num_entries << "\n";

  for(int i=0; i<data.num_entries; i++) {
    std::cout << "\t" << data.dataEntry[i].E << " keV deposited at "
	      << "(" << data.dataEntry[i].x << ", "
	      << data.dataEntry[i].y << ", "
	      << data.dataEntry[i].z << ")\n";
  }
  std::cout << std::flush;
}

void print_JANUS_data(JANUSData& data) {
  std::cout << "---------------------------------\n";
  std::cout << "JANUS Event:\n";
  std::cout << "Num entries: " << data.num_entries << "\n";

  for(int i=0; i<data.num_entries; i++) {
    std::cout << "\t" << data.dataEntry[i].E << " keV deposited at "
              << "(" << data.dataEntry[i].x << ", "
              << data.dataEntry[i].y << ", "
              << data.dataEntry[i].z << ")\n";
  }
  std::cout << std::flush;
}

int main(int argc, char** argv) {
  if(argc < 3) {
    std::cerr << "Usage: unpacker INPUT_FILE OUTPUT_FILE" << std::endl;
    return 1;
  }

  const char* input_filename = argv[1];
  const char* output_filename = argv[2];

  FILE* input_file = fopen(input_filename,"rb");
  TH1* gamma_energy = new TH1F("gamma_energy","gamma_energy",
			       4000, 0, 4000);
  TH1* janus_energy = new TH1F("janus_energy","janus_energy",
			       40000, 0, 400e3);

  TH1* JANUS_Etot = new TH1F("Summed JANUS Energy", "Summed JANUS Energy", 40000, 0,400e3);

  TH1* SeGA_Etot = new TH1F("Summed SeGA Energy", "Summed SeGA Energy", 4000, 0, 4000); 

  while(true) {
    Header header;
    size_t bytes_read = fread(&header, header.bytes(), 1, input_file);
    if(bytes_read == 0) {
      break;
    }
     
    double jEtot = 0;
    double sEtot = 0;	

    switch(header.type) {
    case Type::JANUS: {
      JANUSData data;
      size_t bytes_read = fread(&data.num_entries, sizeof(data.num_entries), 1, input_file);
      if(bytes_read == 0) {
	break;
      }
    
      fread(&data.dataEntry, sizeof(DataEntry), data.num_entries, input_file);

      /*if (data.num_entries > 1) {
       print_JANUS_data(data);
      }*/

      for(int i=0; i<data.num_entries; i++) {
	double energy_keV = data.dataEntry[i].E;
	janus_energy->Fill(gRandom->Gaus(energy_keV, (0.04/2.355)*energy_keV));
        jEtot += energy_keV;
      }
    }
      break;

    case Type::SeGA: {
      SegaData data;
      size_t bytes_read = fread(&data.num_entries, sizeof(data.num_entries), 1, input_file);
      if(bytes_read == 0) {
	break;
      }
    
      fread(&data.dataEntry, sizeof(DataEntry), data.num_entries, input_file);

      /*if (data.num_entries > 1) {
       print_sega_data(data);
      }*/

      //bool peakEnergy = 0;

      for(int i=0; i<data.num_entries; i++) {
	double energy_keV = data.dataEntry[i].E;
	gamma_energy->Fill(gRandom->Gaus(energy_keV, (0.04/2.355)*energy_keV));
        sEtot += energy_keV;
        //if (data.dataEntry[i].E > 35.0 && data.dataEntry[i].E < 52.0) {
         //peakEnergy = 1;
        //}
      }
      //if (peakEnergy) 
       //{printf("\n %i entries in an event with energy deposited in the 50 keV peak \n", data.num_entries);}
    }
      break;

    default:
      break;
    }

    if (jEtot != 0) {
     JANUS_Etot->Fill(gRandom->Gaus(jEtot, (0.04/2.355)*jEtot));
    }    
   
   if (sEtot != 0) {
    SeGA_Etot->Fill(gRandom->Gaus(sEtot, (0.04/2.355)*sEtot));
   }
   
  }

  fclose(input_file);

  TFile* output_file = new TFile(output_filename,"RECREATE");
  gamma_energy->Write();
  janus_energy->Write();
  SeGA_Etot->Write();
  JANUS_Etot->Write();
  output_file->Close();
}
