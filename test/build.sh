#!/usr/bin/bash

g++ -otest -I../sdk/include -I../deps/sdl3.2.x/include -I../deps/glfw3.3/include -I../deps/glad/include ../deps/glad/src/glad.c test.cpp -lglfw -lm
