#ifndef SFMGR_H_INCLUDED
#define SFMGR_H_INCLUDED

#define BYTEORDER_LITTLE

#undef min
#undef max

#include <string>
#include <list>
#include <memory>
#include <inttypes.h>
#include <stdio.h>

namespace FSMgr
{
class Iterator;
class iDirNode;
class iFile;
class FSFile;

typedef std::unique_ptr<iFile> File;

#if defined(WIN32) && !defined(__WINE__)
constexpr const char FSD[] = "\\";
#else
constexpr const char FSD[] = "/";
#endif


class iNode
{
friend class Mgr;
friend class iDirNode;

public:
    enum NTYPE
    {
        NTYPE_NONE = 0,
        NTYPE_DIR = 1,
        NTYPE_FILE = 2
    };
    
public:
    iNode(const std::string &name, const std::string &nodepath):
    _name(name), _path(nodepath) {}

    virtual ~iNode() { Detach(); };
    
    iNode *GetParent() const { return _parent; }
    
    std::string GetRealPath() const { return _path; }; // Get real fs path
    std::string GetName() const { return _name; };
    std::string GetVPath() const  // Get virtual path constructed in memory
    {
        if (_parent != nullptr)
            return _parent->GetVPath() + FSD + _name;
        else
            return _name;
    }
    

    virtual int GetType() const { return NTYPE_NONE; };
    virtual bool IsWritable() const { return false; };
    virtual bool IsDir() const { return false; };
    virtual bool IsSeekable() const { return false; };
    virtual bool IsPhysical() const { return false; };
    virtual bool IsFile() const { return false; }; // Contain data
    
    // Physical delete. Must do destroy node
    virtual bool Delete() { delete this; return true; };
  
    virtual iNode *Detach(const std::string &name) { return nullptr; };
    virtual bool Detach(iNode *n) {return false;};
    
    virtual bool Detach()
    {
        if (!_parent)
            return true;

        return _parent->Detach(this);
    }

protected:
    std::string _name;
    std::string _path;

    iNode *_parent = nullptr;
};


class iFileNode: public iNode
{
public:
    using iNode::iNode;
    
    virtual int GetType() const { return NTYPE_FILE; };
    virtual bool IsFile() const { return true; };
    
    virtual File OpenRead(bool textMode = false) {return nullptr;};
    virtual File OpenAppend(bool textMode = false) {return nullptr;};
    virtual File OpenWrite(bool textMode = false) {return nullptr;};
};

class FSFileNode: public iFileNode
{
public:    
    using iFileNode::iFileNode;
    
    virtual bool IsSeekable() const { return true; };
    virtual bool IsPhysical() const { return true; };
    virtual bool IsWritable() const { return true; };
    
    virtual bool Delete();
    
    virtual File OpenRead(bool textMode = false);
    virtual File OpenAppend(bool textMode = false);
    virtual File OpenWrite(bool textMode = false);
    
};


class iDirNode: public iNode
{
public:
    class Iterator
    {
    public:
        Iterator(iDirNode *dr):
        _d(dr), _cur(dr->_nodes.begin())
        {}

        Iterator() = default;

        iNode *Get()
        {
            if (!_d || _cur == _d->_nodes.end())
                return nullptr;

            return *_cur;
        }
        
        void Next()
        {
            if (!_d || _cur == _d->_nodes.end())
                return;
            
            _cur++;
        }
        
        bool IsEnd() const { return _cur == _d->_nodes.end(); }

        operator bool() const  {  return _d != nullptr;  }
        iNode * operator*() { return Get(); }

    private:
        iDirNode *_d = nullptr;
        std::list<iNode *>::iterator _cur;
    };

    
    
public:    
    using iNode::iNode;
    
    virtual int GetType() const { return NTYPE_DIR; };
    virtual bool IsDir() const { return true; };
    
    virtual ~iDirNode() { Flush(); };
    
    void Flush();

    void AddNode(iNode *n)
    {
        n->Detach();
        n->_parent = this;
        _nodes.push_back(n);
    }

    iNode *GetNode(const std::string &name);
    
    virtual iDirNode *MakeDir(const std::string &name);
    
    using iNode::Delete;
    virtual bool Delete(const std::string &name);
    
    virtual iNode *Detach(const std::string &name);
    virtual bool Detach(iNode *node);
    bool Remove(const std::string &name);
    
    bool ReplaceContent(iNode *oldNode, iNode *content);
    
    void Layout(iDirNode *nod);
    
    Iterator ReadDir() { return Iterator(this); };
    
    iNode *VisitNodePath(const std::string &path, std::string *out);

    void DumpDir();
    
