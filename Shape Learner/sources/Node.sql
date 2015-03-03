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
  "pointCount" INTEGER NOT NULL DEFAULT '1',
  "label" INTEGER NOT NULL DEFAULT '1',
  "refGraph" BIGINT NOT NULL);

CREATE INDEX "Node_RefGraph"
  ON "Node" USING BTREE ("refGraph");

ALTER TABLE "Node"
  ADD CONSTRAINT "refGraph_fk"
    FOREIGN KEY ("refGraph")
    REFERENCES "Graph" ("idGraph")
    ON DELETE CASCADE
    INITIALLY DEFERRED;
