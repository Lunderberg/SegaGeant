#ifndef DATAOUTPUTFORMAT
#define DATAOUTPUTFORMAT 
#include <cstring>

struct DataEntry
{
	double E, x, y, z;
};

struct Data
{
	int num_entries;
	DataEntry dataEntry[100];

	size_t bytes()
	{
		return sizeof(int) + num_entries*sizeof(DataEntry);
	}
}__attribute__((__packed__));

#endif
