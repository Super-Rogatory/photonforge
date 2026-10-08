# PhotonForge

PhotonForge is a physically-based Monte Carlo renderer written in C++. It supports both **path tracing** and **photon mapping**, simulating realistic lighting effects like soft shadows, indirect illumination, color bleeding, and caustics. Designed to be modular, fast, and educational.

[Link]( https://docs.google.com/presentation/d/1cuxnHiPxdNVjZc35f5_ioNTofjmt96YahLIUCPYXu_U/edit?usp=sharing) to google presentation showing architecture and renders
---

## 🚧 Features (in Progress)

- ✅ Path tracing (unbiased global illumination)
- ✅ Photon mapping (two-pass caustic + indirect light system)
- ✅ Hybrid mode: path tracing + photon lookup
- ✅ Physically-based materials: Diffuse, Specular, Cook-Torrance
- ✅ Acceleration structures (BVH, KD-Tree)
- ✅ Outputs `.ppm` image format for easy testing and debugging
- ✅ Multi-threaded CPU Support

---

## 🗂 Simplified Structure

```bash
src/
├── main.cpp                  # Entry point
├── core/                     # Math, ray, camera, scene
├── geometry/                 # Spheres, triangles, KD-tree, BVH
├── materials/                # Core BRDFs (diffuse, specular, Cook-Torrance)
├── integrators/              # PathTracer, PhotonMapper, Hybrid
├── lights/                   # Light sources
└── utils/                    # Scene loading, image writing, logging
```
---

## 🛠️ Build Instructions

### Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

> Make sure you have a C++ compiler (GCC or Clang) and CMake installed.

---

## 🚀 How to Run the Renderer

```bash
./photonforge
```

---

## 👨‍💻 Authors

- Abel Tung  
- Chukwudi Ikem  
- Claudia Osorio  
- Elmer Vasquez  

---

## 📚 References

- [Photon Mapping – Henrik Wann Jensen](https://graphics.stanford.edu/papers/photonglobal/)
- [PBRT: Physically Based Rendering](https://www.pbrt.org/)
- [James Griffin's Path Tracer](https://github.com/JamesGriffin/Path-Tracer)


