/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    cds_objects.cc - this file is part of MediaTomb.
    
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

/// \file cds_objects.cc

#ifdef HAVE_CONFIG_H
    #include "autoconfig.h"
#endif

#include "cds_objects.h"
#include "tools.h"
#include "storage.h"
#include "mxml/mxml.h"

using namespace zmm;
using namespace mxml;

CdsObject::CdsObject() : Object()
{
    metadata = Ref<Dictionary>(new Dictionary());
    auxdata = Ref<Dictionary>(new Dictionary());
    resources = Ref<Array<CdsResource> >(new Array<CdsResource>);
    id = INVALID_OBJECT_ID;
    parentID = INVALID_OBJECT_ID;
    refID = INVALID_OBJECT_ID;
    virt = 0;
    sortPriority = 0;
    objectFlags = OBJECT_FLAG_RESTRICTED;
}

void CdsObject::copyTo(Ref<CdsObject> obj)
{
    obj->setID(id);
    obj->setRefID(refID);
    obj->setParentID(parentID);
    obj->setTitle(title);
    obj->setClass(upnpClass);
    obj->setLocation(location);
    obj->setVirtual(virt);
    obj->setMetadata(metadata->clone());
    obj->setAuxData(auxdata->clone());
    obj->setFlags(objectFlags);
    obj->setSortPriority(sortPriority);
    for (int i = 0; i < resources->size(); i++)
        obj->addResource(resources->get(i)->clone());
}
int CdsObject::equals(Ref<CdsObject> obj, bool exactly)
{
    if (!(
        id == obj->getID() &&
        parentID == obj->getParentID() &&
        isRestricted() == obj->isRestricted() &&
        title == obj->getTitle() &&
        upnpClass == obj->getClass() &&
        sortPriority == obj->getSortPriority()
       ))
        return 0;
        
    if (! resourcesEqual(obj))
        return 0;
    
    if (! metadata->equals(obj->getMetadata()))
        return 0;
    
    if (exactly && !
        (location == obj->getLocation() &&
         virt == obj->isVirtual() &&
         auxdata->equals(obj->auxdata) &&
         objectFlags == obj->getFlags()
        ))
        return 0;
    return 1;
}

int CdsObject::resourcesEqual(Ref<CdsObject> obj)
{
    if (resources->size() != obj->resources->size())
        return 0;
    
    // compare all resources
    for (int i = 0; i < resources->size(); i++)
    {
        if (! resources->get(i)->equals(obj->resources->get(i)))
            return 0;
    }
    return 1;
}

void CdsObject::validate()
{
    if ((!string_ok(this->title)) ||
        (!string_ok(this->upnpClass)))
    {
        throw _Exception(_("CdsObject: validation failed"));
    }

}

Ref<CdsObject> CdsObject::createObject(unsigned int objectType)
{
    CdsObject *pobj;
    
    if(IS_CDS_CONTAINER(objectType))
    {
        pobj = new CdsContainer();
    }
    else if(IS_CDS_ITEM_INTERNAL_URL(objectType))
    {
        pobj = new CdsItemInternalURL();
    }
    else if(IS_CDS_ITEM_EXTERNAL_URL(objectType))
    {
        pobj = new CdsItemExternalURL();
    }
    else if(IS_CDS_ACTIVE_ITEM(objectType))
    {
        pobj = new CdsActiveItem();
    }
    else if(IS_CDS_ITEM(objectType))
    {
        pobj = new CdsItem();
    }
    else
    {
        throw _Exception(_("invalid object type: ") + objectType);
    }
    return Ref<CdsObject>(pobj);
}


/* CdsItem */

CdsItem::CdsItem() : CdsObject()
{
    objectType = OBJECT_TYPE_ITEM;
    upnpClass = _("object.item");
    mimeType = _(MIMETYPE_DEFAULT);
    trackNumber = 0;
}

void CdsItem::copyTo(Ref<CdsObject> obj)
{
    CdsObject::copyTo(obj);
    if (! IS_CDS_ITEM(obj->getObjectType()))
        return;
    Ref<CdsItem> item = RefCast(obj, CdsItem);
//    item->setDescription(description);
    item->setMimeType(mimeType);
    item->setTrackNumber(trackNumber);
}
int CdsItem::equals(Ref<CdsObject> obj, bool exactly)
{
    Ref<CdsItem> item = RefCast(obj, CdsItem);
    if (! CdsObject::equals(obj, exactly))
        return 0;
    return ((mimeType == item->getMimeType()) && 
            (trackNumber == item->getTrackNumber()));
}

void CdsItem::validate()
{
    CdsObject::validate();
//    log_info("mime: [%s] loc [%s]\n", this->mimeType.c_str(), this->location.c_str());
    if ((!string_ok(this->mimeType)) || (!check_path(this->location)))
        throw _Exception(_("CdsItem: validation failed"));
}

