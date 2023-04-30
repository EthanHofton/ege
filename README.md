## EGE

Ethans Game Engine

## Build Instructions

Build using debug or release profile with the commands below:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=[DEBUG|RELEASE]
cmake --build .
```

## Running Instruction

After running the build, `bin/ege` executable will be created. **_IMPORTANT_** run from the build directory otherwise the resources will not load properly.

```
./bin/ege
```
