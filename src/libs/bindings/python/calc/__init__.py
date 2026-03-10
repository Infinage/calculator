"""
@package calc
@file __init__.py
@brief Python bindings for the Calculator C API.

This module provides a thin Python wrapper around the Calculator C API
implemented in C++. The bindings use ctypes to call the shared library.

Example usage:

@code{.py}
import calc
c = calc.Calculator()
c.compute("1 + 2 * 3")
@endcode
"""

# ----------------------------------------------------------------------
# Internal imports
# ----------------------------------------------------------------------
import ctypes as _ctypes
import pathlib as _pathlib
import sys as _sys

# ----------------------------------------------------------------------
# Load the shared C API library (Cross-Platform)
# ----------------------------------------------------------------------
_base_path = _pathlib.Path(__file__).parent

# Map Python platform to expected extension
_ext_map = {
    "win32": ".dll",
    "darwin": ".dylib",
    "linux": ".so",
}

# Determine the extension; default to .so for other Unix-like systems
_ext = _ext_map.get(_sys.platform, ".so")
_lib_name = f"_libc_api{_ext}"
_lib_path = _base_path / _lib_name

if not _lib_path.exists():
    # Fallback: try to find any file starting with _libc_api
    # Helpful if the binary has a version suffix (e.g., .so.1)
    _found = list(_base_path.glob("_libc_api.*"))
    if _found:
        _lib_path = _found[0]
    else:
        raise ImportError(
            f"Calculator C API binary not found in {_base_path}. "
            f"Expected {_lib_name}"
        )

_lib = _ctypes.CDLL(str(_lib_path))

# ----------------------------------------------------------------------
# Define C function signatures (internal)
# ----------------------------------------------------------------------
_lib.Calculator_compute.argtypes = [_ctypes.c_char_p, _ctypes.POINTER(_ctypes.c_double)]
_lib.Calculator_compute.restype = _ctypes.c_bool

_lib.Calculator_get_last_error.argtypes = []
_lib.Calculator_get_last_error.restype = _ctypes.c_char_p

# ----------------------------------------------------------------------
# Public Python API
# ----------------------------------------------------------------------
class Calculator:
    """Python interface to the C++ Calculator via the C API.

    Provides a simple static method for evaluating expressions.
    Handles errors raised by the underlying C API.
    """

    @staticmethod
    def compute(expr: str) -> float:
        """Evaluate a mathematical expression using the C API.

        Args:
            expr (str): The expression to evaluate.

        Returns:
            float: Result of the expression.

        Raises:
            ValueError: If the expression is invalid or evaluation fails.
        """
        result = _ctypes.c_double()
        success = _lib.Calculator_compute(expr.encode("utf-8"), _ctypes.byref(result))
        if not success:
            err = _lib.Calculator_get_last_error().decode("utf-8")
            raise ValueError(f"{err}")
        return result.value

# ----------------------------------------------------------------------
# Public API export
# Only these names are visible to external users
# ----------------------------------------------------------------------
__all__ = ["Calculator"]
