/*
 * HnStatistics.cc
 *
 * DO NOT EDIT THIS FILE!
 *
 * This file is automatically generated by obj2ptr.
 * 2002/09/13 12:41:10
 */

#include "HnSRTree/HnStatistics.hh"
#include "HnSRTree/HnStatisticsObj.hh"

/*
 * HnStatistics
 */

const HnStatistics HnStatistics::null;

HnStatistics
new_HnStatistics(void)
{
    HnStatisticsObj *_obj;

    _obj = new HnStatisticsObj();

    if ( _obj->hasConstructorFailed() ) {
        delete _obj;
        return (HnStatisticsObj *)NULL;
    }

    return _obj;
}

#define HnClass HnStatistics
#include "HnSRTree/HnClassArray.cpp"

