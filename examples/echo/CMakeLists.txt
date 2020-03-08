add_executable(stdioecho ./stdioecho.cpp)
add_executable(tcpecho ./tcpecho.cpp)
target_link_libraries(stdioecho huolun)
target_link_libraries(tcpecho huolun)
