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
