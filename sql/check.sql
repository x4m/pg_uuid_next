CREATE EXTENSION pg_uuid_next;


CREATE TABLE guid1
(
	guid_field UUID,
	text_field TEXT DEFAULT(now())
);

-- generation test for v7
TRUNCATE guid1;
INSERT INTO guid1 (guid_field) VALUES (gen_uuid_v7());
INSERT INTO guid1 (guid_field) VALUES (gen_uuid_v7());
SELECT count(DISTINCT guid_field) FROM guid1;

-- generation test for v8
TRUNCATE guid1;
INSERT INTO guid1 (guid_field) VALUES (gen_uuid_v8());
INSERT INTO guid1 (guid_field) VALUES (gen_uuid_v8());
SELECT count(DISTINCT guid_field) FROM guid1;
