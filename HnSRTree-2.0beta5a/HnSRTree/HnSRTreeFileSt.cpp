/*
 * HnSRTreeFileSt.cc
 * Copyright (C) 1999 Norio Katayama
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA
 *
 * 07/27/1999 Norio KATAYAMA
 * $Id: HnSRTreeFileSt.cc,v 1.6 2002/09/13 11:54:15 katayama Exp $
 */

#include <stdlib.h>
#include "HnSRTree/HnSRTreeFileSt.h"
#include "HnSRTree/HnSRTreeFile.hh"

/*
 * conversion functions
 */

/* HnPoint */
static HnPoint pointStToPoint(const HnPointSt *pointSt);
static HnPointSt *pointToPointSt(const HnPoint &point);
static HnPointVector
pointVectorStToPointVector(const HnPointVectorSt *pointsSt);
static HnPointVectorSt *
pointVectorToPointVectorSt(const HnPointVector &points);

/* HnDataItem */
static HnDataItem dataItemStToDataItem(const HnDataItemSt *dataItemSt);
static HnDataItemSt *dataItemToDataItemSt(const HnDataItem &dataItem);
static HnDataItemVector
dataItemVectorStToDataItemVector(const HnDataItemVectorSt *dataItemsSt);
static HnDataItemVectorSt *
dataItemVectorToDataItemVectorSt(const HnDataItemVector &dataItems);

/* HnSphere */
static HnSphere sphereStToSphere(const HnSphereSt *sphereSt);

/* HnRect */
static HnRect rectStToRect(const HnRectSt *rectSt);

/* HnProperties */
static HnProperties
propertiesStToProperties(const HnPropertiesSt *propertiesSt);
static void
propertiesToPropertiesSt(const HnProperties &properties,
			 HnPropertiesSt *propertiesSt_return);

/*
 * Functions
 */

HnSRTreeFileSt *
HnSRTreeFileSt_create(const char *path, int dimension, int dataItemSize,
		      const HnPropertiesSt *propertiesSt)
{
    HnSRTreeFile *file;
    HnProperties properties;
    HnSRTreeFileSt *fileSt;

    file = new HnSRTreeFile();

    if ( propertiesSt == NULL ) {
	properties = HnProperties::null;
    }
    else {
	properties = propertiesStToProperties(propertiesSt);
    }

    *file = new_HnSRTreeFile(path, dimension, dataItemSize, properties);

    if ( *file == HnSRTreeFile::null ) {
	delete file;
	return NULL;
    }

    fileSt = (HnSRTreeFileSt *)HnMalloc(sizeof(HnSRTreeFileSt));
    fileSt->opaque = file;

    return fileSt;
}

HnSRTreeFileSt *
HnSRTreeFileSt_open(const char *path, const char *mode)
{
    HnSRTreeFile *file;
    HnSRTreeFileSt *fileSt;

    file = new HnSRTreeFile();

    *file = new_HnSRTreeFile(path, mode);

    if ( *file == HnSRTreeFile::null ) {
	delete file;
	return NULL;
    }

    fileSt = (HnSRTreeFileSt *)HnMalloc(sizeof(HnSRTreeFileSt));
    fileSt->opaque = file;

    return fileSt;
}

HnSRTreeFileSt *
HnSRTreeFileSt_build(const char *path,
		     int dimension, int dataItemSize, 
		     const HnPointVectorSt *pointsSt,
		     const HnDataItemVectorSt *dataItemsSt,
		     const HnPropertiesSt *propertiesSt)
{
    HnSRTreeFile *file;
    HnPointVector points;
    HnDataItemVector dataItems;
    HnProperties properties;
    HnSRTreeFileSt *fileSt;

    points = pointVectorStToPointVector(pointsSt);
    dataItems = dataItemVectorStToDataItemVector(dataItemsSt);

    if ( propertiesSt == NULL ) {
	properties = HnProperties::null;
    }
    else {
	properties = propertiesStToProperties(propertiesSt);
    }

    file = new HnSRTreeFile();

    *file = new_HnSRTreeFile(path, dimension, dataItemSize,
			     points, dataItems, properties);

    if ( *file == HnSRTreeFile::null ) {
	delete file;
	return NULL;
    }

    fileSt = (HnSRTreeFileSt *)HnMalloc(sizeof(HnSRTreeFileSt));
    fileSt->opaque = file;

    return fileSt;
}

