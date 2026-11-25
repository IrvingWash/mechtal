# mechtal

This is a port to C++ of my Jai game engine, which is here: [metal](https://github.com/IrvingWash/metal)

Work in progress, currently done:
- Windowing
- Input handling
- glTF parsing


## Structure

`mechtal/`   - "the game"
`gltf/`      - GLTF Parser
`input/`     - Subproject for handling inputs. Currently is a wrapper around SDL3
`windowing/` - Subproject for window creation. Currently is a wrapper around SDL3

## Building

Debug:
```bash
cmake -B build-debug -DSANITIZE=ON
cmake --build build-debug
```

Release:
```bash
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```
