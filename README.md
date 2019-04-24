# pybind11-example

examples of Python binds c/c++ using [pybind11](https://github.com/pybind/pybind11)


[simple-usage](./simple-usage)

[with-cmake](./with-cmake)

[with-cmake-cuda](./with-cuda)

first append the following to [CMakeLists.txt](./with-cuda/cuBERT/CMakeLists.txt):

```
sed -i -e '/^set(mkl_SHARED_LIBRARIES ${mkl_SHARED_LIBRARIES} PARENT_SCOPE)$/d' cuBERT_CMAKE
echo 'set(mkl_SHARED_LIBRARIES ${mkl_SHARED_LIBRARIES} PARENT_SCOPE)' >> cuBERT_CMAKE
```

otherwise the build `so(dylib)` won't be copied into site-package
