# Implicit Modeller

Implementation of a basic implicit function modelling kernel.

## Building

### Linux (Debian flavors)

```bash
sudo apt update
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
mkdir build
cd build
cmake ..
make
```

## Development Container

This should work for any of the major desktop platforms, but is not particularly performant.
You will likely see banding due to vnc compression and your framerate will be lower than native.
The solution is intended to be good enough for debugging only if you haven't configured your local development environment.
A native environment will basically always be better than this.

Real time rendering can be achieved using the novnc viewer.
Run one of the examples and navigate to http://localhost:6080.
The password is "vscode".
