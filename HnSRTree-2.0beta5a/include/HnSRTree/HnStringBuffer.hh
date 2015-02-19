/*
 * HnStringBuffer.hh
 *
 * DO NOT EDIT THIS FILE!
 *
 * This file is automatically generated by obj2ptr.
 * 2002/09/13 12:41:10
 */

#ifndef _HnStringBuffer_hh
#define _HnStringBuffer_hh

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnPointer.hh"

/*
 * HnStringBuffer
 */

class _HNSRTIMP HnStringBuffer: public HnPointer {
private:
    friend class HnStringBufferObj;
    HnStringBuffer(const HnStringBufferObj *ptr) {
	HnPointer::assign((HnObject *)ptr);
    }
    HnStringBufferObj *getObject(void) const {
	return (HnStringBufferObj *)HnPointer::getObject();
    }
public:
    static const HnStringBuffer null;
    HnStringBuffer(void) {}
public:
    HnStringBuffer(const char *ptr);
    operator char *(void) const;
    HnStringBuffer &operator + (const HnString &str);
    HnStringBuffer &operator + (const char *ptr);
    HnStringBuffer &operator + (char c);
    HnStringBuffer &operator + (int value);
    HnStringBuffer &operator + (double value);
    HnStringBuffer &operator += (const HnString &str);
    HnStringBuffer &operator += (const char *ptr);
    HnStringBuffer &operator += (char c);
    HnStringBuffer &operator += (int value);
    HnStringBuffer &operator += (double value);

    _HNSRTIMP friend HnStringBuffer
    new_HnStringBuffer(void);

    const char *chars(void) const;
    int length(void) const;

    HnStringBuffer append(const char *ptr, int n);
    HnStringBuffer append(const HnString &str);
    HnStringBuffer append(const char *ptr);
    HnStringBuffer append(char c);

    operator const char *(void) const;
    HnString toString(void) const;

    HnStringBuffer reverse(void);
};

#include "HnSRTree/HnString.hh"

#define HnClass HnStringBuffer
#include "HnSRTree/HnClassArray.hh"

#include "HnSRTree/HnStringBufferObj.hh"

inline const char *
HnStringBuffer::chars(void) const
{
    return getObject()->chars();
}

inline int
HnStringBuffer::length(void) const
{
    return getObject()->length();
}

inline HnStringBuffer
HnStringBuffer::append(const char *ptr, int n)
{
    return getObject()->append(ptr, n);
}

inline HnStringBuffer
HnStringBuffer::append(const HnString &str)
{
    return getObject()->append(str);
}

inline HnStringBuffer
HnStringBuffer::append(const char *ptr)
{
    return getObject()->append(ptr);
}

inline HnStringBuffer
HnStringBuffer::append(char c)
{
    return getObject()->append(c);
}

inline HnStringBuffer::operator const char *(void) const
{
    return getObject()->operator const char *();
}

inline HnString
HnStringBuffer::toString(void) const {
    if ( isInvalid() ) {
	return "HnStringBuffer::null";
    }
    else {
	return getObject()->toString();
    }
}

inline HnStringBuffer
HnStringBuffer::reverse(void)
{
    return getObject()->reverse();
}

inline
HnStringBuffer::operator char *(void) const
{
    if ( isInvalid() ) {
	return NULL;
    }
    else {
	return (char *)chars();
    }
}

inline HnStringBuffer &
HnStringBuffer::operator + (const HnString &str)
{
    append(str);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (const char *ptr)
{
    append(ptr);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (char c)
{
    append(c);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (int value)
{
    append(HnString::valueOf(value));
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator + (double value)
{
    append(HnString::valueOf(value));
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (const HnString &str)
{
    append(str);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (const char *ptr)
{
    append(ptr);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (char c)
{
    append(c);
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (int value)
{
    append(HnString::valueOf(value));
    return *this;
}

inline HnStringBuffer &
HnStringBuffer::operator += (double value)
{
    append(HnString::valueOf(value));
    return *this;
}


#endif /* _HnStringBuffer_hh */
