--select * from "ObjectClass";
--select * from "GraphClass";
--select * from "Graph" order by "idGraph";
/*
select 
	"idGraph", "objectName", "viewNumber","nodeCount","edgeCount","cumulativeMass","DAGCost",
	"fileOffset","MaxTSVDimension","totalTSVSum","shape_xMax","shape_xMin","shape_yMax",
	"shape_yMin","shape_Height","shape_Width","refGraphClass","refObjectClass" 
from "Graph" 
order by "idGraph";
*/
--select * from "Node" order by "refGraph", "idNode";
--select * from "Edge" order by "refGraph", "idEdge";
--select * from "Point" order by "refGraph", "refNode", "idPoint";