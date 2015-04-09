/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "container_person_age_weight" CASCADE;

DROP TABLE IF EXISTS "container_person_emails" CASCADE;

DROP TABLE IF EXISTS "container_person_nicknames" CASCADE;

DROP TABLE IF EXISTS "container_person" CASCADE;

CREATE TABLE "container_person" (
  "id" BIGSERIAL NOT NULL PRIMARY KEY,
  "first" TEXT NOT NULL,
  "last" TEXT NOT NULL);

CREATE TABLE "container_person_nicknames" (
  "object_id" BIGINT NOT NULL,
  "index" BIGINT NOT NULL,
  "value" TEXT NOT NULL,
  CONSTRAINT "object_id_fk"
    FOREIGN KEY ("object_id")
    REFERENCES "container_person" ("id")
    ON DELETE CASCADE);

CREATE INDEX "container_person_nicknames_object_id_i"
  ON "container_person_nicknames" ("object_id");

CREATE INDEX "container_person_nicknames_index_i"
  ON "container_person_nicknames" ("index");

CREATE TABLE "container_person_emails" (
  "object_id" BIGINT NOT NULL,
  "value" TEXT NOT NULL,
  CONSTRAINT "object_id_fk"
    FOREIGN KEY ("object_id")
    REFERENCES "container_person" ("id")
    ON DELETE CASCADE);

CREATE INDEX "container_person_emails_object_id_i"
  ON "container_person_emails" ("object_id");

CREATE TABLE "container_person_age_weight" (
  "object_id" BIGINT NOT NULL,
  "key" SMALLINT NOT NULL,
  "value" REAL NOT NULL,
  CONSTRAINT "object_id_fk"
    FOREIGN KEY ("object_id")
    REFERENCES "container_person" ("id")
    ON DELETE CASCADE);

CREATE INDEX "container_person_age_weight_object_id_i"
  ON "container_person_age_weight" ("object_id");
