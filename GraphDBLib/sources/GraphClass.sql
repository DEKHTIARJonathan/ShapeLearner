/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "GraphClass" CASCADE;

CREATE TABLE "GraphClass" (
  "graphClassName" VARCHAR(255) NOT NULL PRIMARY KEY,
  "directGraph" BOOLEAN NOT NULL DEFAULT '0',
  "acyclicGraph" BOOLEAN NOT NULL DEFAULT '0');
