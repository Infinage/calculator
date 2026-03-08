set(AVAILABLE_SPECIAL_TARGETS "" CACHE INTERNAL "List of special targets")

# Function to register targets
function(register_special_target name)
    list(APPEND AVAILABLE_SPECIAL_TARGETS "${name}")
    set(AVAILABLE_SPECIAL_TARGETS "${AVAILABLE_SPECIAL_TARGETS}" CACHE INTERNAL "" FORCE)
endfunction()
