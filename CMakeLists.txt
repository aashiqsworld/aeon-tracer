cmake_minimum_required(VERSION 3.26)
project(cpu_ray_tracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cpu_ray_tracer
        camera.h
        color.h
        common.h
        hittable.h
        hittable_list.h
        main.cc
        material.h
        ray.h
        sphere.h
        vec3.h
        interval.h
        mesh.h
        int3.h
        bitmap.h
        OBJ_Loader.h
        triangle.h)
