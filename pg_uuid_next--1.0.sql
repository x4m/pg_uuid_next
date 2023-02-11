/* contrib/pg_tm_aux/pg_uuid_next--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_uuid_next" to load this file. \quit

CREATE FUNCTION gen_uuid_v7()
RETURNS uuid
AS 'MODULE_PATHNAME', 'gen_uuid_v7'
LANGUAGE C VOLATILE;

CREATE FUNCTION gen_uuid_v8()
RETURNS uuid
AS 'MODULE_PATHNAME', 'gen_uuid_v8'
LANGUAGE C VOLATILE;
