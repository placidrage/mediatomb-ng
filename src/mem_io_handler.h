/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    mem_io_handler.h - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.cc>,
                       Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>
    
    Copyright (C) 2006-2007 Gena Batyan <bgeradz@mediatomb.cc>,
                            Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>,
                            Leonhard Wimmer <leo@mediatomb.cc>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    version 2 along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    
    $Id$
*/

/// \file mem_io_handler.h
/// \brief definition of the MemIOHandler class.
#ifndef __MEM_IO_HANDLER_H__
#define __MEM_IO_HANDLER_H__

#include "common.h"
#include "io_handler.h"

/// \brief Allows the web server to read from a memory buffer instead of a file.
class MemIOHandler : public IOHandler
{
protected:
    /// \brief buffer that is holding our data.
    char *buffer;
    off_t length;

    /// \brief current offset in the buffer
    off_t   pos;
    
public:
    /// \brief Initializes the internal buffer.
    /// \param buffer all operations will be done on this buffer.
    MemIOHandler(void *buffer, int length);
    MemIOHandler(zmm::String str);
    virtual ~MemIOHandler();

    /// 
    virtual void open(IN enum UpnpOpenFileMode mode);
    virtual int read(OUT char *buf, IN size_t length);
                      
    virtual void seek(IN off_t offset, IN int whence);
};


#endif // __MEM_IO_HANDLER_H__

