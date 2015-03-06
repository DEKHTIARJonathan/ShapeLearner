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
  "graphClass" VARCHAR(255) NOT NULL,
  "objectClass" VARCHAR(255) NOT NULL,
  "objectName" VARCHAR(255) NOT NULL,
  "viewNumber" INTEGER NOT NULL DEFAULT '1');

CREATE INDEX "index_Graph_graphClass"
  ON "Graph" USING BTREE ("graphClass");

CREATE INDEX "index_Graph_objectClass"
  ON "Graph" USING BTREE ("objectClass");

CREATE UNIQUE INDEX "index_Graph_objectName"
  ON "Graph" USING BTREE ("objectName");

ALTER TABLE "Graph"
  ADD CONSTRAINT "graphClass_fk"
    FOREIGN KEY ("graphClass")
    REFERENCES "GraphClass" ("graphClassName")
    ON DELETE CASCADE
    INITIALLY DEFERRED,
  ADD CONSTRAINT "objectClass_fk"
    FOREIGN KEY ("objectClass")
    REFERENCES "ObjectClass" ("objectClassName")
    ON DELETE CASCADE
    INITIALLY DEFERRED;

/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "Node" CASCADE;

CREATE TABLE "Node" (
  "idNode" BIGSERIAL NOT NULL PRIMARY KEY,
  "index" INTEGER NOT NULL DEFAULT '1',
  "level" INTEGER NOT NULL DEFAULT '1',
  "mass" INTEGER NOT NULL DEFAULT '1',
  "type" INTEGER NOT NULL DEFAULT '1',
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
  "source" BIGINT NOT NULL,
  "target" BIGINT NOT NULL,
  "refGraph" BIGINT NOT NULL,
  "weight" INTEGER NOT NULL DEFAULT '1');

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

