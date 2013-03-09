# Includes
IDIR       =../include
IDIR_NATIVE=../../mariadb-native-client/include

# Default folders
ODIR=obj
DIRS=$(ODIR)	$(OSDIR)

# Sources
SRCS=database.cpp date_time.cpp decimal.cpp main.cpp test.cpp

CXX     =g++
CPPFLAGS=-I$(IDIR) -I$(IDIR_NATIVE) -std=c++11
CXXFLAGS=-Wall -O2
LIBS    =../lib/libmariadb++.a ../../mariadb-native-client/libmariadb/libmariadbclient.a -lz -lssl -lstdc++ -pthread
TARGET  =unittest
MKDIR   =mkdir -p

OBJS=$(patsubst %.cpp,$(ODIR)/%.o,$(SRCS))
DEPS=$(patsubst %.cpp,$(ODIR)/%.d,$(SRCS))

.PHONY:	clean all

all:	$(TARGET)

# Build target
$(TARGET):	makedirs	$(OBJS)	$(DEPS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

# Objects generation
$(ODIR)/%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Generating dependencies
$(ODIR)/%.d:	%.cpp
	$(CXX) -M $(CXXFLAGS) $(CPPFLAGS) $< > $@

#Cleanup objects / dependencies and target
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

# Create directories
makedirs:
	@$(call make-dirs)
	
define make-dirs
   	for dir in $(DIRS);\
   	do\
   		mkdir -p $$dir;\
   	done
endef


