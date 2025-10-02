# raytracer

A ray tracer written in C++.

---

## features
- PPM and PNG reading/writing
- Spheres

## build
```
make
```

### dependencies
- `pkg-config` must be installed to detect libraries
- `libpng++` can be installed to add PNG support
  - if not detected when building, the raytracer will only support PPMs
- `g++`
  
### other make targets
- `clean`: remove build files
- `libs`: show linked libraries

## usage
```
./raytracer (-h/--help) (-x/--width <width>) (-y/--height <height>) (-a/--aspect-ratio <ratio>) (filename)
```

### args
- `-h/--help`: display usage info
- `-x/--width (width)`: change resulting image width
  - default `400`
- `-y/--height (height)`: change resulting image height
  - default `200`
- `-a/--aspect-ratio (ratio)`: change resulting aspect ratio
  - if specified, you can also optionally provide either `-x/--width` or `-y/--height` and the aspect ratio will be applied to get the other size. otherwise, it's applied to the height using the default width.
  - floats or ratios (ie, `16:9`) are allowed
- `(filename)`: change output file
  - `.ppm` and `.png` are supported
  - default `out.png`
