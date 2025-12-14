# raytracer

A ray tracer written in C++.

[View summary report](report.pdf)

![main scene](final.png)
<sup>It's never lupus.</sup>

---

## build
```
make
```

### dependencies
- `pkg-config` must be installed to detect libraries
- `libpng` and `libpng++` can be installed to add PNG support
  - if both aren't detected when building, the raytracer will only support PPMs
- `libassimp` can be installed to add model rendering support
  - if not detected when building, scenes will skip loading model files
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
- `-s/--antialias-samples`: number of antialiasing samples per pixel
  - default `10`
- `-d/--max-recursion-depth`: maximum recursion depth for one ray
  - default `50`
- `-F/--fov`: image field of view in degrees
  - default `80`
- `-w/--workers`: number of worker threads
  - default `4`
- `-f/--frac`: fractional rendering, renders only a fraction of the image
  - this allows rendering parts of the image across multiple machines and joining them into one once they're finished
  - use the `joiner` binary to join them (list of files to join in order, then the destination file)
  - see [`megarender.sh`](megarender.sh) for an example of how I used this to parallelize across 3 devices with SSH
  - format is a fraction: for example, 1/3, 2/3, and 3/3 will get all 3 parts of an image
- `-A/--adaptive-sampling`: enables adaptive sampling, which will stop collecting samples for a pixel early if the 
  current color is within a confidence interval (controlled with `-t`).
- `-t/--as-tolerance`: confidence interval threshold, below which pixel sampling stops early. default
  - default `1.8`
- `-S/--scene`: name of the scene to render
  - default `house`, otherwise `demo`
- `(filename)`: change output file
  - `.ppm` and `.png` are supported
  - default `out.png`

## citations
### algorithms
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
- [Adaptive Sampling](https://cs184.eecs.berkeley.edu/sp21/docs/proj3-1-part-5) from Berkeley CS184/284A
- [Fast, Minimum Storage Ray/Triangle Intersection](https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf)

### scene
I haven't bundled any of these here, to render you'll have to download them
and write them with the names noted into `assets/`.
- `chair.glb`: [Executive Chair](https://poly.pizza/m/hW11P9DDXa) by Zsky
- `lamp.glb`: [Desk lamp](https://sketchfab.com/3d-models/desk-lamp-7377ec591df04445a1aae370017aaa13) by KaramellGlass 
- `computer.glb`: [Desktop Computer](https://sketchfab.com/3d-models/desktop-computer-ce4e4200cf0b472d956c36ab7bb94405) by GeniusPilot2016
- `chair.glb`: [Lowpoly Office Chair](https://sketchfab.com/3d-models/lowpoly-office-chair-e5719afff2464e5e810115532e16cc05) by sleepyjoshua
- `books.glb`: [Books](https://sketchfab.com/3d-models/books-62fc2898df55483f87461a8f12a0113f) by igor-tkachenko
- `coffee_cup.glb`: [Coffee Cup Modern !](https://sketchfab.com/3d-models/coffee-cup-modern-e5c9000d9f204c3b88daaf2b953ea459) by Nexus Development
- `book_stack.glb`: [Pila De Libros • Stack of books](https://sketchfab.com/3d-models/pila-de-libros-stack-of-books-66a6934a2201484e87980d5efa3883cc#download) by Blender Pink
- `skull.glb`: [Skull Proportions & Key Landmarks](https://sketchfab.com/3d-models/skull-proportions-key-landmarks-20c0f43e3778481c830978709d784681) by Shape Foundations
- `book_row.glb`: [Books](https://sketchfab.com/3d-models/books-e71051054d614b9ebfb9c0a600942b10) by Rifqi-02
- `globe.glb`: [Lowpoly/Stylised GLOBE](https://sketchfab.com/3d-models/lowpolystylised-globe-aecef111255146418e63d84e99812edb#download) by Ethan C
- `visitor_chair.glb`: [Chair](https://poly.pizza/m/2aXnX7qocj1) by CMHT Oculus