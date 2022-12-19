#source file search paths
VPATH = ./src 

# compiler include search paths
INCS = -I./include \
	-I../windex/include \
	-I../raven-set \
	-I../boost/boost1_78 

# 	-I../ravencode/pub/src

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs -lComctl32 \
#	-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static

# folders
ODIR = ./.vscode/obj
RDIR = ../ravencode/pub/src

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_starter = \
	$(ODIR)/main.o \
	$(ODIR)/cStrategy.o \
	$(ODIR)/cGUI.o \

### COMPILE

$(ODIR)/%.o: %.cpp cStrategy.h cGUI.h
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

starter: $(OBJ_starter)
	g++ -o bin/strategy.exe \
	$(OPT) $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

rebuild: clean starter

