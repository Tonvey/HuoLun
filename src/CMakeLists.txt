project(huolun)
file(GLOB srcs ./*.cpp ./*.cc ./*.c)
add_library(${PROJECT_NAME} ${srcs})
