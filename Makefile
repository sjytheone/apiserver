TARGET   	= ./lib/apiserver.so
OBJPATH 	= .
SOURCES		= $(wildcard *.cpp ./src/*.cpp)
OBJS    	= $(patsubst %.cpp,%.o,$(SOURCES))
FULLOBJS 	= $(patsubst %.cpp,$(OBJPATH)/%.o,$(SOURCES))
INCPATH		= -I. -I./include
LIBPATH		= -L. -lrt
CFLAGS		= -w -c

LFLAGS          = -w -shared
CFLAGS          = -w -c
CXX     	= g++

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o $(TARGET) $(LIBPATH)

$(OBJS):$(SOURCES)
	$(CXX) $(CFLAGS) $*.cpp -o $(OBJPATH)/$@ $(INCPATH)

clean:
	rm -f $(OBJPATH)/*.o
	rm -f $(TARGET)