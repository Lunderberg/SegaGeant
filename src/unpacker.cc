#include <iostream>
#include <cstdio>

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

int main() {
  FILE* input_file = fopen("binOutput.bin","rb");

  while(true) {
    Data data;
    size_t bytes_read = fread(&data.num_entries, sizeof(data.num_entries), 1, input_file);
    if(bytes_read == 0) {
      break;
    }

    
    fread(&data.dataEntry, sizeof(DataEntry), data.num_entries, input_file);

    print_data(data);
  }

  fclose(input_file);
}
