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
  - doubles or ratios (ie, `16:9`) are allowed
- `-s/--antialias/samples`: number of antialiasing samples per pixel
  - default `10`
- `-d/--max-recursion-depth`: maximum recursion depth for one ray
  - default `50`
- `-w/--workers`: number of worker threads
  - default `4`
- `-f/--frac`: fractional rendering, renders only a fraction of the image
  - this allows rendering parts of the image across multiple machines and joining them into one once they're finished
  - use the `joiner` binary to join them (list of files to join in order, then the destination file)
  - see [`megarender.sh`](megarender.sh) for an example of how I used this to parallelize across 3 devices with SSH
  - format is a fraction: for example, 1/3, 2/3, and 3/3 will get all 3 parts of an image
- `(filename)`: change output file
  - `.ppm` and `.png` are supported
  - default `out.png`
