# contrib/pg_uuid_next/Makefile

MODULE_big	= pg_uuid_next
OBJS = \
	$(WIN32RES) \
	pg_uuid_next.o

EXTENSION = pg_uuid_next
DATA = pg_uuid_next--1.0.sql
PGFILEDESC = "pg_uuid_next - functions to generate UUID v7 and v8"

REGRESS = check

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pg_uuid_next
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