void
HnSRTreeFileSt_close(HnSRTreeFileSt *fileSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->close();
    delete file;

    HnFree(fileSt, sizeof(HnSRTreeFileSt));
}

void
HnSRTreeFileSt_store(HnSRTreeFileSt *fileSt,
		     const HnPointSt *pointSt, const HnDataItemSt *dataItemSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->store(pointStToPoint(pointSt), dataItemStToDataItem(dataItemSt));
}

void
HnSRTreeFileSt_remove(HnSRTreeFileSt *fileSt,
		      const HnPointSt *pointSt, const HnDataItemSt *dataItemSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->remove(pointStToPoint(pointSt), dataItemStToDataItem(dataItemSt));
}

void
HnSRTreeFileSt_getFirstInRect(HnSRTreeFileSt *fileSt,
			      const HnRectSt *queryRect,
			      HnPointSt **pointSt_return,
			      HnDataItemSt **dataItemSt_return)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;
    HnPoint point;
    HnDataItem dataItem;

    if ( queryRect == NULL ) {
	file->getFirst(&point, &dataItem);
    }
    else {
	file->getFirst(rectStToRect(queryRect), &point, &dataItem);
    }

    if ( point == HnPoint::null ) {
	*pointSt_return = NULL;
	*dataItemSt_return = NULL;
    }
    else {
	*pointSt_return = pointToPointSt(point);
	*dataItemSt_return = dataItemToDataItemSt(dataItem);
    }
}

void
HnSRTreeFileSt_getFirstInSphere(HnSRTreeFileSt *fileSt,
				const HnSphereSt *querySphere,
				HnPointSt **pointSt_return,
				HnDataItemSt **dataItemSt_return)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;
    HnPoint point;
    HnDataItem dataItem;

    if ( querySphere == NULL ) {
	file->getFirst(&point, &dataItem);
    }
    else {
	file->getFirst(sphereStToSphere(querySphere), &point, &dataItem);
    }

    if ( point == HnPoint::null ) {
	*pointSt_return = NULL;
	*dataItemSt_return = NULL;
    }
    else {
	*pointSt_return = pointToPointSt(point);
	*dataItemSt_return = dataItemToDataItemSt(dataItem);
    }
}

void
HnSRTreeFileSt_getNext(HnSRTreeFileSt *fileSt,
		       HnPointSt **pointSt_return,
		       HnDataItemSt **dataItemSt_return)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;
    HnPoint point;
    HnDataItem dataItem;

    file->getNext(&point, &dataItem);

    if ( point == HnPoint::null ) {
	*pointSt_return = NULL;
	*dataItemSt_return = NULL;
    }
    else {
	*pointSt_return = pointToPointSt(point);
	*dataItemSt_return = dataItemToDataItemSt(dataItem);
    }
}

void
HnSRTreeFileSt_getInRect(HnSRTreeFileSt *fileSt, const HnRectSt *queryRect,
			 HnPointVectorSt **pointsSt_return,
			 HnDataItemVectorSt **dataItemsSt_return)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;
    HnPointVector points;
    HnDataItemVector dataItems;

    file->getInRect(rectStToRect(queryRect), &points, &dataItems);

    *pointsSt_return = pointVectorToPointVectorSt(points);
    *dataItemsSt_return = dataItemVectorToDataItemVectorSt(dataItems);
}

void
HnSRTreeFileSt_getInSphere(HnSRTreeFileSt *fileSt,
			   const HnSphereSt *querySphere,
			   HnPointVectorSt **pointsSt_return,
			   HnDataItemVectorSt **dataItemsSt_return)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;
    HnPointVector points;
    HnDataItemVector dataItems;

    file->getInSphere(sphereStToSphere(querySphere), &points, &dataItems);

    *pointsSt_return = pointVectorToPointVectorSt(points);
    *dataItemsSt_return = dataItemVectorToDataItemVectorSt(dataItems);
}

