#ifndef DATAOUTPUTFORMAT
#define DATAOUTPUTFORMAT 
#include <cstring>

enum Type
{
 JANUS,
 SeGA,
 None,
};

struct Header
{
       Type type;
       int size, evtNb;

       size_t bytes() {
        return 2*(sizeof(int)) + sizeof(Type);
       }        
}__attribute__((__packed__));

struct DataEntry
{
	double E, x, y, z;
};

struct SegaData
{
	int num_entries;
	DataEntry dataEntry[100];

	size_t bytes()
	{
		return sizeof(int) + num_entries*sizeof(DataEntry);
	}
}__attribute__((__packed__));

struct JANUSData
{
        int num_entries;
        DataEntry dataEntry[100];

        size_t bytes()
        {
                return sizeof(int) + num_entries*sizeof(DataEntry);
        }
}__attribute__((__packed__));


#endif
