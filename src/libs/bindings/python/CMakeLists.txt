file(COPY setup.py calc pyproject.toml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(Python3 COMPONENTS Interpreter Development)
if (PYTHON3_FOUND)
    register_special_target(python_wheel)
    add_custom_target(python_wheel DEPENDS c_api
        COMMENT "Building Python wheels for Calculator"
        COMMAND ${Python3_EXECUTABLE} -m build --wheel
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
