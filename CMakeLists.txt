cmake_minimum_required(VERSION 3.26)
project(aeon-tracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(aeon-tracer
        src/bitmap.h
        src/Camera.h
        src/Color.h
        src/Common.h
        src/Hittable.h
        src/HittableList.h
        src/int3.h
        src/Interval.h
        src/main.cc
        src/Material.h
        src/Mesh.h
        src/MovingSphere.h
        src/OBJ_Loader.h
        src/Ray.h
        src/Sphere.h
        src/Triangle.h
        src/vec3.h
        src/AABB.h
        src/BVH.h)
