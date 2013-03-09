# Includes
IDIR       =include
IDIR_NATIVE=../mariadb-native-client/include

# Default folders
ODIR=obj
LDIR=lib
SDIR=src
OSDIR=$(ODIR)/$(SDIR)
DIRS=$(LDIR)	$(ODIR)	$(OSDIR)

# Sources
SRCS=account.cpp bind.cpp concurrency.cpp connection.cpp date_time.cpp decimal.cpp exceptions.cpp last_error.cpp private.cpp result_set.cpp save_point.cpp statement.cpp time.cpp time_span.cpp transaction.cpp worker.cpp

CXX     =g++
AR      =ar -r -s
CPPFLAGS=-I$(IDIR) -I$(IDIR_NATIVE) -std=c++11
CXXFLAGS=-Wall -O2
LIBS    =-lz -lssl -lstdc++ -pthread
TARGET  =libmariadb++.a
MKDIR   =mkdir -p

OBJS=$(patsubst %.cpp,$(OSDIR)/%.o,$(SRCS))
DEPS=$(patsubst %.cpp,$(OSDIR)/%.d,$(SRCS))

.PHONY:	clean all

all:	$(TARGET)

# Build target
$(TARGET):	makedirs	$(LDIR)	$(ODIR)	$(OSDIR)	$(OBJS)	$(DEPS)
	$(AR) $(LDIR)/$(TARGET) $(OBJS)

# Objects generation
$(OSDIR)/%.o:	$(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Generating dependencies
$(OSDIR)/%.d:	$(SDIR)/%.cpp
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


