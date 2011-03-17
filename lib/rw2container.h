/*
 * libopenraw - rw2container.h
 *
 * Copyright (C) 2011 Hubert Figuiere
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


#ifndef _ORF_CONTAINER_H__
#define _ORF_CONTAINER_H__

#include "ifdfilecontainer.h"

namespace OpenRaw {
namespace Internals {

static const char ORF_SUBTYPE_16BPP = 'O';
static const char ORF_SUBTYPE_12BPP = 'S';

class IOFile;

class Rw2Container
	: public IfdFileContainer
{
public:
	Rw2Container(IO::Stream *file, off_t offset);
	/** destructor */
	virtual ~Rw2Container();

	/**
		 Check the ORF magic header.
	 */
	virtual IfdFileContainer::EndianType 
	isMagicHeader(const char *p, int len);

private:
	char subtype_;
	/* avoid these being called. */
	Rw2Container(const Rw2Container &);
	Rw2Container & operator=(const Rw2Container &);
};

}
}


#endif
