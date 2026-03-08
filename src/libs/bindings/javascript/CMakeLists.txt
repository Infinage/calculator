find_program(EMPP_EXECUTABLE em++)
if (NOT EMPP_EXECUTABLE)
    message(WARNING "em++ not found in path.")
    return()
endif()

# em++ calc_js_api.cc ../../core/*.cc -I../../core/include -std=c++23 -O3 --bind -o calculator.js
set(CALC_SOURCE_DIR $<TARGET_PROPERTY:calculator,SOURCE_DIR>)
set(CALC_SOURCES calculator.cc evaluator.cc parser.cc tokenizer.cc)
LIST(TRANSFORM CALC_SOURCES PREPEND ${CALC_SOURCE_DIR}/)
add_custom_command(OUTPUT calculator.wasm
    COMMAND ${EMPP_EXECUTABLE}
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/calc_js_api.cc 
    ${CALC_SOURCES} -I${CALC_SOURCE_DIR}/include
    $<$<CONFIG:Release>:-O3> -std=c++23 --bind -o calculator.js
    DEPENDS ${CALC_SOURCES}
    COMMENT "Building target calculator.wasm"
    COMMAND_EXPAND_LISTS)

# Making sure the build invokes the custom command above
add_custom_target(calculator_wasm ALL DEPENDS calculator.wasm)

# An optional target to package this folder as a zip file
register_special_target(wasm_zip)
add_custom_target(wasm_zip DEPENDS calculator_wasm
    COMMAND ${CMAKE_COMMAND} -E tar "cf" 
    ${CMAKE_CURRENT_BINARY_DIR}/calculator_wasm.zip 
    ${CMAKE_CURRENT_SOURCE_DIR}/calculator.html
    ${CMAKE_CURRENT_SOURCE_DIR}/calculator.js
    ${CMAKE_CURRENT_SOURCE_DIR}/calculator.wasm
    --format=zip 
    COMMENT "Packaging JS/WASM bindings into calculator_wasm.zip")
