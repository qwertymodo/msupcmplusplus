# msupcm++

A native libSox implementation of the msupcm tool

### Windows download

See Releases

### Compiling on Linux (experimental)

Compile with:
```
make
```
Several audio libraries are required. If you get linker errors, you probably need to install the libraries mentioned in the errors, via development packages in your distro's package management system. If so, you may also need to re-clone the project and its submodules so that the SoX configure picks up those libraries; otherwise you may get runtime errors about no handler for detected file types.

When built, the binary is available at:
```
msupcm++/msupcm
```
