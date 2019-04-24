cmake_minimum_required(VERSION 3.7)
project(with_cuda)

# using same path `@loader_path` during building and installation
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "@loader_path")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# gen lib - in cuBERT makefile
add_subdirectory(cuBERT)

# bind-python
add_subdirectory(pybind11)

include_directories(cuBERT/src)
link_directories(cuBERT)

pybind11_add_module(cuBERT_pybind python/cuBERT/pybind/cuBERT_pybind.cc)

if(cuBERT_ENABLE_MKL_SUPPORT)
    link_directories(${mkl_SHARED_LIBRARIES})  # using so-lib-dir
    message("site-package:" ${PYTHON_SITE_PACKAGE})
    message("mkl_SHARED_LIBRARIES:" ${mkl_SHARED_LIBRARIES})

    # do the copying
    foreach(mkl_lib ${mkl_SHARED_LIBRARIES})
        message("copy" ${mkl_lib} " to " ${PYTHON_SITE_PACKAGE})
        add_custom_command(
            TARGET cuBERT_pybind
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            ARGS -E copy ${mkl_lib} ${PYTHON_SITE_PACKAGE}
    )
    endforeach(mkl_lib)

    target_link_libraries(cuBERT_pybind PRIVATE cuBERT ${mkl_SHARED_LIBRARIES})
else()
    target_link_libraries(cuBERT_pybind PRIVATE cuBERT)
endif(cuBERT_ENABLE_MKL_SUPPORT)
