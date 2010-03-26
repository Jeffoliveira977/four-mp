/// \file
/// \brief Header file that describes common functions shared by many of the console classes.
/// \details See individual description.
/// \author FaTony

#pragma once

/// \brief Wrapper for realloc.
/// \param[in,out] buffer Buffer to resize.
/// \param[in] size Size to which resize.
/// \return True on success, false otherwise.
bool ResizeArrayBuffer(char **&buffer, const unsigned short size);

/// \brief Wrapper for realloc.
/// \param[in,out] buffer Buffer to resize.
/// \param[in] size Size to which resize.
/// \return True on success, false otherwise.
bool ResizeStringBuffer(char *&buffer, const unsigned int size);