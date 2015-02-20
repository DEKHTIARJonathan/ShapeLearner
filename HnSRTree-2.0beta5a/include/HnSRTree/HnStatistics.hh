/*
 * HnStatistics.hh
 *
 * DO NOT EDIT THIS FILE!
 *
 * This file is automatically generated by obj2ptr.
 * 2002/09/13 12:41:10
 */

#ifndef _HnStatistics_hh
#define _HnStatistics_hh

#ifndef _HNSRTIMP
#ifdef _MSC_VER
#define _HNSRTIMP _declspec(dllimport)
#else
#define _HNSRTIMP
#endif
#endif

#include "HnSRTree/HnPointer.hh"

/*
 * HnStatistics
 */

class _HNSRTIMP HnStatistics: public HnPointer {
private:
    friend class HnStatisticsObj;
    HnStatistics(const HnStatisticsObj *ptr) {
	HnPointer::assign((HnObject *)ptr);
    }
    HnStatisticsObj *getObject(void) const {
	return (HnStatisticsObj *)HnPointer::getObject();
    }
public:
    static const HnStatistics null;
    HnStatistics(void) {}
public:
    _HNSRTIMP friend HnStatistics
    new_HnStatistics(void);
    void addSample(double sample);
    void print(void) const;

    double getSum(void) const;
    double getAvg(void) const;
    double getVar(void) const;
    double getStd(void) const;
    double getMin(void) const;
    double getMax(void) const;
    int getNumSamples(void) const;
};

#include "HnSRTree/HnString.hh"

#define HnClass HnStatistics
#include "HnSRTree/HnClassArray.hh"

#include "HnSRTree/HnStatisticsObj.hh"

inline void
HnStatistics::addSample(double sample)
{
    getObject()->addSample(sample);
}

inline void
HnStatistics::print(void) const
{
    getObject()->print();
}

inline double
HnStatistics::getSum(void) const
{
    return getObject()->getSum();
}

inline double
HnStatistics::getAvg(void) const
{
    return getObject()->getAvg();
}

inline double
HnStatistics::getVar(void) const
{
    return getObject()->getVar();
}

inline double
HnStatistics::getStd(void) const
{
    return getObject()->getStd();
}

inline double
HnStatistics::getMin(void) const
{
    return getObject()->getMin();
}

inline double
HnStatistics::getMax(void) const
{
    return getObject()->getMax();
}

inline int
HnStatistics::getNumSamples(void) const
{
    return getObject()->getNumSamples();
}


#endif /* _HnStatistics_hh */