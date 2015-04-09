/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "relation_employee_projects" CASCADE;

DROP TABLE IF EXISTS "relation_employee" CASCADE;

DROP TABLE IF EXISTS "relation_project" CASCADE;

DROP TABLE IF EXISTS "relation_employer" CASCADE;

CREATE TABLE "relation_employer" (
  "name" TEXT NOT NULL PRIMARY KEY);

CREATE TABLE "relation_project" (
  "name" TEXT NOT NULL PRIMARY KEY);

CREATE TABLE "relation_employee" (
  "id" BIGSERIAL NOT NULL PRIMARY KEY,
  "first" TEXT NOT NULL,
  "last" TEXT NOT NULL,
  "employer" TEXT NOT NULL,
  CONSTRAINT "employer_fk"
    FOREIGN KEY ("employer")
    REFERENCES "relation_employer" ("name")
    INITIALLY DEFERRED);

CREATE TABLE "relation_employee_projects" (
  "object_id" BIGINT NOT NULL,
  "value" TEXT NOT NULL,
  CONSTRAINT "object_id_fk"
    FOREIGN KEY ("object_id")
    REFERENCES "relation_employee" ("id")
    ON DELETE CASCADE,
  CONSTRAINT "value_fk"
    FOREIGN KEY ("value")
    REFERENCES "relation_project" ("name")
    INITIALLY DEFERRED);

CREATE INDEX "relation_employee_projects_object_id_i"
  ON "relation_employee_projects" ("object_id");
