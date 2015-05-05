/*
  Zipios++ - a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015  Made to Order Software Corporation

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/** \file
 * \brief Implementation of zipios::FilterInputStreambuf.
 *
 * This file implements the filter used to parse Zip archives.
 */

#include "filterinputstreambuf.hpp"

#include "zipios/zipiosexceptions.hpp"


namespace zipios
{

/** \class FilterInputStreambuf
 * \brief A base class to develop input stream filters.
 *
 * An input stream buffer filter is an std::streambuf that filters the
 * input it gets from the std::streambuf it is attached to.
 *
 * zipios::FilterInputStreambuf is a base class to derive input streambuf
 * filters from.
 */


/** \brief Initialize a filter input stream buffer.
 *
 * This constructor initializes the FilterInputStreambuf by saving
 * the inbuf pointer in this class.
 *
 * \param[in] inbuf  The streambuf to use for input.
 */
FilterInputStreambuf::FilterInputStreambuf(std::streambuf *inbuf)
    : m_inbuf(inbuf)
{
    if(!m_inbuf)
    {
        throw InvalidStateException("FilterInputStreambuf::FilterInputStreambuf() was called with a null streambuf pointer");
    }
}


/** \brief Clean up the object.
 *
 * At this time the destructor does nothing.
 *
 * The former version would eventually delete the m_inbuf pointer. However,
 * here we have decided that this class did not own that pointer and thus
 * should not have such permissions.
 */
FilterInputStreambuf::~FilterInputStreambuf()
{
}


} // namespace

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
