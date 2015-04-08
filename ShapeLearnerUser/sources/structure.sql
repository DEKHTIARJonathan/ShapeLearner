/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "GraphClass" CASCADE;

CREATE TABLE "GraphClass" (
  "graphClassName" VARCHAR(255) NOT NULL PRIMARY KEY,
  "directGraph" BOOLEAN NOT NULL DEFAULT '0',
  "acyclicGraph" BOOLEAN NOT NULL DEFAULT '0');

/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "ObjectClass" CASCADE;

CREATE TABLE "ObjectClass" (
  "objectClassName" VARCHAR(255) NOT NULL PRIMARY KEY);

/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "Graph" CASCADE;

CREATE TABLE "Graph" (
  "idGraph" BIGSERIAL NOT NULL PRIMARY KEY,
  "objectName" VARCHAR(255) NOT NULL,
  "viewNumber" BIGINT NOT NULL DEFAULT '1',
  "refGraphClass" VARCHAR(255) NOT NULL,
  "refObjectClass" VARCHAR(255) NOT NULL);

CREATE INDEX "index_Graph_graphClass"
  ON "Graph" USING BTREE ("refGraphClass");

CREATE INDEX "index_Graph_objectClass"
  ON "Graph" USING BTREE ("refObjectClass");

CREATE UNIQUE INDEX "index_Graph_objectName"
  ON "Graph" USING BTREE ("objectName");

ALTER TABLE "Graph"
  ADD CONSTRAINT "refGraphClass_fk"
    FOREIGN KEY ("refGraphClass")
    REFERENCES "GraphClass" ("graphClassName")
    ON DELETE CASCADE
    INITIALLY DEFERRED,
  ADD CONSTRAINT "refObjectClass_fk"
    FOREIGN KEY ("refObjectClass")
    REFERENCES "ObjectClass" ("objectClassName")
    ON DELETE CASCADE
    INITIALLY DEFERRED;

/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "Node" CASCADE;

CREATE TABLE "Node" (
  "idNode" BIGSERIAL NOT NULL PRIMARY KEY,
  "index" BIGINT NOT NULL DEFAULT '1',
  "level" BIGINT NOT NULL DEFAULT '1',
  "mass" BIGINT NOT NULL DEFAULT '1',
  "type" BIGINT NOT NULL DEFAULT '1',
  "label" VARCHAR(255) NOT NULL DEFAULT '1',
  "refGraph" BIGINT NOT NULL);

CREATE INDEX "Node_RefGraph"
  ON "Node" USING BTREE ("refGraph");

ALTER TABLE "Node"
  ADD CONSTRAINT "refGraph_fk"
    FOREIGN KEY ("refGraph")
    REFERENCES "Graph" ("idGraph")
    ON DELETE CASCADE
    INITIALLY DEFERRED;

/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "Edge" CASCADE;

CREATE TABLE "Edge" (
  "idEdge" BIGSERIAL NOT NULL PRIMARY KEY,
  "weight" BIGINT NOT NULL DEFAULT '1',
  "source" BIGINT NOT NULL,
  "target" BIGINT NOT NULL,
  "refGraph" BIGINT NOT NULL);

CREATE INDEX "index_Edge_source"
  ON "Edge" USING BTREE ("source");

CREATE INDEX "index_Edge_target"
  ON "Edge" USING BTREE ("target");

CREATE UNIQUE INDEX "index_Edge_link"
  ON "Edge" USING BTREE (
    "source",
    "target");

CREATE INDEX "index_Edge_refGraph"
  ON "Edge" USING BTREE ("refGraph");

ALTER TABLE "Edge"
  ADD CONSTRAINT "source_fk"
    FOREIGN KEY ("source")
    REFERENCES "Node" ("idNode")
    ON DELETE CASCADE
    INITIALLY DEFERRED,
  ADD CONSTRAINT "target_fk"
    FOREIGN KEY ("target")
    REFERENCES "Node" ("idNode")
    ON DELETE CASCADE
    INITIALLY DEFERRED,
  ADD CONSTRAINT "refGraph_fk"
    FOREIGN KEY ("refGraph")
    REFERENCES "Graph" ("idGraph")
    ON DELETE CASCADE
    INITIALLY DEFERRED;

/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "Point" CASCADE;

CREATE TABLE "Point" (
  "idPoint" BIGSERIAL NOT NULL PRIMARY KEY,
  "xCoord" DOUBLE PRECISION NOT NULL DEFAULT '0',
  "yCoord" DOUBLE PRECISION NOT NULL DEFAULT '0',
  "radius" DOUBLE PRECISION NOT NULL DEFAULT '1',
  "refGraph" BIGINT NOT NULL,
  "refNode" BIGINT NOT NULL);

CREATE INDEX "index_Point_refNode"
  ON "Point" USING BTREE ("refNode");

CREATE INDEX "index_Point_refGraph"
  ON "Point" USING BTREE ("refGraph");

CREATE INDEX "index_Point_region2D"
  ON "Point" USING BTREE (
    "xCoord",
    "yCoord");

ALTER TABLE "Point"
  ADD CONSTRAINT "refGraph_fk"
    FOREIGN KEY ("refGraph")
    REFERENCES "Graph" ("idGraph")
    ON DELETE CASCADE
    INITIALLY DEFERRED,
  ADD CONSTRAINT "refNode_fk"
    FOREIGN KEY ("refNode")
    REFERENCES "Node" ("idNode")
    ON DELETE CASCADE
    INITIALLY DEFERRED;

-- GET TABLE STATE FOR ALL TABLES 

CREATE OR REPLACE FUNCTION public.count_rows(
    _schema text,
    _tablename text)
  RETURNS integer AS
$BODY$
DECLARE
rec integer;
BEGIN
EXECUTE 'SELECT count(*) FROM "' || $1 || '"."' || $2 || '"' INTO rec;
RETURN rec;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION public.count_rows(text, text)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION public.gettablestate()
  RETURNS TABLE(_schema text, _tname text, _count integer) AS
$BODY$
BEGIN

   RETURN QUERY
   
   SELECT table_schema::text, table_name::text, count_rows(table_schema, table_name)
FROM information_schema.tables 
WHERE table_schema not in ('pg_catalog', 'information_schema') and table_type='BASE TABLE'
order by 3 desc;

END
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;
ALTER FUNCTION public.gettablestate()
  OWNER TO postgres;
