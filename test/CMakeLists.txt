find_package(cmocka REQUIRED)

if(NOT TARGET cmocka::cmocka)
    add_library(cmocka::cmocka INTERFACE IMPORTED)
    set_target_properties(cmocka::cmocka PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMOCKA_INCLUDE_DIR}
                                                    INTERFACE_LINK_LIBRARIES ${CMOCKA_LIBRARY})
endif()

add_executable(robotraconteurlite_array_test array_test.c)
target_link_libraries(robotraconteurlite_array_test robotraconteurlite cmocka::cmocka)
add_test(array_test robotraconteurlite_array_test)
add_executable(robotraconteurlite_array_types_test array_test_types.c)
target_link_libraries(robotraconteurlite_array_types_test robotraconteurlite cmocka::cmocka)
add_test(array_test_types robotraconteurlite_array_types_test)
add_executable(robotraconteurlite_message_test message_test.c)
target_link_libraries(robotraconteurlite_message_test robotraconteurlite cmocka::cmocka)
add_test(message_test robotraconteurlite_message_test)

add_executable(robotraconteurlite_misc_test misc_test.c)
target_link_libraries(robotraconteurlite_misc_test robotraconteurlite cmocka::cmocka)
add_test(misc_test robotraconteurlite_misc_test)

add_subdirectory(no_stdint_test)
