/*MT*
    
    MediaTomb - http://www.mediatomb.org/
    
    sql_storage.h - this file is part of MediaTomb.
    
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

/// \file sql_storage.h

#ifndef __SQL_STORAGE_H__
#define __SQL_STORAGE_H__

#include "zmmf/zmmf.h"
#include "cds_objects.h"
#include "dictionary.h"
#include "storage.h"
#include "hash.h"
#include "sync.h"

#define QTB                 table_quote_begin
#define QTE                 table_quote_end

#define CDS_OBJECT_TABLE            "mt_cds_object"
#define CDS_ACTIVE_ITEM_TABLE       "mt_cds_active_item"
#define INTERNAL_SETTINGS_TABLE     "mt_internal_setting"
#define AUTOSCAN_TABLE              "mt_autoscan"

class SQLResult;

class SQLRow : public zmm::Object
{
public:
    SQLRow(zmm::Ref<SQLResult> sqlResult);
    virtual ~SQLRow();
    virtual zmm::String col(int index) = 0;
protected:
    zmm::Ref<SQLResult> sqlResult;
};

class SQLResult : public zmm::Object
{
public:
    SQLResult();
    virtual ~SQLResult();
    virtual zmm::Ref<SQLRow> nextRow() = 0;
    virtual unsigned long long getNumRows() = 0;
};

class SQLStorage : public Storage
{
public:
    SQLStorage();
    virtual ~SQLStorage();
    
    virtual void init();
    
    /* methods to override in subclasses */
    virtual zmm::String quote(zmm::String str) = 0;
    virtual zmm::String quote(int val) = 0;
    virtual zmm::String quote(unsigned int val) = 0;
    virtual zmm::String quote(long val) = 0;
    virtual zmm::String quote(unsigned long val) = 0;
    virtual zmm::Ref<SQLResult> select(zmm::String query) = 0;
    virtual int exec(zmm::String query, bool getLastInsertId = false) = 0;
    
    virtual void addObject(zmm::Ref<CdsObject> object, int *changedContainer);
    virtual void updateObject(zmm::Ref<CdsObject> object, int *changedContainer);
    
    virtual zmm::Ref<CdsObject> loadObject(int objectID);
    virtual int getChildCount(int contId, bool containers, bool items);
    
    //virtual zmm::Ref<zmm::Array<CdsObject> > selectObjects(zmm::Ref<SelectParam> param);
    
    virtual zmm::Ref<DBRHash<int> > getObjects(int parentID);
    
    virtual bool removeObjects(zmm::Ref<DBRHash<int> > list);
    
    virtual int removeObject(int objectID, bool all, int *objectType = NULL);
    
    /* accounting methods */
    virtual int getTotalFiles();
    
    virtual zmm::Ref<zmm::Array<CdsObject> > browse(zmm::Ref<BrowseParam> param);
    virtual zmm::Ref<zmm::Array<zmm::StringBase> > getMimeTypes();
    
    //virtual zmm::Ref<CdsObject> findObjectByTitle(zmm::String title, int parentID);
    virtual zmm::Ref<CdsObject> findObjectByPath(zmm::String fullpath);
    virtual int findObjectIDByPath(zmm::String fullpath);
    virtual zmm::String incrementUpdateIDs(int *ids, int size);
    
    virtual zmm::String buildContainerPath(int parentID, zmm::String title);
    
    virtual void addContainerChain(zmm::String path, int *containerID, int *updateID);
    
    virtual zmm::String getInternalSetting(zmm::String key);
    virtual void storeInternalSetting(zmm::String key, zmm::String value) = 0;
    
    virtual zmm::Ref<AutoscanList> getAutoscanList(scan_mode_t scanmode);
    virtual void addAutoscanDirectory(zmm::Ref<AutoscanDirectory> adir);
    virtual void removeAutoscanDirectory(int objectId);
    virtual bool isAutoscanDirectory(int objectId);
    virtual void autoscanUpdateLM(zmm::Ref<AutoscanDirectory> adir);
    
    virtual void shutdown() = 0;
    
    virtual int ensurePathExistence(zmm::String path, int *changedContainer);
protected:
    
    char table_quote_begin;
    char table_quote_end;
    
    zmm::String sql_query;
    
    /* helper for createObjectFromRow() */
    zmm::String getRealLocation(int parentID, zmm::String location);
    
    zmm::Ref<CdsObject> createObjectFromRow(zmm::Ref<SQLRow> row);
    
    /* helper for findObjectByPath and findObjectIDByPath */ 
    zmm::Ref<SQLRow> _findObjectByPath(zmm::String fullpath);
    
    int _ensurePathExistence(zmm::String path, int *changedContainer);
    
    /* helper class and helper function for addObject and updateObject */
    class AddUpdateTable : public Object
    {
    public:
        AddUpdateTable(zmm::String table, zmm::Ref<Dictionary> dict)
        {
            this->table = table;
            this->dict = dict;
        }
        zmm::String getTable() { return table; }
        zmm::Ref<Dictionary> getDict() { return dict; }
    protected:
        zmm::String table;
        zmm::Ref<Dictionary> dict;
    };
    zmm::Ref<zmm::Array<AddUpdateTable> > _addUpdateObject(zmm::Ref<CdsObject> obj, bool isUpdate, int *changedContainer);
    
    
    /* helper for removeObject(s) */
    void _removeObjects(zmm::String objectIDs);
    void _recursiveRemove(zmm::String objectIDs);
    
    
    /*
    zmm::String selectQueryBasic;
    zmm::String selectQueryExtended;
    zmm::String selectQueryFull;
    */
    
    /* helpers for removeObject() */
    
    int *rmIDs;
    zmm::Ref<DBHash<int> > rmIDHash;
    int *rmParents;
    zmm::Ref<DBBHash<int, int> > rmParentHash;
    
    bool dbRemovesDeps;
    
    /* location hash helpers */
    zmm::String addLocationPrefix(char prefix, zmm::String path);
    zmm::String stripLocationPrefix(char* prefix, zmm::String path);
    zmm::String stripLocationPrefix(zmm::String path);
    
    zmm::Ref<CdsObject> checkRefID(zmm::Ref<CdsObject> obj);
    int createContainer(int parentID, zmm::String name, zmm::String path, bool isVirtual);
    
    zmm::String mapBool(bool val) { return quote((val ? 1 : 0)); }
    bool remapBool(zmm::String field) { return (string_ok(field) && field == "1"); }
    
    /*
    void rmInit();
    void rmCleanup();
    void rmDeleteIDs();
    void rmUpdateParents();
    void rmFlush();
    void rmObject(zmm::Ref<CdsObject> obj);
    void rmItem(zmm::Ref<CdsObject> obj);
    void rmChildren(zmm::Ref<CdsObject> obj);
    void rmDecChildCount(zmm::Ref<CdsObject> obj);
    */
    
    //zmm::Ref<DSOHash<CdsObject> > objectTitleCache;
    //zmm::Ref<DBOHash<int, CdsObject> > objectIDCache;
    
    //int nextObjectID;
};

#endif // __SQL_STORAGE_H__

