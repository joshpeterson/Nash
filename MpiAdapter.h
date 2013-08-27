#ifndef __MPI_ADAPTER_H
#define __MPI_ADAPTER_H

#include <mpi.h>
#include "MpiAdapterInterface.h"

class MpiAdapter : public MpiAdapterInterface
{
public:
	virtual int MpiCommRank(MPI_Comm comm) const
	{
		int rank;
		MPI_Comm_rank(comm, &rank);

		return rank;
	}

	virtual void MpiSend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) const
	{
		MPI_Send(buf, count, datatype, dest,tag,comm);
	}

	virtual void MpiRecv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status) const
	{
		MPI_Recv(buf, count, datatype, source, tag, comm, status);
	}
};

#endif // __MPI_ADAPTER_H