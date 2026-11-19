# mechtal

This is a port to C++ of my Jai game engine, which is here: [metal](https://github.com/IrvingWash/metal)

## Structure

`mechtal/` - "the game"
`gltf/`    - GLTF Parser
`input/`   - Subroject for handling inputs. Currently is a wrapper around SDL3
`windowing/` - Subproject for window creation. Currently is a wrapper around SDL3

## Building

Debug:
```bash
cmake -B build-debug -DSANITIZE=ON
cmake --build-debug build
```

Release:
```bash
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build-release build
```
