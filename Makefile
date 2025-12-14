# I wrote this before I knew how to use CMake.
# I'm in too deep to turn back now

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -g -Isrc
LDFLAGS =

LIBPNG_AVAILABLE := $(shell pkg-config --exists libpng && echo "1" || echo "0")
ASSIMP_AVAILABLE := $(shell pkg-config --exists assimp && echo "1" || echo "0")
# :)
PNGPP_HEADERS := $(shell { echo '#include <png++/png.hpp>'; echo 'int main(){return 0;}'; } | $(CXX) $(shell pkg-config --cflags libpng 2>/dev/null) -x c++ - -o /dev/null 2>/dev/null && echo "1" || echo "0")

ifeq ($(LIBPNG_AVAILABLE)$(PNGPP_HEADERS),11)
    CXXFLAGS += -DPNGPP $(shell pkg-config --cflags libpng)
    LDFLAGS += $(shell pkg-config --libs libpng)
endif

ifeq ($(ASSIMP_AVAILABLE),1)
	CXXFLAGS += -DASSIMP $(shell pkg-config --cflags assimp)
	LDFLAGS += $(shell pkg-config --libs assimp)
endif

all: raytracer joiner

raytracer: \
		build/main.o \
		build/camera.o \
		build/images/ppm.o \
		build/images/png.o \
		build/images/image.o \
		build/math/interval.o \
		build/math/vec3.o \
		build/math/ray.o \
		build/math/pdf.o \
		build/obj/object.o \
		build/obj/scene.o \
		build/obj/sphere.o \
		build/obj/triangle.o \
		build/obj/quad.o \
		build/obj/cdm.o \
		build/obj/material.o \
		build/obj/texture.o \
		build/obj/model.o \
		build/obj/bvh.o \
		build/obj/aabb.o
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o raytracer \
		build/main.o \
		build/camera.o \
		build/images/ppm.o \
		build/images/png.o \
		build/images/image.o \
		build/math/interval.o \
		build/math/ray.o \
		build/math/vec3.o \
		build/math/pdf.o \
		build/obj/object.o \
		build/obj/scene.o \
		build/obj/sphere.o \
		build/obj/triangle.o \
		build/obj/quad.o \
		build/obj/cdm.o \
		build/obj/material.o \
		build/obj/texture.o \
		build/obj/model.o \
		build/obj/bvh.o \
		build/obj/aabb.o \
		$(LDFLAGS)

joiner: \
		build/joiner.o \
		build/images/ppm.o \
		build/images/png.o \
		build/images/image.o
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o joiner \
		build/joiner.o \
		build/images/ppm.o \
		build/images/png.o \
		build/images/image.o \
		$(LDFLAGS)

build/joiner.o: src/joiner.cpp src/macros.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/joiner.cpp -o build/joiner.o

build/main.o: src/main.cpp src/macros.hpp src/scenes/house.hpp src/scenes/demo.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/camera.o: src/camera.cpp src/camera.hpp src/macros.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/camera.cpp -o build/camera.o

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

build/math/interval.o: src/math/interval.cpp src/math/interval.hpp src/macros.hpp
	mkdir -p build/math
	$(CXX) $(CXXFLAGS) -c src/math/interval.cpp -o build/math/interval.o

build/math/pdf.o: src/math/pdf.cpp src/math/pdf.hpp src/macros.hpp
	mkdir -p build/math
	$(CXX) $(CXXFLAGS) -c src/math/pdf.cpp -o build/math/pdf.o

build/obj/object.o: src/obj/object.cpp src/obj/object.hpp src/macros.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/object.cpp -o build/obj/object.o

build/obj/scene.o: src/obj/scene.cpp src/obj/scene.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/scene.cpp -o build/obj/scene.o

build/obj/sphere.o: src/obj/sphere.cpp src/obj/sphere.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/sphere.cpp -o build/obj/sphere.o

build/obj/triangle.o: src/obj/triangle.cpp src/obj/triangle.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/triangle.cpp -o build/obj/triangle.o

build/obj/quad.o: src/obj/quad.cpp src/obj/quad.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/quad.cpp -o build/obj/quad.o

build/obj/cdm.o: src/obj/cdm.cpp src/obj/cdm.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/cdm.cpp -o build/obj/cdm.o

build/obj/material.o: src/obj/material.cpp src/obj/material.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/material.cpp -o build/obj/material.o

build/obj/texture.o: src/obj/texture.cpp src/obj/texture.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/texture.cpp -o build/obj/texture.o

build/obj/model.o: src/obj/model.cpp src/obj/model.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/model.cpp -o build/obj/model.o

build/obj/bvh.o: src/obj/bvh.cpp src/obj/bvh.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/bvh.cpp -o build/obj/bvh.o

build/obj/aabb.o: src/obj/aabb.cpp src/obj/aabb.hpp src/macros.hpp src/obj/object.hpp
	mkdir -p build/obj
	$(CXX) $(CXXFLAGS) -c src/obj/aabb.cpp -o build/obj/aabb.o

.PHONY: clean
clean:
	rm -rf build/ raytracer

.PHONY: libs
libs:
	@echo "libpng++: $(shell test '$(LIBPNG_AVAILABLE)$(PNGPP_HEADERS)' = '11' && echo 'y' || echo 'n')"
	@echo "libassimp: $(shell test '$(ASSIMP_AVAILABLE)' = '1' && echo 'y' || echo 'b')"