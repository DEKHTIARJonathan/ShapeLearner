DROP TABLE IF EXISTS "jobStatus" CASCADE;

CREATE TABLE "jobStatus" (
  "name" VARCHAR(255) NOT NULL PRIMARY KEY
);

INSERT INTO "jobStatus" (name) VALUES 
('Not Started'),
('Waiting'),
('Ongoing'),
('Error'),
('Finished');

DROP TABLE IF EXISTS "jobs" CASCADE;

CREATE TABLE "jobs" (
  "idJob" BIGSERIAL NOT NULL PRIMARY KEY,
  "jobStatus" VARCHAR(255) NOT NULL,
  "partID" INTEGER NOT NULL,
  "partName" VARCHAR(255) NOT NULL,
  "serverIP" VARCHAR(255) NOT NULL,
  "serverPort" VARCHAR(255) NOT NULL,
  "message" TEXT NOT NULL,
  "updateDate" TIMESTAMP NOT NULL
);

CREATE INDEX "index_jobStatus"
  ON "jobs" USING BTREE ("jobStatus");

CREATE INDEX "index_partID"
  ON "jobs" USING BTREE ("partID");

ALTER TABLE "jobs"
  ADD CONSTRAINT "jobStatus_fk"
    FOREIGN KEY ("jobStatus")
    REFERENCES "jobStatus" ("name")
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
  RETURNS TABLE(_schema text, _tname text, _count integer,_ttype text) AS
$BODY$
BEGIN

   RETURN QUERY
   
   SELECT table_schema::text, table_name::text, count_rows(table_schema, table_name), table_type::text
FROM information_schema.tables 
WHERE table_schema not in ('pg_catalog', 'information_schema')
order by 4 , 2;

END
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;
ALTER FUNCTION public.gettablestate()
  OWNER TO postgres;