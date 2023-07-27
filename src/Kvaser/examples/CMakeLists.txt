
# add_executable(test_listen listencan.cpp)
# target_link_libraries(test_listen ${BORIS_CAN_LIBRARIES})
# target_include_directories(test_listen PUBLIC ${BORIS_CAN_INCLUDE_DIRS} ..)


add_executable(test_monitor canmonitor.cpp)
target_link_libraries(test_monitor ${BORIS_CAN_LIBRARIES})
target_include_directories(test_monitor PUBLIC ${BORIS_CAN_INCLUDE_DIRS} ../..)

