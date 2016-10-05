#include <iostream>
#include <cstdio>

#include "TFile.h"
#include "TH1.h"
#include "TRandom.h"

#include "DataOutputFormat.hh"

void print_data(Data& data) {
  std::cout << "---------------------------------\n";
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
  TH1* hist = new TH1F("gamma_energy","gamma_energy",
  		       4000, 0, 4000);

  while(true) {
    Data data;
    size_t bytes_read = fread(&data.num_entries, sizeof(data.num_entries), 1, input_file);
    if(bytes_read == 0) {
      break;
    }

    
    fread(&data.dataEntry, sizeof(DataEntry), data.num_entries, input_file);

    for(int i=0; i<data.num_entries; i++) {
      //      std::cout << data.dataEntry[i].E << std::endl;
      double energy_keV = data.dataEntry[i].E;
      hist->Fill(gRandom->Gaus(energy_keV, (0.04/2.355)*energy_keV));
    }

        print_data(data);
  }

  fclose(input_file);

  TFile* output_file = new TFile(output_filename,"RECREATE");
  hist->Write();
  output_file->Close();
}
