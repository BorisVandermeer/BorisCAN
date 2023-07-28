

find_package(Threads REQUIRED)

SET(CMAKE_CXX_STANDARD=11)

add_executable(SimpleWrite SimpleWrite.cpp)
target_link_libraries(SimpleWrite ${BORIS_CAN_LIBRARIES})
target_include_directories(SimpleWrite PUBLIC ${BORIS_CAN_INCLUDE_DIRS})

add_executable(SimpleWriteLoop SimpleWriteLoop.cpp)
target_link_libraries(SimpleWriteLoop ${BORIS_CAN_LIBRARIES}  Threads::Threads)
target_include_directories(SimpleWriteLoop PUBLIC ${BORIS_CAN_INCLUDE_DIRS})

add_executable(SimpleMointor SimpleMointor.cpp)
target_link_libraries(SimpleMointor ${BORIS_CAN_LIBRARIES} )
target_include_directories(SimpleMointor PUBLIC ${BORIS_CAN_INCLUDE_DIRS})

add_executable(CallBackMonitor CallBackMonitor.cpp)
target_link_libraries(CallBackMonitor ${BORIS_CAN_LIBRARIES}  )
target_include_directories(CallBackMonitor PUBLIC ${BORIS_CAN_INCLUDE_DIRS})