void
HnSRTreeFileSt_getNeighbors(HnSRTreeFileSt *fileSt,
			    const HnPointSt *queryPointSt, int maxCount,
			    HnPointVectorSt **pointsSt_return,
			    HnDataItemVectorSt **dataItemsSt_return)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;
    HnPointVector points;
    HnDataItemVector dataItems;

    file->getNeighbors(pointStToPoint(queryPointSt), maxCount,
		       &points, &dataItems);

    *pointsSt_return = pointVectorToPointVectorSt(points);
    *dataItemsSt_return = dataItemVectorToDataItemVectorSt(dataItems);
}

void
HnSRTreeFileSt_getColoredNeighbors(HnSRTreeFileSt *fileSt,
				   const HnPointVectorSt *queryPointsSt,
				   int maxCount,
				   HnPointVectorSt **pointsSt_return,
				   HnDataItemVectorSt **dataItemsSt_return,
				   HnSRTreeCompColorsFunc *compColorsFunc)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;
    HnPointVector points;
    HnDataItemVector dataItems;

    file->getColoredNeighbors(pointVectorStToPointVector(queryPointsSt),
			      maxCount,
			      &points, &dataItems,
			      compColorsFunc);

    *pointsSt_return = pointVectorToPointVectorSt(points);
    *dataItemsSt_return = dataItemVectorToDataItemVectorSt(dataItems);
}

int
HnSRTreeFileSt_getDimension(HnSRTreeFileSt *fileSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    return file->getDimension();
}

int
HnSRTreeFileSt_getBlockSize(HnSRTreeFileSt *fileSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    return file->getBlockSize();
}

int
HnSRTreeFileSt_getDataItemSize(HnSRTreeFileSt *fileSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    return file->getDataItemSize();
}

int
HnSRTreeFileSt_getHeight(HnSRTreeFileSt *fileSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    return file->getHeight();
}

void
HnSRTreeFileSt_check(HnSRTreeFileSt *fileSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->check();
}

void
HnSRTreeFileSt_print(HnSRTreeFileSt *fileSt, HnBool verbose)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->print(verbose);
}

void
HnSRTreeFileSt_resetProfile(HnSRTreeFileSt *fileSt)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->resetProfile();
}

void
HnSRTreeFileSt_copyProfileInto(HnSRTreeFileSt *fileSt,
			       HnSRTreeProfileSt *profile)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->copyProfileInto(profile);
}

HnBool
HnSRTreeFileSt_dumpToFile(HnSRTreeFileSt *fileSt, const char *fileName)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    return file->dumpToFile(fileName);
}

HnBool
HnSRTreeFileSt_dumpToFileStream(HnSRTreeFileSt *fileSt, FILE *fp)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    return file->dumpToFileStream(fp);
}

void
HnSRTreeFileSt_setProperties(HnSRTreeFileSt *fileSt,
			     const HnPropertiesSt *properties)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    file->setProperties(propertiesStToProperties(properties));
}

void
HnSRTreeFileSt_getProperties(HnSRTreeFileSt *fileSt,
			     HnPropertiesSt *propertiesSt_return)
{
    HnSRTreeFile *file = (HnSRTreeFile *)fileSt->opaque;

    propertiesToPropertiesSt(file->getProperties(), propertiesSt_return);
}

void
HnSRTreeFileSt_setDebug(HnBool debug)
{
    HnSRTreeFile::setDebug(debug);
}

/*
 * Conversion functions
 */

static HnPointSt *
pointToPointSt(const HnPoint &point)
{
    int dimension = point.getDimension();
    HnPointSt *pointSt;

    pointSt = HnPointSt_allocate(dimension);
    memcpy(pointSt->coords, point.getCoords(), sizeof(double) * dimension);

    return pointSt;
}

