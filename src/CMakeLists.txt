file(GLOB srcs ./*.cpp ./*.cc ./*.c ./*/*.cpp)
add_library(${PROJECT_NAME} STATIC ${srcs})
