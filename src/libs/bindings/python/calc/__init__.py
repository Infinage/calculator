"""
@package calc
@file __init__.py
@brief Python bindings for the Calculator C API.

This module provides a thin Python wrapper around the Calculator C API
implemented in C++. The bindings are implemented using ctypes.

Example usage:

@code{.py}
from calc import Calculator
Calculator.compute("1 + 2 * 3")
@endcode
"""

import ctypes
import pathlib

# ----------------------------------------------------------------------
# Load the shared library
# ----------------------------------------------------------------------
lib_path = pathlib.Path(__file__).parent / "libc_api.so"
lib = ctypes.CDLL(str(lib_path))

# ----------------------------------------------------------------------
# Define C function signatures for ctypes
# ----------------------------------------------------------------------
# bool Calculator_compute(const char *expr, double *result)
lib.Calculator_compute.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_double)]
lib.Calculator_compute.restype = ctypes.c_bool

# const char *Calculator_get_last_error()
lib.Calculator_get_last_error.argtypes = []
lib.Calculator_get_last_error.restype = ctypes.c_char_p

# ----------------------------------------------------------------------
# Python wrapper class
# ----------------------------------------------------------------------
class Calculator:
    """
    @brief Python interface to the Calculator engine.

    Provides access to the underlying C++ calculator via the C API.
    Expressions are evaluated using the static method compute().
    """

    @staticmethod
    def compute(expr: str) -> float:
        """
        @brief Compute the result of a mathematical expression.

        Calls the C API function `Calculator_compute`. Raises a ValueError if the
        computation fails (e.g., invalid syntax or imbalanced parentheses).

        @param expr The expression to compute (e.g., "1 + 2 * 3").
        @return The computed result as a float.
        @throws ValueError If the expression is invalid or evaluation fails.
        """
        result = ctypes.c_double()
        success = lib.Calculator_compute(expr.encode("utf-8"), ctypes.byref(result))
        if not success:
            # Retrieve the last error from the C API (returns a c_char_p)
            err = lib.Calculator_get_last_error().decode("utf-8")
            raise ValueError(f"Computation error: {err}")
        return result.value

# ----------------------------------------------------------------------
# Example usage when run as a script
# ----------------------------------------------------------------------
if __name__ == "__main__":
    print("Compute '1 + 2 * 3':", Calculator.compute("1 + 2 * 3"))
    try:
        Calculator.compute("12 + 3)")
    except ValueError as e:
        print("Caught error:", e)
