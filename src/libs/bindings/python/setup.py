"""
@file setup.py
@brief Packaging configuration for the Python Calculator bindings.

This script builds and installs the `calc` Python package which wraps
the Calculator C API.
"""

import pathlib
import setuptools

here = pathlib.Path(__file__).parent

setuptools.setup(
    name="calc",
    version="0.1",
    packages=setuptools.find_packages(),
    package_data={"calc": ["libc_api.so"]},
)
