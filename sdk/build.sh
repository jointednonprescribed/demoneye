#!/usr/bin/bash

PARENT='/p/jointednonprescribed/demoneye/main'
CWD=$PARENT/sdk

g++ -shared -fPIC -o$PARENT/bin/libdemoneye.so -I$PARENT/deps/glad/include -I$PARENT/deps/glfw3.3/include -I$PARENT/deps/sdl3.2.x/include -I$CWD/include $CWD/lib/gl.cxx -ldemoneye-glad -lglfw &&
sudo cp $PARENT/bin/libdemoneye.so /usr/lib
