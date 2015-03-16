#pragma once
/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000-2015  Thomas Sondergaard

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
 * \brief Common header for all our catch tests.
 *
 * Zipios++ comes with a unit test suite. This header defines things
 * that all the tests access, such as the catch.hpp header file.
 */

#include <catch.hpp>


namespace zipios_test
{


class auto_unlink_t
{
public:
                            auto_unlink_t(std::string const& filename);
                            ~auto_unlink_t();

private:
    std::string const       m_filename;
};


class file_t
{
public:
    typedef std::shared_ptr<file_t>     pointer_t;
    typedef std::vector<pointer_t>      vector_t;
    typedef std::vector<std::string>    filenames_t;

    enum class type_t
    {
        UNKNOWN,
        REGULAR,
        DIRECTORY
    };

    file_t(type_t t, int children_count, std::string const& new_filename = "");
    ~file_t();
    type_t type() const;
    std::string const& filename() const;
    vector_t const& children() const;
    size_t size();
    type_t find(std::string const& name);
    filenames_t get_all_filenames() const;

private:
    void get_filenames(filenames_t& names, std::string const& parent) const;

    std::string     m_filename;
    vector_t        m_children;
    type_t          m_type;
};


} // zipios_test namespace 
// vim: ts=4 sw=4 et