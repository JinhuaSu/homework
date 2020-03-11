import mpi4py.MPI as MPI
comm = MPI.COMM_WORLD
comm_rank = comm.Get_rank()
comm_size = comm.Get_size()

data_send = [comm_rank] * 4
comm.send( data_send, dest=(comm_rank+1)%comm_size )
# 如果comm_rank-1<0,会自动加comm_size变为正数
data_recv = comm.recv( source=(comm_rank-1)%comm_size )
print( "my rank is %d, I received :" % comm_rank )
print( data_recv )

