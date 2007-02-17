/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    pages.cc - this file is part of MediaTomb.
    
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

/// \file pages.cc

#ifdef HAVE_CONFIG_H
    #include "autoconfig.h"
#endif

#include "pages.h"

using namespace zmm;

WebRequestHandler *create_web_request_handler(String page)
{
    if (page == "add") return new web::add();
    if (page == "remove") return new web::remove();
    if (page == "add_object") return new web::addObject();
    if (page == "auth") return new web::auth();
    if (page == "containers") return new web::containers();
    if (page == "directories") return new web::directories();
    if (page == "files") return new web::files();
    if (page == "items") return new web::items();
    if (page == "edit_load") return new web::edit_load();
    if (page == "edit_save") return new web::edit_save();
    if (page == "autoscan") return new web::autoscan();
    if (page == "update") return new web::update();
    if (page == "tasks") return new web::tasks();
    
    throw _Exception(_("Unknown page: ") + page);
}


