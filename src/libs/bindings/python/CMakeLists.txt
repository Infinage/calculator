file(COPY setup.py calc DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(Python3 COMPONENTS Interpreter Development)
if (PYTHON3_FOUND)
    register_special_target(python_wheel)
    add_custom_target(python_wheel DEPENDS c_api
        COMMENT "Building Python wheels for Calculator"
        COMMAND ${Python3_EXECUTABLE} setup.py bdist_wheel
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
