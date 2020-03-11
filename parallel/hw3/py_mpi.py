from mpi4py import MPI
print("my rank is %d" % MPI.COMM_WORLD.Get_rank())
