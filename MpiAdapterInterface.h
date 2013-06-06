#ifndef __MPI_ADAPTER_INTERFACE_H
#define __MPI_ADAPTER_INTERFACE_H

#include <mpi.h>

/*
 * Wrap the C interface to MPI.
*/
class MpiAdapterInterface
{
public:
	virtual void MpiSend(const void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) const = 0;
};

#endif // __MPI_ADAPTER_INTERFACE_H