CdsActiveItem::CdsActiveItem() : CdsItem()
{
    objectType |= OBJECT_TYPE_ACTIVE_ITEM;

    upnpClass = _(UPNP_DEFAULT_CLASS_ACTIVE_ITEM);
    mimeType = _(MIMETYPE_DEFAULT);
}

void CdsActiveItem::copyTo(Ref<CdsObject> obj)
{
    CdsItem::copyTo(obj);
    if (! IS_CDS_ACTIVE_ITEM(obj->getObjectType()))
        return;
    Ref<CdsActiveItem> item = RefCast(obj, CdsActiveItem);
    item->setAction(action);
    item->setState(state);
}
int CdsActiveItem::equals(Ref<CdsObject> obj, bool exactly)
{
    Ref<CdsActiveItem> item = RefCast(obj, CdsActiveItem);
    if (! CdsItem::equals(obj, exactly))
        return 0;
    if (exactly &&
       (action != item->getAction() ||
        state != item->getState())
    )
        return 0;
    return 1;
}

void CdsActiveItem::validate()
{
    CdsItem::validate();
    if ((!string_ok(this->action)) || (!check_path(this->action)))
        throw _Exception(_("CdsActiveItem: validation failed"));
}
//---------

CdsItemExternalURL::CdsItemExternalURL() : CdsItem()
{
    objectType |= OBJECT_TYPE_ITEM_EXTERNAL_URL;

    upnpClass = _(UPNP_DEFAULT_CLASS_ITEM);
    mimeType = _(MIMETYPE_DEFAULT);
}

void CdsItemExternalURL::validate()
{
    CdsObject::validate();
    if ((!string_ok(this->mimeType)) || (!string_ok(this->location)))
        throw _Exception(_("CdsItemExternalURL: validation failed"));
}
//---------

CdsItemInternalURL::CdsItemInternalURL() : CdsItemExternalURL()
{
    objectType |= OBJECT_TYPE_ITEM_INTERNAL_URL;

    upnpClass = _("object.item");
    mimeType = _(MIMETYPE_DEFAULT);
}

void CdsItemInternalURL::validate()
{
    CdsObject::validate();
    if ((!string_ok(this->mimeType)) || (!string_ok(this->location)))
        throw _Exception(_("CdsItemInternalURL: validation failed"));

    if (this->location.startsWith(_("http://")))
        throw _Exception(_("CdsItemInternalURL: validation failed: must be realtive!"));
}

CdsContainer::CdsContainer() : CdsObject()
{
    objectType = OBJECT_TYPE_CONTAINER;
    updateID = 0;
    // searchable = 0; is now in objectFlags; by default all flags (except "restricted") are not set
    childCount = -1;
    upnpClass = _(UPNP_DEFAULT_CLASS_CONTAINER);
    autoscanType = OBJECT_AUTOSCAN_NONE;
}

void CdsContainer::copyTo(Ref<CdsObject> obj)
{
    CdsObject::copyTo(obj);
    if (! IS_CDS_CONTAINER(obj->getObjectType()))
        return;
    Ref<CdsContainer> cont = RefCast(obj, CdsContainer);
    cont->setUpdateID(updateID);
}
int CdsContainer::equals(Ref<CdsObject> obj, bool exactly)
{
    Ref<CdsContainer> cont = RefCast(obj, CdsContainer);
    return (
        CdsObject::equals(obj, exactly) &&
        isSearchable() == cont->isSearchable()
    );
}

void CdsContainer::validate()
{
    CdsObject::validate();
    /// \todo well.. we have to know if a container is a real directory or just a virtual container in the database
/*    if (!check_path(this->location, true))
        throw _Exception(_("CdsContainer: validation failed")); */
}
void CdsObject::optimize()
{
    metadata->optimize();
    auxdata->optimize();
    resources->optimize();
}

int CdsObjectTitleComparator(void *arg1, void *arg2)
{
    /// \todo get rid of getTitle() to avod unnecessary reference counting ops
    return strcmp(((CdsObject *)arg1)->title.c_str(),
                ((CdsObject *)arg2)->title.c_str());
}

String CdsContainer::getVirtualPath()
{
    String location;
    if (getID() == CDS_ID_ROOT)
    {
        location = _("/");
    }
    else if (getID() == CDS_ID_FS_ROOT)
    {
        Ref<Storage> storage = Storage::getInstance();
        location = _("/") + storage->getFsRootName();
    }
    else if (string_ok(getLocation()))
    {
        location = getLocation();
        if (! isVirtual())
        {
            Ref<Storage> storage = Storage::getInstance();
            location = _("/") + storage->getFsRootName() + location;
        }
    }

    if (!string_ok(location))
        throw _Exception(_("virtual location not available"));

    return location;
}

String CdsItem::getVirtualPath()
{
    Ref<Storage> storage = Storage::getInstance();
    Ref<CdsObject> cont = storage->loadObject(getParentID());
    String location = cont->getVirtualPath();
    location = location + '/' + getTitle();

    if (!string_ok(location))
        throw _Exception(_("virtual location not available"));

    return location;
}



