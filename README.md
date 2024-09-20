## implement_compress_by_cuda
Compare the compression of CUDA and C++
## start test cpp compress
cd ~/implement_compress_by_cuda
python generate_data.py
mkdir build
cd build 
cmake ..
make -j$(nproc)
./test_cpp_compress