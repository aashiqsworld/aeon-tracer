cmake_minimum_required(VERSION 3.26)
project(aeon-tracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(aeon-tracer
        src/bitmap.h
        src/camera.h
        src/color.h
        src/common.h
        src/hittable.h
        src/hittable_list.h
        src/int3.h
        src/interval.h
        src/main.cc
        src/material.h
        src/mesh.h
        src/moving_sphere.h
        src/OBJ_Loader.h
        src/ray.h
        src/sphere.h
        src/triangle.h
        src/vec3.h
        src/aabb.h)
