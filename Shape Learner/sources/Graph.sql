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

CREATE INDEX "graph_index"
  ON "Graph" USING BTREE ("graphClass");

CREATE INDEX "object_index"
  ON "Graph" USING BTREE ("objectClass");

CREATE UNIQUE INDEX "name_index"
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

