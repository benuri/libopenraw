/* -*- Mode: C++ -*- */
/*
 * libopenraw - ifddir.h
 *
 * Copyright (C) 2006-2015 Hubert Figuiere
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

#ifndef OR_INTERNALS_IFDDIR_H
#define OR_INTERNALS_IFDDIR_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <exception>
#include <map>
#include <memory>
#include <vector>

#include <libopenraw/debug.h>

#include "ifdentry.hpp"
#include "trace.hpp"

namespace OpenRaw {
namespace Internals {

class IfdFileContainer;

class IfdDir {
public:
    typedef std::shared_ptr<IfdDir> Ref;
    typedef std::vector<Ref> RefVec;

    IfdDir(off_t _offset, IfdFileContainer &_container);
    virtual ~IfdDir();

    bool isPrimary() const;
    bool isThumbnail() const;

    /** return the offset */
    off_t offset() const { return m_offset; }
    const IfdFileContainer &container() const { return m_container; }

    /** load the directory to memory */
    bool load();
    /** return the number of entries*/
    int numTags() { return m_entries.size(); }
    IfdEntry::Ref getEntry(uint16_t id) const;

    /** Get a T value from an entry
     * @param id the IFD field id
     * @retval v the long value
     * @return true if success
     */
    template <typename T>
    bool getValue(uint16_t id, T &v) const
    {
        bool success = false;
        IfdEntry::Ref e = getEntry(id);
        if (e != NULL) {
            try {
                v = IfdTypeTrait<T>::get(*e);
                success = true;
            }
            catch (const std::exception &ex) {
                Debug::Trace(ERROR) << "Exception raised " << ex.what()
                                    << " fetch value for " << id << "\n";
            }
        }
        return success;
    }

    /** Get an loosely typed integer value from an entry.
     * This method is  preferred over getLongValue()
     * or getShortValue() unless you really want the strong
     * typing that IFD structure provide
     * @param id the IFD field id
     * @retval v the long value
     * @return true if success
     */
    bool getIntegerValue(uint16_t id, uint32_t &v);

    /** get the offset of the next IFD
     * in absolute
     */
    off_t nextIFD();

    /** get the SubIFD at index idx.
     * @return Ref to the new IfdDir if found
     */
    Ref getSubIFD(uint32_t idx = 0) const;
    /** get all SubIFDs
     * @retval ifds the list of IFDs Ref
     * @return true if found / success
     */
    bool getSubIFDs(std::vector<IfdDir::Ref> &ifds);

    /** get the Exif IFD.
     * @return Ref to the new IfdDir if found
     */
    Ref getExifIFD();

    /** get the MakerNote IFD.
     * @return Ref to the new MakerNoteDir if found
     */
    Ref getMakerNoteIfd();

private:
    off_t m_offset;
    IfdFileContainer &m_container;
    std::map<uint16_t, IfdEntry::Ref> m_entries;
};
}
}

#endif
