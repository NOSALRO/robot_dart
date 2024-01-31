# # Build pybind11 if needed; for clang we need at least v2.5.0
# if [ "$COMPILER" = "clang" ]; then
#     git clone https://github.com/pybind/pybind11 -b 'v2.5.0' --single-branch --depth 1
#     cd pybind11
#     mkdir build && cd build
#     cmake .. -DCMAKE_BUILD_TYPE=Release -DPYBIND11_TEST=OFF
#     make -j4
#     sudo make install
#     cd ../..
# fi
