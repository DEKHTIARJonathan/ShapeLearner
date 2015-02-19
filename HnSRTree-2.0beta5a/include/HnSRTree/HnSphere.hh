/*
 * HnSphere.hh
 *
 * DO NOT EDIT THIS FILE!
 *
 * This file is automatically generated by obj2ptr.
 * 2002/09/13 12:44:53
 */

#ifndef _HnSphere_hh
#define _HnSphere_hh

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnPointer.hh"

#include "HnSRTree/HnPoint.hh"

/*
 * HnSphere
 */

class _HNSRTIMP HnSphere: public HnPointer {
private:
    friend class HnSphereObj;
    HnSphere(const HnSphereObj *ptr) {
	HnPointer::assign((HnObject *)ptr);
    }
    HnSphereObj *getObject(void) const {
	return (HnSphereObj *)HnPointer::getObject();
    }
public:
    static const HnSphere null;
    HnSphere(void) {}
public:
    _HNSRTIMP friend HnSphere
    new_HnSphere(const HnPoint &center, double radius);
    const HnPoint &getCenter(void) const;
    double getRadius(void) const;
    int getDimension(void) const;

    HnBool equals(const HnSphere &sphere) const;
    HnString toString(void) const;

    /* utilities */
    HnBool includes(const HnPoint &point) const;
    HnBool overlaps(const HnSphere &sphere) const;
    double getVolume(void) const;

    double getMinDistance(const HnPoint &point) const;
    double getMaxDistance(const HnPoint &point) const;

    double getLowerBoundMinDistance(const HnPoint &point) const;
    double getUpperBoundMaxDistance(const HnPoint &point) const;
};

#include "HnSRTree/HnString.hh"

#define HnClass HnSphere
#include "HnSRTree/HnClassArray.hh"

#include "HnSRTree/HnSphereObj.hh"

inline const HnPoint &
HnSphere::getCenter(void) const
{
    return getObject()->getCenter();
}

inline double
HnSphere::getRadius(void) const
{
    return getObject()->getRadius();
}

inline int
HnSphere::getDimension(void) const
{
    return getObject()->getDimension();
}

inline HnBool
HnSphere::equals(const HnSphere &ptr) const {
    if ( isInvalid() ) {
	if ( ptr.isInvalid() ) {
	    return HnTRUE;
	}
	else {
	    return HnFALSE;
	}
    }
    else {
	if ( ptr.isInvalid() ) {
	    return HnFALSE;
	}
	else {
	    return getObject()->equals(ptr);
	}
    }
}

inline HnString
HnSphere::toString(void) const {
    if ( isInvalid() ) {
	return "HnSphere::null";
    }
    else {
	return getObject()->toString();
    }
}

inline HnBool
HnSphere::includes(const HnPoint &point) const
{
    return getObject()->includes(point);
}

inline HnBool
HnSphere::overlaps(const HnSphere &sphere) const
{
    return getObject()->overlaps(sphere);
}

inline double
HnSphere::getVolume(void) const
{
    return getObject()->getVolume();
}

inline double
HnSphere::getMinDistance(const HnPoint &point) const
{
    return getObject()->getMinDistance(point);
}

inline double
HnSphere::getMaxDistance(const HnPoint &point) const
{
    return getObject()->getMaxDistance(point);
}

inline double
HnSphere::getLowerBoundMinDistance(const HnPoint &point) const
{
    return getObject()->getLowerBoundMinDistance(point);
}

inline double
HnSphere::getUpperBoundMaxDistance(const HnPoint &point) const
{
    return getObject()->getUpperBoundMaxDistance(point);
}


#endif /* _HnSphere_hh */
