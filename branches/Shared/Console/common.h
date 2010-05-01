/// \file
/// \brief Header file that describes common functions shared by many of the console classes.
/// \details See individual description.
/// \author FaTony

#pragma once

/// \brief Wrapper for realloc.
/// \tparam BUFFERTYPE Type of the buffer.
/// \tparam INTERNALTYPE Type that buffer points to.
/// \tparam SIZETYPE Type of the size.
/// \param[in,out] buffer Buffer to resize.
/// \param[in] size Size to which resize.
/// \return True on success, false otherwise.
template <typename BUFFERTYPE, typename INTERNALTYPE, typename SIZETYPE>
bool ResizeBuffer(BUFFERTYPE &buffer, const SIZETYPE size)
{
	BUFFERTYPE tempbuffer = (BUFFERTYPE)realloc(buffer, size * sizeof(INTERNALTYPE));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}