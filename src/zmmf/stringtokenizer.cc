/*MT*
    
    MediaTomb - http://www.mediatomb.org/
    
    stringtokenizer.cc - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.org>,
                       Sergey Bostandzhyan <jin@mediatomb.org>
    Copyright (C) 2006 Gena Batyan <bgeradz@mediatomb.org>,
                       Sergey Bostandzhyan <jin@mediatomb.org>,
                       Leonhard Wimmer <leo@mediatomb.org>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    
    $Id$
*/

/// \file stringtokenizer.cc

#ifdef HAVE_CONFIG_H
    #include "autoconfig.h"
#endif

#include "stringtokenizer.h"

#include <string.h>

using namespace zmm;

StringTokenizer::StringTokenizer(String str)
{
	this->str = str;
	pos = 0;
	len = str.length();
}
String StringTokenizer::nextToken(String seps)
{
	char *cstr = str.c_str();
	char *cseps = seps.c_str();
	while(pos < len && strchr(cseps, cstr[pos]))
		pos++;
	if(pos < len)
	{
		int start = pos;
		while(pos < len && ! strchr(cseps, cstr[pos]))
			pos++;
		return str.substring(start, pos - start);
	}
	return nil;
}