static HnPoint
pointStToPoint(const HnPointSt *pointSt)
{
    HnPoint point;

    point = new_HnPoint(pointSt->dimension);
    memcpy(point.getCoords(), pointSt->coords,
	   sizeof(double) * pointSt->dimension);

    return point;
}

static HnPointVectorSt *
pointVectorToPointVectorSt(const HnPointVector &points)
{
    HnPointVectorSt *pointsSt;
    int i;

    pointsSt = HnPointVectorSt_allocate();

    for ( i=0; i<points.size(); i++ ) {
	HnPointVectorSt_addElement(pointsSt,
				   pointToPointSt(points.elementAt(i)));
    }

    return pointsSt;
}

static HnPointVector
pointVectorStToPointVector(const HnPointVectorSt *pointsSt)
{
    HnPointVector points;
    int i;

    points = new_HnPointVector();

    for ( i=0; i<pointsSt->size; i++ ) {
	points.addElement(pointStToPoint(pointsSt->elements[i]));
    }

    return points;
}

static HnDataItemSt *
dataItemToDataItemSt(const HnDataItem &dataItem)
{
    HnDataItemSt *dataItemSt;

    dataItemSt =
	HnDataItemSt_allocate(dataItem.toCharArray(), dataItem.length());

    return dataItemSt;
}

static HnDataItem
dataItemStToDataItem(const HnDataItemSt *dataItemSt)
{
    HnDataItem dataItem;

    dataItem = new_HnDataItem(dataItemSt->buffer, dataItemSt->length);
    return dataItem;
}

static HnDataItemVectorSt *
dataItemVectorToDataItemVectorSt(const HnDataItemVector &dataItems)
{
    HnDataItemVectorSt *dataItemsSt;
    int i;

    dataItemsSt = HnDataItemVectorSt_allocate();

    for ( i=0; i<dataItems.size(); i++ ) {
	HnDataItemVectorSt_addElement
	    (dataItemsSt, dataItemToDataItemSt(dataItems.elementAt(i)));
    }

    return dataItemsSt;
}

static HnDataItemVector
dataItemVectorStToDataItemVector(const HnDataItemVectorSt *dataItemsSt)
{
    HnDataItemVector dataItems;
    int i;

    dataItems = new_HnDataItemVector();

    for ( i=0; i<dataItemsSt->size; i++ ) {
	dataItems.addElement(dataItemStToDataItem(dataItemsSt->elements[i]));
    }

    return dataItems;
}


static HnSphere
sphereStToSphere(const HnSphereSt *sphereSt)
{
    HnPoint center;
    HnSphere sphere;
    int axis;

    center = new_HnPoint(sphereSt->center->dimension);

    for ( axis=0; axis<sphereSt->center->dimension; axis++ ) {
	center.setCoordAt(sphereSt->center->coords[axis], axis);
    }

    sphere = new_HnSphere(center, sphereSt->radius);

    return sphere;
}

static HnRect
rectStToRect(const HnRectSt *rectSt)
{
    HnRect rect;
    int axis;

    rect = new_HnRect(rectSt->dimension);

    for ( axis=0; axis<rectSt->dimension; axis++ ) {
	rect.setRangeAt(rectSt->ranges[axis].min,
			rectSt->ranges[axis].max, axis);
    }

    return rect;
}

static HnProperties
propertiesStToProperties(const HnPropertiesSt *propertiesSt)
{
    HnProperties properties;
    int i;

    properties = new_HnProperties();

    for ( i=0; i<propertiesSt->keys->size; i++ ) {
	properties.setProperty(propertiesSt->keys->elements[i]->buffer,
			       propertiesSt->values->elements[i]->buffer);
    }

    return properties;
}

static void
propertiesToPropertiesSt(const HnProperties &properties,
			 HnPropertiesSt *propertiesSt_return)
{
    int i;

    HnPropertiesSt_removeAllProperties(propertiesSt_return);

    for ( i=0; i<properties.size(); i++ ) {
	HnString key = properties.getKeyAt(i);
	HnString value = properties.getValueAt(i);

	HnPropertiesSt_setProperty(propertiesSt_return,
				   (char *)key, (char *)value);
    }
}
