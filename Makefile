CXX=g++
CXXFLAGS=-O3 -g -Wall -fPIC -std=c++1y `root-config --cflags` -Iinclude -I${ROOT_UTILS_PATH}/include -I${ATLAS_PATH}/include
LDFLAGS=`root-config --glibs --ldflags` -Llib -L${ROOT_UTILS_PATH}/lib -L${ATLAS_PATH}/lib -lUtilities -lAtlasUtils -lAtlasStyle

libraries = LocalUtilities
algorithms = ZBosonReco
binaries = Process

.PHONY : libs algs bins directories clean

all : directories libs algs bins

libs : $(libraries)
algs : $(algorithms)
bins : $(binaries)

directories :
	mkdir -p bin lib obj

LocalUtilities : src/LocalUtilities.cxx
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -o lib/lib$@.so src/$@.cxx

# Main needs to be compiled into binary
Process : $(libraries) $(algorithms) src/Process.cxx
	$(CXX) $(CXXFLAGS) src/Process.cxx $(LDFLAGS) $(libraries:%=-l%) $(algorithms:%=-l%) -o bin/Process.exe

obj/%.o : src/%.cxx
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib/lib%.so : %

# Algorithms need to be compiled into object files and then migrated to a shared library
% : obj/%.o $(libraries)
	$(CXX) -shared $(LDFLAGS) $(libraries:%=-l%) -o lib/lib$@.so $<

clean :
	rm -rf ./lib/*.so*
	rm -rf ./bin/*
	rm -rf ./obj/*
