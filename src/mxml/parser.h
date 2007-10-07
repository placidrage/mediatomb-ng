/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    parser.h - this file is part of MediaTomb.
    
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
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
    
    $Id$
*/

/// \file parser.h

#ifndef __MXML_PARSER_H__
#define __MXML_PARSER_H__

#include "zmmf/zmmf.h"

#ifdef HAVE_EXPAT
#include <expat.h>
#include "zmmf/object_stack.h"
#endif
namespace mxml
{

class Element;

class Context;

#ifndef HAVE_EXPAT
    class Input;
#endif
    
class Parser : public zmm::Object
{
public:
    Parser();
    zmm::Ref<Element> parseFile(zmm::String);
    zmm::Ref<Element> parseString(zmm::String);

protected:

#ifndef HAVE_EXPAT
    zmm::Ref<Element> parse(zmm::Ref<Context> ctx, zmm::Ref<Input> input,
                            zmm::String parentTag, int state);
#else
    zmm::Ref<Element> parse(zmm::Ref<Context> ctx, zmm::String input);

    zmm::Ref<zmm::ObjectStack<Element> > elements;
    zmm::Ref<Element> root;
    zmm::Ref<Element> curEl;

    static void XMLCALL element_start(void *userdata, const char *name, const char **attrs);
    static void XMLCALL element_end(void *userdata, const char *name);
    static void XMLCALL character_data(void *userdata, const XML_Char *s, int len);
#endif

};

}

#endif // __MXML_PARSER_H__
