#if defined(WIN32) && !defined(__WINE__)

#include <windows.h>

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif

#include "fsmgr.h"
#include <stdarg.h>
#include <string.h>

#include "common/strutils.h"

namespace FSMgr
{
iDirNode *Mgr::_directories = nullptr;




bool FSFileNode::Delete()
{
    bool result = true;
#if defined(WIN32) && !defined(__WINE__)
    if (DeleteFile(_path.c_str()) == 0)
        result = false;
#else
    if (remove(_path.c_str()) != 0)
        result = false;
#endif // WIN32
    
    delete this;
    return result;
}

File FSFileNode::OpenRead(bool textMode)
{
    if (textMode)
        return File(new FSFile(_path, "r"));
    
    return File(new FSFile(_path, "rb"));
}

File FSFileNode::OpenAppend(bool textMode)
{
    if (textMode)
        return File(new FSFile(_path, "a"));
    
    return File(new FSFile(_path, "ab"));
}

File FSFileNode::OpenWrite(bool textMode)
{
    if (textMode)
        return File(new FSFile(_path, "w"));
    
    return File(new FSFile(_path, "wb"));
}







void iDirNode::Flush()
{
    while(!_nodes.empty())
    {
        iNode *n = _nodes.front();
        _nodes.pop_front();
        
        n->_parent = nullptr;
        delete n;
    }
}

iNode *iDirNode::GetNode(const std::string &n)
{
    if ( n == "." )
        return this;
    else if ( n == ".." )
        return _parent;
    else
    {
        for (const auto &i : _nodes)
        {
            if ( !Common::StriCmp(n, i->_name) )
                return i;
        }
    }

    return nullptr;
}

iDirNode *iDirNode::MakeDir(const std::string &newDirName)
{
    if (newDirName.empty() || newDirName.find_first_of("\\/") != std::string::npos)
        return nullptr;
    
    iNode *node = GetNode(newDirName);
    if (node)
    {
        if ( node->IsDir() )
            return (iDirNode *)node;
        return nullptr;
    }
    
    std::string newPath = _path + FSD + newDirName;
    
    iDirNode *newDir = new iDirNode(newDirName, newPath);
    
    AddNode(newDir);
    return newDir;
}

bool iDirNode::Delete(const std::string &name)
{
    if (name.empty() || name.find_first_of("\\/") != std::string::npos)
        return false;
    
    iNode *node = GetNode(name);
    if (node == this)
        return false;
    
    return node->Delete();
}

iNode * iDirNode::Detach(const std::string &vname)
{
    for(std::list<iNode *>::iterator nod = _nodes.begin(); nod != _nodes.end(); nod++)
    {
        if (!Common::StriCmp((*nod)->_name, vname))
        {
            if ((*nod)->_parent == this)
                (*nod)->_parent = nullptr;
            
            _nodes.erase(nod);
            return *nod;
        }
    }
    return nullptr;
}

bool iDirNode::Detach(iNode *node)
{
    for(std::list<iNode *>::iterator nod = _nodes.begin(); nod != _nodes.end(); nod++)
    {
        if (*nod == node)
        {
            if (node->_parent == this)
                node->_parent = nullptr;
            _nodes.erase(nod);
            return true;
        }
    }
    
    return false;
}

bool iDirNode::Remove(const std::string &vname)
{
    for(std::list<iNode *>::iterator nod = _nodes.begin(); nod != _nodes.end(); nod++)
    {
        if (!Common::StriCmp((*nod)->_name, vname))
        {
            (*nod)->_parent = nullptr;
            delete *nod;
            _nodes.erase(nod);
            return true;
        }
    }
    return false;
}

bool iDirNode::ReplaceContent(iNode *oldNode, iNode *content)
{
    for (auto it = _nodes.begin(); it != _nodes.end(); it++)
    {
        if ( *it == oldNode )
        {
            content->_name = oldNode->_name;
            content->_parent = this;
            *it = content;
            
            oldNode->_parent = nullptr;

            delete oldNode;
            return true;
        }
    }
    return false;
}


void iDirNode::Layout(iDirNode *nod)
{
    _path = nod->_path;
            
    for(std::list<iNode *>::iterator it = nod->_nodes.begin(); 
        it != nod->_nodes.end(); 
        it = _nodes.erase(it))
    {
        iNode *nev = *it;
        // Will be erased from list, so do silent detach
        nev->_parent = nullptr;
        
        iNode *old = GetNode((*it)->_name);
        
        if (old)
        {
            // Do mix by override
            if ( nev->IsDir() && old->IsDir() )
            {
                dynamic_cast<iDirNode *>(old)->Layout( dynamic_cast<iDirNode *>(nev) );
                delete nev;
            }
            else // Replace
            {
                delete old;
                
                AddNode(nev);
            }
        }
        else
            AddNode(nev);
    }
}

iNode *iDirNode::VisitNodePath(const std::string &vpath, std::string *out)
{
    iDirNode *curdir = this;
    iNode *node = this;
    out->clear();

    size_t pos = 0;
    while(pos != std::string::npos)
    {
        size_t start = pos;
        pos = vpath.find_first_of("/\\", start);

        std::string vname;
        if (pos == std::string::npos)
            vname = vpath.substr(start);
        else
        {
            vname = vpath.substr(start, pos - start);
            pos++; //Next symbol
        }

        if (!vname.empty())
        {
            iNode *nd = curdir->GetNode( vname );

            if (!nd)
            {
                *out = vpath.substr(start);
                return node;
            }
            else
            {
                if (nd->IsDir())
                {
                    curdir = static_cast<iDirNode *>(nd);
                    node = nd;
                }
                else
                    return nd;
            }
        }
    }

    return node;
}

void iDirNode::DumpDir()
{
    std::string pth = GetVPath();
    printf("%s \t(%s)\n", pth.c_str(), GetRealPath().c_str());

    for (std::list<iNode *>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
    {
        if ( (*it)->GetType() == NTYPE_FILE )
        {
            printf("%s%s%s \t(%s)\n", pth.c_str(), FSD, (*it)->GetName().c_str(), (*it)->GetRealPath().c_str());
        }
    }

    for (std::list<iNode *>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
    {
        if ( (*it)->GetType() == NTYPE_DIR )
        {
            ((iDirNode *) (*it) )->DumpDir();
        }
    }
}

File iDirNode::ReadFile(const std::string &name, bool textMode)
{
    iNode *node = GetNode(name);
    
    if (!node)
        return nullptr;
    
    if (!node->IsFile())
        return nullptr;
    
    return static_cast<iFileNode *>(node)->OpenRead(textMode);
}

File iDirNode::AppendFile(const std::string &name, bool textMode)
{
    iNode *node = GetNode(name);
    
    if (!node)
        return nullptr;
    
    if (!node->IsFile())
        return nullptr;
    
    return static_cast<iFileNode *>(node)->OpenAppend(textMode);
}

File iDirNode::WriteFile(const std::string &name, bool textMode)
{
    iNode *node = GetNode(name);
    
    if (!node)
        return nullptr;
    
    if (!node->IsFile())
        return nullptr;
    
    return static_cast<iFileNode *>(node)->OpenAppend(textMode);
}





iDirNode *FSDirNode::MakeDir(const std::string &newDirName)
{
    if (newDirName.empty() || newDirName.find_first_of("\\/") != std::string::npos)
        return nullptr;
    
    iNode *node = GetNode(newDirName);
    if (node)
    {
        if ( node->IsDir() )
            return (iDirNode *)node;
        return nullptr;
    }
    
    std::string newPath = _path + FSD + newDirName;
    
    iDirNode *newDir = nullptr;
#if defined(WIN32) && !defined(__WINE__)
    if (CreateDirectory(newPath.c_str(), nullptr))
        newDir = new FSDirNode(newDirName, newPath);
#else
    if (mkdir(newPath.c_str(), 0755) == 0)
        newDir = new FSDirNode(newDirName, newPath);
#endif // WIN32

    if (!newDir)
        return nullptr;

    AddNode(newDir);
    return newDir;
}

bool FSDirNode::Delete()
{
    bool result = true;
#if defined(WIN32) && !defined(__WINE__)
    if ( !RemoveDirectory(_path.c_str()) )
        result = false;
#else
    if (rmdir(_path.c_str()) == -1)
        result = false;
#endif // WIN32
    delete this;
    return result;
}


File FSDirNode::AppendFile(const std::string &name, bool textMode)
{
    iNode *node = GetNode(name);
    
    if (!node)
    {
        node = new FSFileNode(name, _path + FSD + name);
        
        AddNode(node);
    }
    
    if (!node->IsFile())
        return nullptr;
    
    return static_cast<iFileNode *>(node)->OpenAppend(textMode);
}

File FSDirNode::WriteFile(const std::string &name, bool textMode)
{
    iNode *node = GetNode(name);
    
    if (!node)
    {
        node = new FSFileNode(name, _path + FSD + name);
        
        AddNode(node);
    }
    
    if (!node->IsFile())
        return nullptr;
    
    return static_cast<iFileNode *>(node)->OpenAppend(textMode);
}


#if defined(WIN32) && !defined(__WINE__)
iDirNode *FSDirNode::ScanDir(const std::string &_name, const std::string &_path)
{
    std::string tmp = _path + "\\*";

    WIN32_FIND_DATA fdata;
    HANDLE hf = FindFirstFile(tmp.c_str(), &fdata);
    if (hf == INVALID_HANDLE_VALUE)
        return nullptr;

    tmp.pop_back(); // delete *

    iDirNode *ndr = new iDirNode(_name, _path);

    do
    {
        if ( strcmp(fdata.cFileName, ".") != 0 && strcmp(fdata.cFileName, "..") != 0 )
        {
            std::string tmp2 = tmp + fdata.cFileName;

            if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                iNode *sub = ScanDir(fdata.cFileName, tmp2);
                if (sub)
                    ndr->addNode(sub);
            }
            else
            {
                ndr->addNode(new iFileNode(fdata.cFileName, tmp2));
            }
        }
    }
    while ( FindNextFile(hf, &fdata) != 0);

    FindClose(hf);

    return ndr;
}
#else
iDirNode *FSDirNode::ScanDir(const std::string &_name, const std::string &_path)
{
    DIR *dr = opendir(_path.c_str());
    if (!dr)
        return nullptr;

    iDirNode *ndr = new iDirNode(_name, _path);

    std::string tmp = _path + "/";

    for (dirent *ent = readdir(dr); ent; ent = readdir(dr))
    {
        if (ent->d_type == DT_REG || ent->d_type == DT_DIR)
        {
            if ( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 )
            {
                std::string tmp2 = tmp + ent->d_name;

                if (ent->d_type == DT_REG)
                {
                    ndr->AddNode(new FSFileNode(ent->d_name, tmp2) );
                }
                else
                {
                    iNode *sub = ScanDir(ent->d_name, tmp2);
                    if (sub)
                        ndr->AddNode(sub);
                }
            }
        }
    }

    closedir(dr);

    return ndr;
}
#endif // WIN32


iNode *FSDirNode::MakeNodeWithPath(const std::string &diskPath)
{
    iNode *node = nullptr;

    std::string tmp = diskPath;
    if (!tmp.empty() && (tmp.back() == '\\' || tmp.back() == '/') )
        tmp.pop_back();

    if (tmp.empty())
        tmp = ".";

    std::string tmpName = tmp;

    size_t ps = tmpName.find_last_of("\\/");

    if (ps != std::string::npos)
        tmpName = tmpName.substr(ps + 1);

#if defined(WIN32) && !defined(__WINE__)
    DWORD attr = GetFileAttributes(diskPath.c_str());
    if ( attr != INVALID_FILE_ATTRIBUTES)
    {
        if ( attr & FILE_ATTRIBUTE_DIRECTORY )
            node = ScanDir(tmpName, tmp);
        else
            node = new FSFileNode(tmpName, tmp);
    }
#else
    struct stat attr;

    if ( stat(diskPath.c_str(), &attr) == 0)
    {
        if ( S_ISDIR(attr.st_mode) )
            node = ScanDir(tmpName, tmp);
        else if ( S_ISREG(attr.st_mode) )
            node = new FSFileNode(tmpName, tmp);
    }
#endif // WIN32

    return node;
}





FSFile::FSFile(const std::string &diskPath, const std::string &mode)
{
    hndl = __FPtr( fopen(diskPath.c_str(), mode.c_str()), &MyClose );
    
    if (mode.find("w") != std::string::npos)
        _writeMode = true;
}

FSFile::FSFile(FSFile *b, bool del)
{
    if (b)
    {
        *this = std::move(*b);
        
        if (del)
            delete b;
    }
}


FSFile::~FSFile()
{
}

bool FSFile::OK() const
{
    if (hndl)
        return true;
    else
        return false;
}

bool FSFile::eof() const
{
    if (!hndl)
        return true;

    return feof(hndl.get()) != 0;
}



size_t FSFile::read(void *buf, size_t num)
{
    if (!hndl)
        return 0;

    size_t sz = fread(buf, 1, num, hndl.get());

    if (sz != num)
        _ReadERR = true;

    return sz;
}

size_t FSFile::write(const void *buf, size_t num)
{
    if (!hndl)
        return 0;

    size_t sz = fwrite(buf, 1, num, hndl.get());
    if (sz != num)
        _WriteERR = true;

    return sz;
}

size_t FSFile::tell() const
{
    if (!hndl)
        return 0;

    return ftell(hndl.get());
}

int FSFile::seek(long int offset, int origin)
{
    if (!hndl)
        return -100;

    return fseek(hndl.get(), offset, origin);
}

void FSFile::close()
{
    hndl.reset();
}

char *FSFile::gets(char *str, int num)
{
    if (!hndl)
        return nullptr;

    return fgets(str, num, hndl.get());
}

int FSFile::puts(const std::string &str)
{
    if (!hndl)
        return -100;

    return fputs(str.c_str(), hndl.get());
}

int FSFile::printf(const std::string &format, ...)
{
    if (!hndl)
        return -100;

    va_list args;
    va_start (args, format);

    int num = vfprintf(hndl.get(), format.c_str(), args);

    va_end (args);

    return num;
}

int FSFile::vprintf(const std::string &format, va_list args)
{
    if (!hndl)
        return -100;

    int num = vfprintf(hndl.get(), format.c_str(), args);
    return num;
}


bool FSFile::ReadLine(std::string *out)
{
	if (!hndl)
        return false;

	out->clear();
	char buf[256];
	bool ok = false;

	while(fgets(buf, 256, hndl.get()))
	{
		ok = true;
		(*out) += buf;
		if (out->back() == '\n' || out->back() == '\r')
			break;
	}
	return ok;
}

bool iFile::readErr()
{
    bool tmp = _ReadERR;
    _ReadERR = false;
    return tmp;
}

uint8_t iFile::readU8()
{
    if (!OK())
        return 0;

    uint8_t val = 0;
    if (read(&val, 1) != 1)
        _ReadERR = true;
    return val;
}

int8_t iFile::readS8()
{
    if (!OK())
        return 0;

    int8_t val = 0;
    if (read(&val, 1) != 1)
        _ReadERR = true;
    return val;
}

uint16_t iFile::readU16L()
{
    if (!OK())
        return 0;

    uint16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
}

int16_t iFile::readS16L()
{
    if (!OK())
        return 0;

    int16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
}

uint16_t iFile::readU16B()
{
    if (!OK())
        return 0;

    uint16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

int16_t iFile::readS16B()
{
    if (!OK())
        return 0;

    int16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

uint32_t iFile::readU32L()
{
    if (!OK())
        return 0;

    uint32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
}

int32_t iFile::readS32L()
{
    if (!OK())
        return 0;

    int32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
}

uint32_t iFile::readU32B()
{
    if (!OK())
        return 0;

    uint32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

int32_t iFile::readS32B()
{
    if (!OK())
        return 0;

    int32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

float iFile::readFloatL()
{
    if (!OK())
        return 0.0;

    float val = 0.0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return val;
}

float iFile::readFloatB()
{
    if (!OK())
        return 0.0;

    float val = 0.0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return val;
}

bool iFile::writeU8(uint8_t val)
{
    if (!OK())
        return false;

    return write(&val, 1) == 1;
}

bool iFile::writeS8(int8_t val)
{
    if (!OK())
        return false;

    return write(&val, 1) == 1;
}

bool iFile::writeU16L(uint16_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFile::writeS16L(int16_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFile::writeU16B(uint16_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFile::writeS16B(int16_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFile::writeU32L(uint32_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFile::writeS32L(int32_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFile::writeU32B(uint32_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFile::writeS32B(int32_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFile::writeFloatL(float val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFile::writeFloatB(float val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}



iDirNode *Mgr::GetRoot()
{
    return _directories;
}

void Mgr::SetBaseDir(const std::string &_path)
{
    std::string tmp = _path;

    if (!tmp.empty() && (tmp.back() == '\\' || tmp.back() == '/') )
        tmp.pop_back();

    if (tmp.empty())
        tmp = ".";

    if (_directories)
        delete _directories;

    _directories = FSDirNode::ScanDir("", tmp);
}

iNode *Mgr::FindNode(const std::string &path)
{
    if (!_directories)
        return nullptr;
    
    if (path.empty())
        return _directories;
        
    std::string leaved;
    iNode *node = _directories->VisitNodePath(path, &leaved);

    if (!node)
        return nullptr;

    if ( !leaved.empty() ) //If not exist
        return nullptr;

    return node;
}

bool Mgr::IsFile(const std::string &path)
{
    
    iNode *tmp = FindNode(path);

    if (!tmp)
        return false;

    if (!tmp->IsFile())
        return false;

    return true;
}

bool Mgr::ReplaceContent(const std::string &path, const std::string &diskPath)
{
    if (!_directories)
        return false;
    
    std::string leaved;
    iNode *oldNode = _directories->VisitNodePath(path, &leaved);

    if (!oldNode || !leaved.empty())
        return false;

    if (oldNode->GetParent() == nullptr) //It's root node, we can't replace it
        return false;

    iNode *newNode = FSDirNode::MakeNodeWithPath(diskPath);

    if (!newNode)
        return false;

    iNode *upNode = oldNode->GetParent();
    
    if (!upNode->IsDir())
        return false;
    
    if (static_cast<iDirNode *>(upNode)->ReplaceContent(oldNode, newNode))
        return true;
    
    delete newNode;
    return false;    
}

bool Mgr::CreateDir(const std::string &path)
{
    if (!_directories)
        return false;
    
    std::string leaved;
    iNode *node = _directories->VisitNodePath(path, &leaved);

    if (node)
    {
        if ( node->IsDir() )
            return false;
        
        iDirNode *nodeDir = static_cast<iDirNode *>(node);

        if ( !leaved.empty() ) //If not exist
        {
            if (leaved.length() > 1 && (leaved.back() == '\\' || leaved.back() == '/') )
                leaved.pop_back();

            if ( leaved.find_first_of("\\/") == std::string::npos )
            {
                iDirNode *newDir = nodeDir->MakeDir(leaved);

                if (!newDir)
                    return false;

                ((iDirNode *)node)->AddNode(newDir);
            }
            else
                return false;
        }
    }
    else
        return false;

    return true;
}


bool Mgr::DeleteDir(const std::string &path)
{
    if (!_directories)
        return false;
    
    std::string leaved;
    iNode *node = _directories->VisitNodePath(path, &leaved);

    if (!node)
        return false;

    if ( !node->IsDir() )
        return false;

    if ( !leaved.empty() ) //If not exist
        return false;

    if ( node->GetParent() == nullptr )
        return false;

    return node->Delete();    
}

iDirNode::Iterator Mgr::readDir(const std::string &path)
{
    if (!_directories)
        return iDirNode::Iterator();
    
    std::string leaved;
    iNode *node = _directories->VisitNodePath(path, &leaved);

    if (!node || !node->IsDir() )
        return iDirNode::Iterator();

    if ( !leaved.empty() ) //If not exist
        return iDirNode::Iterator();

    return static_cast<iDirNode *>(node)->ReadDir();
}

File Mgr::ReadFile(iNode *nod, bool textMode)
{
    if (!nod)
        return nullptr;

    if ( !nod->IsFile() )
        return nullptr;

    return static_cast<iFileNode*>(nod)->OpenRead(textMode);
}

File Mgr::ReadFile(const std::string &path, bool textMode)
{
    return ReadFile( FindNode(path), textMode);
}

File Mgr::AppendFile(iNode *nod, bool textMode)
{
    if (!nod)
        return nullptr;

    if ( !nod->IsFile() )
        return nullptr;

    return static_cast<iFileNode*>(nod)->OpenAppend(textMode);
}

File Mgr::AppendFile(const std::string &path, bool textMode)
{
    if (!_directories)
        return nullptr;
    
    std::string leaved;
    iNode *node = _directories->VisitNodePath(path, &leaved);

    if (!node)
        return nullptr;
    
    if ( !leaved.empty() ) // Not exists
    {
        if ( !node->IsDir() ) // If node not dir
            return nullptr;

        if ( leaved.find_first_of("\\/") != std::string::npos ) // With path
            return nullptr;

        return static_cast<iDirNode *>(node)->AppendFile(leaved, textMode);
    }

    return AppendFile(node, textMode);
}

File Mgr::WriteFile(iNode *nod, bool textMode)
{
    if (!nod)
        return nullptr;

    if ( !nod->IsFile() )
        return nullptr;

    return static_cast<iFileNode*>(nod)->OpenWrite(textMode);
}

File Mgr::WriteFile(const std::string &path, bool textMode)
{
    if (!_directories)
        return nullptr;
    
    std::string leaved;
    iNode *node = _directories->VisitNodePath(path, &leaved);

    if (!node)
        return nullptr;
    
    if ( !leaved.empty() ) // Not exists
    {
        if ( !node->IsDir() ) // If node not dir
            return nullptr;

        if ( leaved.find_first_of("\\/") != std::string::npos ) // With path
            return nullptr;

        return static_cast<iDirNode *>(node)->WriteFile(leaved, textMode);
    }

    return WriteFile(node, textMode);
}


bool Mgr::DeleteFile(const std::string &path)
{
    iNode *node = FindNode(path);

    if (!node)
        return false;

    if (!node->IsFile())
        return false;

    return node->Delete();;
}

void Mgr::Dump()
{
    if (_directories)
        _directories->DumpDir();
}

}
