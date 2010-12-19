# Makefile for the socket programming example
#

EXECUTABLE  := distributed_cuda
# Cuda source files (compiled with cudacc)
CUFILES   := vectorAdd.cu
# # C/C++ source files (compiled with gcc / c++)
CCFILES   := \
  Socket.cpp \
  ClientSocket.cpp \
  ServerSocket.cpp \
  simple_client_main.cpp \
#
# ################################################################################
# # Rules and targets
#
include ../../common/common.mk
#
