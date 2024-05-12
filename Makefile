SOURCE_FILES=$(wildcard src/*.cpp)

all:
	$(CXX) -O2 -shared -fPIC --no-gnu-unique $(SOURCE_FILES)  -o hyprfocus.so `pkg-config --cflags pixman-1 libdrm hyprland` -I/usr/local/include/hyprland/protocols/ -std=c++2b
	strip hyprfocus.so

debug:
	$(CXX) -O2 -shared -fPIC --no-gnu-unique $(SOURCE_FILES)  -o hyprfocus.so `pkg-config --cflags pixman-1 libdrm hyprland` -I/usr/local/include/hyprland/protocols/ -std=c++2b

clean:
	rm ./hyprfocus.so