    // Prototypes. Works only with existing nodes
    virtual File ReadFile(const std::string &name, bool textMode = false);
    virtual File AppendFile(const std::string &name, bool textMode = false);
    virtual File WriteFile(const std::string &name, bool textMode = false);

protected:
    std::list<iNode *> _nodes;
    
    
};


class FSDirNode: public iDirNode
{
public:
    using iDirNode::iDirNode;
    
    virtual iDirNode *MakeDir(const std::string &name);
    
    virtual bool Delete();
    
    virtual bool IsPhysical() const { return true; };
    virtual bool IsWritable() const { return true; };
    
    // If not exist, create FSFileNode 
    virtual File AppendFile(const std::string &name, bool textMode = false) override;
    virtual File WriteFile(const std::string &name, bool textMode = false) override;
    
public:
    static iDirNode *ScanDir(const std::string &_name, const std::string &_path);
    static iNode *MakeNodeWithPath(const std::string &diskPath);
};








class iFile
{
public:
    virtual ~iFile() {};
    
    virtual void close() = 0;
    
    virtual size_t read(void *buf, size_t num) = 0;
    virtual size_t write(const void *buf, size_t num) = 0;

    virtual size_t tell() const = 0;
    virtual int seek(long int offset, int origin) = 0;
    
    virtual bool OK() const = 0;
    virtual bool eof() const = 0;
    
    virtual bool readErr();

    virtual uint8_t readU8();
    virtual int8_t readS8();
    virtual uint16_t readU16L();
    virtual int16_t readS16L();
    virtual uint16_t readU16B();
    virtual int16_t readS16B();
    virtual uint32_t readU32L();
    virtual int32_t readS32L();
    virtual uint32_t readU32B();
    virtual int32_t readS32B();
    virtual float readFloatL();
    virtual float readFloatB();

    virtual bool writeU8(uint8_t val);
    virtual bool writeS8(int8_t val);
    virtual bool writeU16L(uint16_t val);
    virtual bool writeS16L(int16_t val);
    virtual bool writeU16B(uint16_t val);
    virtual bool writeS16B(int16_t val);
    virtual bool writeU32L(uint32_t val);
    virtual bool writeS32L(int32_t val);
    virtual bool writeU32B(uint32_t val);
    virtual bool writeS32B(int32_t val);
    virtual bool writeFloatL(float val);
    virtual bool writeFloatB(float val);
    
protected:
    bool _ReadERR = false;
    bool _WriteERR = false;
};



class FSFile : public iFile
{
public:
    FSFile() = default;
    FSFile(const std::string &diskPath, const std::string &mode);
    virtual ~FSFile();
    
    FSFile(FSFile &&b) = default;
    FSFile& operator=(FSFile &&b) = default;
    
    FSFile(FSFile *b, bool del = true);
    
    FSFile(const FSFile&) = delete;
    FSFile& operator=(const FSFile &) = delete;

    virtual size_t read(void *buf, size_t num) override;
    virtual size_t write(const void *buf, size_t num)  override;

    virtual size_t tell() const override;
    virtual int seek(long int offset, int origin) override;
    
    virtual void close() override;
    
    virtual char *gets(char *str, int num);
    virtual int puts(const std::string &str);
    virtual int printf(const std::string &format, ...);
    virtual int vprintf(const std::string &format,va_list args);
    virtual bool ReadLine(std::string *out);

    virtual bool eof() const override;
    

    virtual bool OK() const override;

    static void closeFile(FSFile *fl) {
        if (fl) delete fl;
    };
    
    bool IsWriting() const {
        return _writeMode;
    }
    
    

protected:
    static int MyClose(FILE *f) { return fclose(f); }
    
    typedef std::unique_ptr<FILE, decltype(&MyClose)> __FPtr;
    
    __FPtr hndl = {nullptr, nullptr};
    bool _writeMode = false;
    
};




class Mgr
{    
public:
    static iDirNode *GetRoot();
    static void SetBaseDir(const std::string &_path);


    static iNode *FindNode(const std::string &path);
    static bool IsFile(const std::string &path);

    static bool ReplaceContent(const std::string &path, const std::string &diskPath);

    static bool CreateDir(const std::string &path);
    static bool DeleteDir(const std::string &path);
    
    static iDirNode::Iterator readDir(const std::string &path);
    
    static File ReadFile(iNode *nod, bool textMode = false);
    static File ReadFile(const std::string &path, bool textMode = false);
    static File AppendFile(iNode *nod, bool textMode = false);
    static File AppendFile(const std::string &path, bool textMode = false);
    static File WriteFile(iNode *nod, bool textMode = false);
    static File WriteFile(const std::string &path, bool textMode = false);
    
    static bool DeleteFile(const std::string &path);
    
    static void Dump();
    
protected:
    static iDirNode *_directories;
};


}

#endif
