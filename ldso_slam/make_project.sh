BUILD_TYPE=Release
NUM_PROC=4

BASE_DIR="$PWD"

# Add library DBoW3
cd "$BASE_DIR/Thirdparty/DBoW3"
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
make -j$NUM_PROC

# Add library g2o
cd "$BASE_DIR/Thirdparty/g2o"
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
make -j$NUM_PROC

# Compile our project
cd "$BASE_DIR"
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
make -j$NUM_PROC
