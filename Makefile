CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g -Isrc
LDFLAGS =

LIBPNG_AVAILABLE := $(shell pkg-config --exists libpng && echo "1" || echo "0")
# :)
PNGPP_HEADERS := $(shell { echo '#include <png++/png.hpp>'; echo 'int main(){return 0;}'; } | $(CXX) $(shell pkg-config --cflags libpng 2>/dev/null) -x c++ - -o /dev/null 2>/dev/null && echo "1" || echo "0")

ifeq ($(LIBPNG_AVAILABLE)$(PNGPP_HEADERS),11)
    CXXFLAGS += -DPNGPP $(shell pkg-config --cflags libpng)
    LDFLAGS += $(shell pkg-config --libs libpng)
endif

raytracer: build/main.o build/images/ppm.o build/images/png.o build/images/image.o build/math/vec3.o build/obj/scene.o build/obj/sphere.o build/math/ray.o
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o raytracer \
		build/main.o \
		build/images/ppm.o \
		build/images/png.o \
		build/images/image.o \
		build/math/vec3.o \
		build/math/ray.o \
		build/obj/scene.o \
		build/obj/sphere.o \
		$(LDFLAGS)

build/main.o: src/main.cpp src/macros.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/images/image.o: src/images/image.cpp src/images/image.hpp src/macros.hpp
	mkdir -p build/images
	$(CXX) $(CXXFLAGS) -c src/images/image.cpp -o build/images/image.o

build/images/ppm.o: src/images/ppm.cpp src/images/ppm.hpp src/images/image.hpp src/macros.hpp
	mkdir -p build/images
	$(CXX) $(CXXFLAGS) -c src/images/ppm.cpp -o build/images/ppm.o

build/images/png.o: src/images/png.cpp src/images/png.hpp src/images/image.hpp src/macros.hpp
	mkdir -p build/images
	$(CXX) $(CXXFLAGS) -c src/images/png.cpp -o build/images/png.o

build/math/vec3.o: src/math/vec3.cpp src/math/vec3.hpp src/macros.hpp
	mkdir -p build/math
	$(CXX) $(CXXFLAGS) -c src/math/vec3.cpp -o build/math/vec3.o

build/math/ray.o: src/math/ray.cpp src/math/ray.hpp src/macros.hpp
	mkdir -p build/math
	$(CXX) $(CXXFLAGS) -c src/math/ray.cpp -o build/math/ray.o

build/obj/scene.o: src/obj/scene.cpp src/obj/scene.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/scene.cpp -o build/obj/scene.o

build/obj/sphere.o: src/obj/sphere.cpp src/obj/sphere.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/sphere.cpp -o build/obj/sphere.o

.PHONY: clean
clean:
	rm -rf build/ raytracer

.PHONY: info
info:
	@echo "libpng available: $(LIBPNG_AVAILABLE)"
	@echo "png++ headers available: $(PNGPP_HEADERS)"
	@echo "PNG++ support enabled: $(shell test '$(LIBPNG_AVAILABLE)$(PNGPP_HEADERS)' = '11' && echo 'YES' || echo 'NO')"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"