/*-------------------------------------------------------------------------
 *
 * pg_uuid_next.c
 *		UUIDs for better data locality
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <sys/time.h>

#include "common/hashfn.h"
#include "port/pg_bswap.h"
#include "miscadmin.h"
#include "utils/builtins.h"
#include "utils/uuid.h"

PG_MODULE_MAGIC;


PG_FUNCTION_INFO_V1(gen_uuid_v7);

Datum
gen_uuid_v7(PG_FUNCTION_ARGS)
{
	pg_uuid_t  *uuid = palloc(UUID_LEN);
	struct timeval tp;
	uint64_t tms;

	gettimeofday(&tp, NULL);

	tms = ((uint64_t)tp.tv_sec) * 1000;
	tms += ((uint64_t)tp.tv_usec) / 1000;

	tms = pg_hton64(tms<<16);

	/* Fill in time part */
	memcpy(&uuid->data[0], &tms, 6);

	/* fill everything after the timestamp with random bytes */
	if (!pg_strong_random(&uuid->data[6], UUID_LEN - 6))
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("could not generate random values")));

	/*
	 * Set magic numbers for a "version 7" (pseudorandom) UUID, see
	 * https://datatracker.ietf.org/doc/html/draft-ietf-uuidrev-rfc4122bis#name-creating-a-uuidv8-value
	 */
	/* set version field, top four bits are 0, 1, 1, 1 */
	uuid->data[6] = (uuid->data[6] & 0x0f) | 0x70;
	/* set variant field, top two bits are 1, 0 */
	uuid->data[8] = (uuid->data[8] & 0x3f) | 0x80;

	PG_RETURN_UUID_P(uuid);
}

PG_FUNCTION_INFO_V1(gen_uuid_v8);

static uint8_t sequence;

Datum
gen_uuid_v8(PG_FUNCTION_ARGS)
{
	pg_uuid_t  *uuid = palloc(UUID_LEN);
	struct timeval tp;
	uint32_t t;
	uint16_t ut;

	gettimeofday(&tp, NULL);
	t = tp.tv_sec - 1577836800;
	t = pg_hton32(t);
	memcpy(&uuid->data[0], &t, 4);

	/* 16 bit subsecond fraction (~15 microsecond resolution) */
	ut = ((uint64_t)tp.tv_usec << 16) / 1000000;
	memcpy(&uuid->data[4], &ut, 2);

	/* fill everything after the timestamp with random bytes */
	if (!pg_strong_random(&uuid->data[6], UUID_LEN - 6))
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("could not generate random values")));

	/*
	 * Set magic numbers for a "version 8" UID, see
	 * https://datatracker.ietf.org/doc/html/draft-ietf-uuidrev-rfc4122bis#name-creating-a-uuidv8-value
	 */
	uuid->data[6] = (uuid->data[6] & 0x0f) | 0x70;
	uuid->data[8] = (uuid->data[8] & 0x3f) | 0x80;

	uuid->data[14] = MyProcPid;
	uuid->data[15] = sequence++;

	PG_RETURN_UUID_P(uuid);
}
