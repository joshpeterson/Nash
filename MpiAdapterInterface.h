#ifndef __MPI_ADAPTER_INTERFACE_H
#define __MPI_ADAPTER_INTERFACE_H

#include <mpi.h>

/*
 * Wrap the C interface to MPI.
*/
class MpiAdapterInterface
{
public:
	virtual int MpiCommRank(MPI_Comm comm) const = 0;
	virtual void MpiSend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) const = 0;
	virtual void MpiRecv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status) const = 0;
};

#endif // __MPI_ADAPTER_INTERFACE_H