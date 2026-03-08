/**
 * @file calc_c_api.h
 * @brief C API wrapper for the Calculator C++ library.
 *
 * This header exposes a minimal C interface for the Calculator engine so it can
 * be used from other languages such as Python, JavaScript, or C.
 *
 * The API operates on mathematical expressions represented as UTF-8 strings.
 * Errors are stored internally and can be retrieved using
 * Calculator_get_last_error().
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief Compute the result of a mathematical expression.
   *
   * Evaluates a mathematical expression using the underlying Calculator engine.
   *
   * @param expr Null-terminated UTF-8 string representing the expression.
   *             Example: `"1 + 2 * 3"`.
   * @param result Pointer to a double where the computed result will be
   * written.
   *
   * @return true if evaluation succeeded.
   * @return false if evaluation failed (syntax error, invalid expression, etc).
   *
   * @note If the function returns false, the error message can be retrieved
   * using Calculator_get_last_error().
   */
  bool Calculator_compute(const char *expr, double *result);

  /**
   * @brief Retrieve the last error message produced by the Calculator.
   *
   * Returns a pointer to a null-terminated string describing the most recent
   * error produced by Calculator_compute().
   *
   * @return Pointer to an internal error string.
   *
   * @warning The returned pointer must not be freed by the caller.
   */
  const char *Calculator_get_last_error();

#ifdef __cplusplus
}
#endif
