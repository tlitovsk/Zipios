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
 * \brief Implementation of zipios::ZipInputStreambuf.
 *
 * This is the implementation of the Zip input std::streambuf class.
 */

#include "zipinputstreambuf.hpp"

#include "zipios/zipiosexceptions.hpp"


namespace zipios
{


/** \class ZipInputStreambuf
 * \brief An input stream buffer for Zip data.
 *
 * The ZipInputStreambuf class is a Zip input streambuf filter that
 * automatically decompresses input data that was compressed using
 * the zlib library.
 */


/** \brief Initialize a ZipInputStreambuf.
 *
 * This ZipInputStreambuf constructor initializes the buffer from the
 * user specified buffer.
 *
 * \param[in,out] inbuf  The streambuf to use for input.
 * \param[in] start_pos  A position to reset the inbuf to before reading.
 *                       Specify -1 to read from the current position.
 */
ZipInputStreambuf::ZipInputStreambuf(std::streambuf *inbuf, offset_t start_pos)
    : InflateInputStreambuf(inbuf, start_pos)
    //, m_current_entry() -- auto-init
    //, m_remain(0) -- auto-init
{
    // read the zip local header
    std::istream is(m_inbuf); // istream does not destroy the streambuf.
    is.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);

    // if the read fails in any way it will throw
    m_current_entry.read(is);
    if(m_current_entry.isValid() && m_current_entry.hasTrailingDataDescriptor())
    {
        throw FileCollectionException("Trailing data descriptor in zip file not supported");
    }

    switch(m_current_entry.getMethod())
    {
    case StorageMethod::DEFLATED:
        reset() ; // reset inflatestream data structures
//std::cerr << "deflated" << std::endl;
        break;

    case StorageMethod::STORED:
        m_remain = m_current_entry.getSize();
        // Force underflow on first read:
        setg(&m_outvec[0], &m_outvec[0] + getBufferSize(), &m_outvec[0] + getBufferSize());
//std::cerr << "stored" << std::endl;
        break;

    default:
        // file not supported... sorry!
        throw FileCollectionException("Unsupported compression format");

    }
}


/** \fn ZipInputStreambuf::ZipInputStreambuf(ZipInputStreambuf const & src);
 * \brief The copy contructor is deleted.
 *
 * ZipInputStreambuf objects cannot be copied so the copy constructor
 * is deleted.
 *
 * \param[in] src  The source to copy.
 */



/** \brief Clean up a ZipInputStreambuf object.
 *
 * The destructor ensures that all resources get released.
 */
ZipInputStreambuf::~ZipInputStreambuf()
{
}


/** \brief Called when more data is required.
 *
 * The function ensures that at least one byte is available
 * in the input area by updating the pointers to the input area
 * and reading more data in from the input sequence if required.
 *
 * \return The value of that character on success or
 *         std::streambuf::traits_type::eof() on failure.
 */
std::streambuf::int_type ZipInputStreambuf::underflow()
{
    switch(m_current_entry.getMethod())
    {
    case StorageMethod::DEFLATED:
        // inflate class takes care of it in this case
        return InflateInputStreambuf::underflow();

    case StorageMethod::STORED:
    {
        // Ok, we are STORED, so we handle it ourselves.
        offset_t const num_b(std::min(m_remain, static_cast<offset_t>(getBufferSize())));
        std::streamsize const g(m_inbuf->sgetn(&m_outvec[0], num_b));
        setg(&m_outvec[0], &m_outvec[0], &m_outvec[0] + g);
        m_remain -= g;
        if(g > 0)
        {
            // we got some data, return it
            return traits_type::to_int_type(*gptr());
        }

        // documentation says to return EOF if no data available
        return traits_type::eof();
    }

    default:
        // This should NEVER be reached or the constructor let something
        // go through that should not have gone through
        throw std::logic_error("ZipInputStreambuf::underflow(): unknown storage method"); // LCOV_EXCL_LINE

    }
}


} // namespace

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
