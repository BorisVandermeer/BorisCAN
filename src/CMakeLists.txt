
SET(BORIS_CAN_SRC basehandler.cpp)

if(USING_KVASER)
    aux_source_directory(Kvaser KVASER_SRC)
    add_library(BORIS_CAN_LIB  ${KVASER_SRC} ${BORIS_CAN_SRC})
    target_include_directories(BORIS_CAN_LIB PUBLIC ${BORIS_CAN_INCLUDE_DIRS} Kvaser/include)
    SET(BORIS_CAN_LIBRARIES BORIS_CAN_LIB canlib )
    if(BUILD_EXAMPLES)
        add_subdirectory(Kvaser/examples)
    endif()
    SET(BORIS_CAN_LIBRARIES ${BORIS_CAN_LIBRARIES} PARENT_SCOPE)
endif()


