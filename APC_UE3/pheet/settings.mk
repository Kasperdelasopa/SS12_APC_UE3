
OPTIMIZATION = -O3
CXXFLAGS = $(OPTIMIZATION) -Wall -fmessage-length=0 -std=c++0x 
CXX = g++

LIB_PATH = -L/opt/gcc-4.7/lib/ -L/opt/gcc-4.7/lib64/ -L/opt/hwloc-1.4/lib/
INCLUDE_PATH = -I. -I/opt/hwloc-1.4/include/


TEST_LIBS = 
# Needed for lupiv test
#TEST_LIBS =	$(LIBS)  -lblas -llapack

