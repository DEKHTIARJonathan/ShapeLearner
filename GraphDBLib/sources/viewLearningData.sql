/* This view is used to perform the Machine Learning approach.
 * Created by Jonathan DEKHTIAR
 */

CREATE VIEW "learning_data" AS
  SELECT
	g."idGraph",
	g."refObjectClass", 
	g."cumulativeMass", 
	g."DAGCost",
	g."MaxTSVDimension", 
	g."totalTSVSum", 
	g."shape_xMax", 
	g."shape_xMin", 
	g."shape_yMax", 
	g."shape_yMin", 
	g."shape_Height", 
	g."shape_Width",
	n."nodeCount",
	e."edgeCount",
	p."pointCount",
	n."avg_pointCount" as "avg_pointPerNode", 
	n."avg_level", 
	n."avg_mass", 
	n."avg_type" as "avg_NodeType", 
	n."avg_role",
	n."avg_subtreeCost", 
	n."avg_tsvNorm",
	p."avg_xCoord", 
	p."avg_yCoord", 
	p."avg_radius", 
	p."avg_speed", 
	p."avg_dr_ds", 
	p."avg_dr", 
	p."avg_direction"
FROM 
	"Graph" as g
LEFT JOIN
	(SELECT 
		count(*) as "nodeCount" , 
		avg("level") as "avg_level", 
		avg("mass") as "avg_mass", 
		avg("type") as "avg_type", 
		avg("role") as "avg_role", 
		avg("pointCount") as "avg_pointCount", 
		avg("subtreeCost") as "avg_subtreeCost", 
		avg("tsvNorm") as "avg_tsvNorm",
		"refGraph"
	FROM "Node"
	GROUP BY "refGraph") as n
ON g."idGraph" = n."refGraph"

LEFT JOIN
	(SELECT 
		count(*) as "edgeCount",
		"refGraph"
	FROM "Edge" 
	GROUP BY  "refGraph") as e
ON g."idGraph" = e."refGraph"

LEFT JOIN
	(SELECT 
		count(*) as "pointCount",
		avg("xCoord") as "avg_xCoord", 
		avg("yCoord") as "avg_yCoord", 
		avg("radius") as "avg_radius", 
		avg("speed") as "avg_speed", 
		avg("dr_ds") as "avg_dr_ds", 
		avg("dr")as "avg_dr", 
		avg("direction") as "avg_direction", 
		"refGraph" 
	FROM "Point" 
	GROUP BY "refGraph") as p
ON g."idGraph" = p."refGraph"

ORDER BY "idGraph";