# mechtal

This is a port to C++ of my Jai game engine, which is here: [metal](https://github.com/IrvingWash/metal)

## Structure

`mechtal/` - "the game"
`gltf/`    - GLTF Parser
`input/`   - Subroject for handling inputs. Currently is a wrapper around SDL3
`windowing/` - Subproject for window creation. Currently is a wrapper around SDL3

## Building

```bash
cmake -B build
cmake --build build
```
