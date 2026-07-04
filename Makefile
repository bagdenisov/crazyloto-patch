CXX      = i686-w64-mingw32-g++
WINDRES  = windres
CXXFLAGS = -shared -static -g
RCFLAGS  = --target=pe-i386
OUT_DIR  = build

TARGETS  = $(OUT_DIR)/_dvapi32.dll $(OUT_DIR)/_ernel32.dll $(OUT_DIR)/modding.exe
RES_OBJ  = info.o

all: $(OUT_DIR) $(TARGETS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(RES_OBJ): info.rc info.manifest
	$(WINDRES) info.rc -o $(RES_OBJ) $(RCFLAGS) --target=pe-i386

$(OUT_DIR)/_dvapi32.dll: _dvapi32.cpp _dvapi32.def $(RES_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ _dvapi32.cpp _dvapi32.def $(RES_OBJ)

$(OUT_DIR)/_ernel32.dll: _ernel32.cpp _ernel32.def $(RES_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ _ernel32.cpp _ernel32.def $(RES_OBJ)

$(OUT_DIR)/modding.exe: modding.c 
	$(CXX) -static -static-libgcc -static-libstdc++ -g -o $@ modding.c

clean:
	rm -f $(RES_OBJ)
	rm -rf $(OUT_DIR)

.PHONY: all clean
