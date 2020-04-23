# Unix Shell
SHELL = /bin/bash

# Libraries
DEBUG = -O3

# Compiler flags and variables
OCCA_INC = -I/ccs/home/belloma2/Programs/OCCA/Master/include
OCCA_LIB = -L/ccs/home/belloma2/Programs/OCCA/Master/lib -locca

CXX = g++
CXX_FLAGS = -Wall
CXX_INCLUDES = -I./ $(OCCA_INC)
CXX_LIBRARIES = $(OCCA_LIB) -lm

FC = gfortran
FC_FLAGS = -fdefault-real-8
FC_INCLUDES =
FC_LIBRARIES = -lgfortran -lm

LD_FLAGS = $(CXX_FLAGS)

# Source files
SRCS_CXX = fast_diagonalization.cpp fdm_operator.cpp utilities.cpp timer.cpp
SRCS_FC = mxm.f
OBJS = $(SRCS_CXX:.cpp=.o) $(SRCS_FC:.f=.o)

# Executable
EXE = fast_diagonalization

# Make rules
all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(DEBUG) $(LD_FLAGS) -o $@ $(OBJS) $(CXX_LIBRARIES)

%.o: %.cpp
	$(CXX) $(DEBUG) $(CXX_FLAGS) $(CXX_INCLUDES) -c $< -o $@

%.o: %.f
	$(FC) $(DEBUG) $(FC_FLAGS) $(FC_INCLUDES) -c $< -o $@

clean:
	rm *.o $(EXE);
