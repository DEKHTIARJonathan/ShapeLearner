OBJS =  \
        ../HnObject/HnAbort.obj \
        ../HnObject/HnBinarySearch.obj \
        ../HnObject/HnBlockFile.obj \
        ../HnObject/HnBlockFileObj.obj \
        ../HnObject/HnBlockStream.obj \
        ../HnObject/HnBlockStreamObj.obj \
        ../HnObject/HnDataItem.obj \
        ../HnObject/HnDataItemObj.obj \
        ../HnObject/HnDataItemSt.obj \
        ../HnObject/HnFTcharArray.obj \
        ../HnObject/HnFTdoubleArray.obj \
        ../HnObject/HnFTfloatArray.obj \
        ../HnObject/HnFTintArray.obj \
        ../HnObject/HnFTlongArray.obj \
        ../HnObject/HnFTucharArray.obj \
        ../HnObject/HnFTuintArray.obj \
        ../HnObject/HnFTulongArray.obj \
        ../HnObject/HnGenericArray.obj \
        ../HnObject/HnGetOpt.obj \
        ../HnObject/HnMalloc.obj \
        ../HnObject/HnObject.obj \
        ../HnObject/HnPointer.obj \
        ../HnObject/HnProperties.obj \
        ../HnObject/HnPropertiesObj.obj \
        ../HnObject/HnPropertiesSt.obj \
        ../HnObject/HnQuickSelect.obj \
        ../HnObject/HnQuickSort.obj \
        ../HnObject/HnString.obj \
        ../HnObject/HnStatistics.obj \
        ../HnObject/HnStatisticsObj.obj \
        ../HnObject/HnStatisticsSt.obj \
        ../HnObject/HnStringBuffer.obj \
        ../HnObject/HnStringBufferObj.obj \
        ../HnObject/HnStringBufferSt.obj \
        ../HnObject/HnStringObj.obj \
        ../HnObject/HnStringSt.obj \
        ../HnObject/HnTimes.obj \
        ../HnObject/HnTimesObj.obj \
        ../HnObject/HnTimesSt.obj \
        ../HnObject/HnTimesStatsSt.obj \
        ../HnRect/HnPoint.obj \
        ../HnRect/HnPointObj.obj \
        ../HnRect/HnPointSt.obj \
        ../HnRect/HnRange.obj \
        ../HnRect/HnRangeSt.obj \
        ../HnRect/HnRect.obj \
        ../HnRect/HnRectObj.obj \
        ../HnRect/HnRectSt.obj \
        ../HnRect/HnSphere.obj \
        ../HnRect/HnSphereObj.obj \
        ../HnRect/HnSphereSt.obj \
        ../HnSRTree/HnSRTreeBlock.obj \
        ../HnSRTree/HnSRTreeBlockObj.obj \
        ../HnSRTree/HnSRTreeCluster.obj \
        ../HnSRTree/HnSRTreeClusterObj.obj \
        ../HnSRTree/HnSRTreeFile.obj \
        ../HnSRTree/HnSRTreeFileObj.obj \
        ../HnSRTree/HnSRTreeFileObj1.obj \
        ../HnSRTree/HnSRTreeFileObj2.obj \
        ../HnSRTree/HnSRTreeFileObj3.obj \
        ../HnSRTree/HnSRTreeFileSt.obj \
        ../HnSRTree/HnSRTreeInfo.obj \
        ../HnSRTree/HnSRTreeInfoObj.obj \
        ../HnSRTree/HnSRTreeLeaf.obj \
        ../HnSRTree/HnSRTreeLeafObj.obj \
        ../HnSRTree/HnSRTreeNeighbor.obj \
        ../HnSRTree/HnSRTreeNeighborObj.obj \
        ../HnSRTree/HnSRTreeNode.obj \
        ../HnSRTree/HnSRTreeNodeObj.obj \
        ../HnSRTree/HnSRTreeProfileSt.obj \
        ../HnSRTree/HnSRTreeReinsert.obj \
        ../HnSRTree/HnSRTreeReinsertObj.obj \
        ../HnSRTree/HnSRTreeStack.obj \
        ../HnSRTree/HnSRTreeStackObj.obj \
        ../HnSRTree/HnSRTreeStatsSt.obj

HnSRTree.lib: $(OBJS)
	lib /nologo /LTCG /out:HnSRTree.lib @libobjs.txt

HnSRTree.dll: $(OBJS)
	link /nologo /dll /out:HnSRTree.dll @libobjs.txt
	mt -manifest HnSRTree.dll.manifest -outputresource:libHnSRTree.dll;2

clean:
	$(RM) *.lib
