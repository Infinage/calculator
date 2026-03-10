"""
@file setup.py
@brief Minimal shim to force platform-specific wheel generation.

This script supplements pyproject.toml. By defining has_ext_modules, 
we ensure the resulting wheel is tagged with the current OS and CPU 
architecture, as it contains a compiled C++ shared library (.so/.dll).
"""

import setuptools

# The lambda: True hack forces setuptools to treat this as a non-pure package.
# All other metadata (name, version, etc.) is pulled from pyproject.toml.
setuptools.setup(has_ext_modules=lambda: True)
