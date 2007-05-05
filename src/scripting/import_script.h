/*  scripting.h - this file is part of MediaTomb.
                                                                                
    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>
                                                                                
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
*/

#ifndef __SCRIPTING_IMPORT_SCRIPT_H__
#define __SCRIPTING_IMPORT_SCRIPT_H__

#include "common.h"
#include "script.h"
#include "cds_objects.h"

class ImportScript : public Script
{
public:
	ImportScript(zmm::Ref<Runtime> runtime);
	void processCdsObject(zmm::Ref<CdsObject> obj);	
    
    zmm::Ref<CdsObject> jsObject2cdsObject(JSObject *js);
protected:
    void cdsObject2jsObject(zmm::Ref<CdsObject> obj, JSObject *js);
    zmm::Ref<Script> runScript;

};

#endif // __SCRIPTING_IMPORT_SCRIPT_H__

