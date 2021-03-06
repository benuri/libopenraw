/*
 * libopenraw - rawcontainer.cpp
 *
 * Copyright (C) 2006-2016 Hubert Figuiere
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <fcntl.h>
#include <memory>

#include <libopenraw/debug.h>

#include "trace.hpp"
#include "endianutils.hpp"
#include "rawcontainer.hpp"

using namespace Debug;

namespace OpenRaw {
namespace Internals {


RawContainer::RawContainer(const IO::Stream::Ptr &_file, off_t _offset)
  : m_file(_file),
    m_offset(_offset),
    m_endian(ENDIAN_NULL)
{
  m_file->open();
  m_file->seek(_offset, SEEK_SET);
}


RawContainer::~RawContainer()
{
  m_file->close();
}


bool RawContainer::readInt8(const IO::Stream::Ptr &f, int8_t & v)
{
  unsigned char buf;
  int s = f->read(&buf, 1);
  if (s != 1) {
    return false;
  }
  v = buf;
  return true;
}

bool RawContainer::readUInt8(const IO::Stream::Ptr &f, uint8_t & v)
{
  unsigned char buf;
  int s = f->read(&buf, 1);
  if (s != 1) {
    return false;
  }
  v = buf;
  return true;
}

bool 
RawContainer::readInt16(const IO::Stream::Ptr &f, int16_t & v)
{
  if (m_endian == ENDIAN_NULL) {
    
    Trace(ERROR) << "null endian\n";
    
    return false;
  }
  unsigned char buf[2];
  int s = f->read(buf, 2);
  if (s != 2) {
    return false;
  }
  if (m_endian == ENDIAN_LITTLE) {
    v = EL16(buf);
  }
  else {
    v = BE16(buf);
  }
  return true;
}


bool 
RawContainer::readInt32(const IO::Stream::Ptr &f, int32_t & v)
{
  if (m_endian == ENDIAN_NULL) {
    
    Trace(ERROR) << "null endian\n";
    
    return false;
  }
  unsigned char buf[4];
  int s = f->read(buf, 4);
  if (s != 4) {
    Trace(ERROR) << "read " << s << " bytes\n";
    return false;
  }

  if (m_endian == ENDIAN_LITTLE) {
    v = EL32(buf);
  }
  else {
    v = BE32(buf);
  }

  return true;
}


bool 
RawContainer::readUInt16(const IO::Stream::Ptr &f, uint16_t & v)
{
  if (m_endian == ENDIAN_NULL) {

    Trace(ERROR) << "null endian\n";

    return false;
  }
  unsigned char buf[2];
  int s = f->read(buf, 2);
  if (s != 2) {
    return false;
  }
  if (m_endian == ENDIAN_LITTLE) {
    v = EL16(buf);
  }
  else {
    v = BE16(buf);
  }
  return true;
}


bool 
RawContainer::readUInt32(const IO::Stream::Ptr &f, uint32_t & v)
{
  if (m_endian == ENDIAN_NULL) {

    Trace(ERROR) << "null endian\n";

    return false;
  }
  unsigned char buf[4];
  int s = f->read(buf, 4);
  if (s != 4) {
    return false;
  }

  if (m_endian == ENDIAN_LITTLE) {
    v = EL32(buf);
  }
  else {
    v = BE32(buf);
  }

  return true;
}


size_t 
RawContainer::fetchData(void *buf, off_t _offset,
                        size_t buf_size)
{
  size_t s;
  m_file->seek(_offset, SEEK_SET);
  s = m_file->read(buf, buf_size);
  return s;
}


}
}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  tab-width:2
  c-basic-offset:2
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
