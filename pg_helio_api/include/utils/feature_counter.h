/*-------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation.  All rights reserved.
 *
 * include/utils/feature_counter.h
 *
 * Utilities for mongo feature usage tracking.
 *
 *-------------------------------------------------------------------------
 */

#ifndef FEATURE_COUNTER_H
#define FEATURE_COUNTER_H

#include <storage/backendid.h>
#include <port/atomics.h>

#define MAX_FEATURE_NAME_LENGTH 255
#define MAX_FEATURE_COUNT 235

/* Internal features that are not exposed */
#define INTERNAL_FEATURE_TYPE MAX_FEATURE_COUNT

/*
 * IMP: Keep this alphabetically sorted while adding new feature types. Sorting is done for better reability.
 * #CodeSync: Keep this in sync with FeatureMapping array in feature_counter.c
 *            For each FeatureType enum a FeatureMapping entry should exist.
 *
 * Make sure that we don't exceed MAX_FEATURE_COUNT
 *
 * Do not use MAX_FEATURE_INDEX and ensure it is the last entry.
 */
typedef enum
{
	/* Feature counter region - Aggregation operators */
	FEATURE_AGG_OPERATOR_ABS,
	FEATURE_AGG_OPERATOR_ACCUMULATOR,
	FEATURE_AGG_OPERATOR_ACOS,
	FEATURE_AGG_OPERATOR_ACOSH,
	FEATURE_AGG_OPERATOR_ADD,
	FEATURE_AGG_OPERATOR_ADDTOSET,
	FEATURE_AGG_OPERATOR_ALLELEMENTSTRUE,
	FEATURE_AGG_OPERATOR_AND,
	FEATURE_AGG_OPERATOR_ANYELEMENTTRUE,
	FEATURE_AGG_OPERATOR_ARRAYELEMAT,
	FEATURE_AGG_OPERATOR_ARRAYTOOBJECT,
	FEATURE_AGG_OPERATOR_ASIN,
	FEATURE_AGG_OPERATOR_ASINH,
	FEATURE_AGG_OPERATOR_ATAN,
	FEATURE_AGG_OPERATOR_ATAN2,
	FEATURE_AGG_OPERATOR_ATANH,
	FEATURE_AGG_OPERATOR_AVG,
	FEATURE_AGG_OPERATOR_BINARYSIZE,
	FEATURE_AGG_OPERATOR_BITAND,
	FEATURE_AGG_OPERATOR_BITNOT,
	FEATURE_AGG_OPERATOR_BITOR,
	FEATURE_AGG_OPERATOR_BITXOR,
	FEATURE_AGG_OPERATOR_BSONSIZE,
	FEATURE_AGG_OPERATOR_CEIL,
	FEATURE_AGG_OPERATOR_CMP,
	FEATURE_AGG_OPERATOR_CONCAT,
	FEATURE_AGG_OPERATOR_CONCATARRAYS,
	FEATURE_AGG_OPERATOR_COND,
	FEATURE_AGG_OPERATOR_CONST,
	FEATURE_AGG_OPERATOR_CONVERT,
	FEATURE_AGG_OPERATOR_COS,
	FEATURE_AGG_OPERATOR_COSH,
	FEATURE_AGG_OPERATOR_DATEADD,
	FEATURE_AGG_OPERATOR_DATEDIFF,
	FEATURE_AGG_OPERATOR_DATESUBTRACT,
	FEATURE_AGG_OPERATOR_DATEFROMPARTS,
	FEATURE_AGG_OPERATOR_DATEFROMSTRING,
	FEATURE_AGG_OPERATOR_DATETOPARTS,
	FEATURE_AGG_OPERATOR_DATETOSTRING,
	FEATURE_AGG_OPERATOR_DATETRUNC,
	FEATURE_AGG_OPERATOR_DAYOFMONTH,
	FEATURE_AGG_OPERATOR_DAYOFWEEK,
	FEATURE_AGG_OPERATOR_DAYOFYEAR,
	FEATURE_AGG_OPERATOR_DEGREESTORADIANS,
	FEATURE_AGG_OPERATOR_DIVIDE,
	FEATURE_AGG_OPERATOR_EQ,
	FEATURE_AGG_OPERATOR_EXP,
	FEATURE_AGG_OPERATOR_FILTER,
	FEATURE_AGG_OPERATOR_FIRST,
	FEATURE_AGG_OPERATOR_FIRSTN,
	FEATURE_AGG_OPERATOR_FLOOR,
	FEATURE_AGG_OPERATOR_FUNCTION,
	FEATURE_AGG_OPERATOR_GETFIELD,
	FEATURE_AGG_OPERATOR_GT,
	FEATURE_AGG_OPERATOR_GTE,
	FEATURE_AGG_OPERATOR_HOUR,
	FEATURE_AGG_OPERATOR_IFNULL,
	FEATURE_AGG_OPERATOR_IN,
	FEATURE_AGG_OPERATOR_INDEXOFARRAY,
	FEATURE_AGG_OPERATOR_INDEXOFBYTES,
	FEATURE_AGG_OPERATOR_INDEXOFCP,
	FEATURE_AGG_OPERATOR_ISARRAY,
	FEATURE_AGG_OPERATOR_ISNUMBER,
	FEATURE_AGG_OPERATOR_ISODAYOFWEEK,
	FEATURE_AGG_OPERATOR_ISOWEEK,
	FEATURE_AGG_OPERATOR_ISOWEEKYEAR,
	FEATURE_AGG_OPERATOR_LAST,
	FEATURE_AGG_OPERATOR_LASTN,
	FEATURE_AGG_OPERATOR_LET,
	FEATURE_AGG_OPERATOR_LITERAL,
	FEATURE_AGG_OPERATOR_LN,
	FEATURE_AGG_OPERATOR_LOG,
	FEATURE_AGG_OPERATOR_LOG10,
	FEATURE_AGG_OPERATOR_LT,
	FEATURE_AGG_OPERATOR_LTE,
	FEATURE_AGG_OPERATOR_LTRIM,
	FEATURE_AGG_OPERATOR_MAKE_ARRAY,
	FEATURE_AGG_OPERATOR_MAP,
	FEATURE_AGG_OPERATOR_MAX,
	FEATURE_AGG_OPERATOR_MAXN,
	FEATURE_AGG_OPERATOR_MERGEOBJECTS,
	FEATURE_AGG_OPERATOR_META,
	FEATURE_AGG_OPERATOR_MILLISECOND,
	FEATURE_AGG_OPERATOR_MIN,
	FEATURE_AGG_OPERATOR_MINN,
	FEATURE_AGG_OPERATOR_MINUTE,
	FEATURE_AGG_OPERATOR_MOD,
	FEATURE_AGG_OPERATOR_MONTH,
	FEATURE_AGG_OPERATOR_MULTIPLY,
	FEATURE_AGG_OPERATOR_NE,
	FEATURE_AGG_OPERATOR_NOT,
	FEATURE_AGG_OPERATOR_OBJECTTOARRAY,
	FEATURE_AGG_OPERATOR_OR,
	FEATURE_AGG_OPERATOR_POW,
	FEATURE_AGG_OPERATOR_PUSH,
	FEATURE_AGG_OPERATOR_RADIANSTODEGREES,
	FEATURE_AGG_OPERATOR_RAND,
	FEATURE_AGG_OPERATOR_RANGE,
	FEATURE_AGG_OPERATOR_REDUCE,
	FEATURE_AGG_OPERATOR_REGEXFIND,
	FEATURE_AGG_OPERATOR_REGEXFINDALL,
	FEATURE_AGG_OPERATOR_REGEXMATCH,
	FEATURE_AGG_OPERATOR_REPLACEONE,
	FEATURE_AGG_OPERATOR_REPLACEALL,
	FEATURE_AGG_OPERATOR_REVERSEARRAY,
	FEATURE_AGG_OPERATOR_ROUND,
	FEATURE_AGG_OPERATOR_RTRIM,
	FEATURE_AGG_OPERATOR_SECOND,
	FEATURE_AGG_OPERATOR_SETDIFFERENCE,
	FEATURE_AGG_OPERATOR_SETEQUALS,
	FEATURE_AGG_OPERATOR_SETFIELD,
	FEATURE_AGG_OPERATOR_SETINTERSECTION,
	FEATURE_AGG_OPERATOR_SETISSUBSET,
	FEATURE_AGG_OPERATOR_SETUNION,
	FEATURE_AGG_OPERATOR_SIN,
	FEATURE_AGG_OPERATOR_SINH,
	FEATURE_AGG_OPERATOR_SIZE,
	FEATURE_AGG_OPERATOR_SLICE,
	FEATURE_AGG_OPERATOR_SORTARRAY,
	FEATURE_AGG_OPERATOR_SPLIT,
	FEATURE_AGG_OPERATOR_SQRT,
	FEATURE_AGG_OPERATOR_STDDEVPOP,
	FEATURE_AGG_OPERATOR_STDDEVSAMP,
	FEATURE_AGG_OPERATOR_STRLENBYTES,
	FEATURE_AGG_OPERATOR_STRLENCP,
	FEATURE_AGG_OPERATOR_STRCASECMP,
	FEATURE_AGG_OPERATOR_SUBSTR,
	FEATURE_AGG_OPERATOR_SUBSTRBYTES,
	FEATURE_AGG_OPERATOR_SUBSTRCP,
	FEATURE_AGG_OPERATOR_SUBTRACT,
	FEATURE_AGG_OPERATOR_SUM,
	FEATURE_AGG_OPERATOR_SWITCH,
	FEATURE_AGG_OPERATOR_TAN,
	FEATURE_AGG_OPERATOR_TANH,
	FEATURE_AGG_OPERATOR_TOBOOL,
	FEATURE_AGG_OPERATOR_TODATE,
	FEATURE_AGG_OPERATOR_TODECIMAL,
	FEATURE_AGG_OPERATOR_TODOUBLE,
	FEATURE_AGG_OPERATOR_TOINT,
	FEATURE_AGG_OPERATOR_TOLONG,
	FEATURE_AGG_OPERATOR_TOLOWER,
	FEATURE_AGG_OPERATOR_TOOBJECTID,
	FEATURE_AGG_OPERATOR_TOSTRING,
	FEATURE_AGG_OPERATOR_TOUPPER,
	FEATURE_AGG_OPERATOR_TRIM,
	FEATURE_AGG_OPERATOR_TRUNC,
	FEATURE_AGG_OPERATOR_TSINCREMENT,
	FEATURE_AGG_OPERATOR_TSSECOND,
	FEATURE_AGG_OPERATOR_TYPE,
	FEATURE_AGG_OPERATOR_UNSETFIELD,
	FEATURE_AGG_OPERATOR_WEEK,
	FEATURE_AGG_OPERATOR_YEAR,
	FEATURE_AGG_OPERATOR_ZIP,

	FEATURE_COLLATION,

	/* Feature counter region - Commands */
	FEATURE_COMMAND_COLLMOD,
	FEATURE_COMMAND_COLLSTATS,
	FEATURE_COMMAND_CREATE_COLLECTION,
	FEATURE_COMMAND_CREATE_VALIDATION,
	FEATURE_COMMAND_CREATE_VIEW,
	FEATURE_COMMAND_CURRENTOP,
	FEATURE_COMMAND_DBSTATS,
	FEATURE_COMMAND_DELETE,
	FEATURE_COMMAND_FINDANDMODIFY,
	FEATURE_COMMAND_INSERT,
	FEATURE_COMMAND_UPDATE,
	FEATURE_COMMAND_VALIDATE_REPAIR,

	/* Feature collMod subfeatures */
	FEATURE_COMMAND_COLLMOD_VIEW,
	FEATURE_COMMAND_COLLMOD_COLOCATION,
	FEATURE_COMMAND_COLLMOD_VALIDATION,

	/* Feature counter region - Create index types */
	FEATURE_CREATE_INDEX_2D,
	FEATURE_CREATE_INDEX_2DSPHERE,
	FEATURE_CREATE_INDEX_FTS,
	FEATURE_CREATE_INDEX_TEXT,
	FEATURE_CREATE_INDEX_TTL,
	FEATURE_CREATE_INDEX_UNIQUE,
	FEATURE_CREATE_INDEX_VECTOR,
	FEATURE_CREATE_INDEX_VECTOR_COS,
	FEATURE_CREATE_INDEX_VECTOR_IP,
	FEATURE_CREATE_INDEX_VECTOR_L2,
	FEATURE_CREATE_INDEX_VECTOR_TYPE_IVFFLAT,
	FEATURE_CREATE_INDEX_VECTOR_TYPE_HNSW,

	/* Feature counter region - Query Operators */
	FEATURE_QUERY_OPERATOR_GEOINTERSECTS,
	FEATURE_QUERY_OPERATOR_GEONEAR,
	FEATURE_QUERY_OPERATOR_GEOWITHIN,
	FEATURE_QUERY_OPERATOR_NEAR,
	FEATURE_QUERY_OPERATOR_NEARSPHERE,
	FEATURE_QUERY_OPERATOR_SAMPLERATE,
	FEATURE_QUERY_OPERATOR_TEXT,

	/* Feature counter region - Aggregation stages */
	FEATURE_STAGE_ADD_FIELDS,
	FEATURE_STAGE_CHANGE_STREAM,
	FEATURE_STAGE_BUCKET,
	FEATURE_STAGE_COLLSTATS,
	FEATURE_STAGE_COUNT,
	FEATURE_STAGE_CURRENTOP,
	FEATURE_STAGE_DOCUMENTS,
	FEATURE_STAGE_FACET,
	FEATURE_STAGE_GEONEAR,
	FEATURE_STAGE_GRAPH_LOOKUP,
	FEATURE_STAGE_GROUP,
	FEATURE_STAGE_GROUP_ACC_FIRSTN,
	FEATURE_STAGE_GROUP_ACC_FIRSTN_GT10,
	FEATURE_STAGE_GROUP_ACC_LASTN,
	FEATURE_STAGE_GROUP_ACC_LASTN_GT10,
	FEATURE_STAGE_INDEXSTATS,
	FEATURE_STAGE_INVERSEMATCH,
	FEATURE_STAGE_LIMIT,
	FEATURE_STAGE_LOOKUP,
	FEATURE_STAGE_MATCH,
	FEATURE_STAGE_MERGE,
	FEATURE_STAGE_PROJECT,
	FEATURE_STAGE_PROJECT_FIND,
	FEATURE_STAGE_REPLACE_ROOT,
	FEATURE_STAGE_REPLACE_WITH,
	FEATURE_STAGE_SAMPLE,
	FEATURE_STAGE_SEARCH,
	FEATURE_STAGE_SEARCH_VECTOR,
	FEATURE_STAGE_SEARCH_VECTOR_IVFFLAT,
	FEATURE_STAGE_SEARCH_VECTOR_HNSW,
	FEATURE_STAGE_SEARCH_VECTOR_PRE_FILTER,
	FEATURE_STAGE_SET,
	FEATURE_STAGE_SETWINDOWFIELDS,
	FEATURE_STAGE_SKIP,
	FEATURE_STAGE_SORT,
	FEATURE_STAGE_SORT_BY_COUNT,
	FEATURE_STAGE_UNIONWITH,
	FEATURE_STAGE_UNSET,
	FEATURE_STAGE_UNWIND,
	FEATURE_STAGE_VECTOR_SEARCH_KNN,
	FEATURE_STAGE_VECTOR_SEARCH_MONGO,

	/* Feature usage stats */
	FEATURE_TTL_PURGER_CALLS,

	/* Feature mapping region - User CRUD*/
	FEATURE_USER_CREATE,
	FEATURE_USER_DROP,
	FEATURE_USER_GET,
	FEATURE_USER_UPDATE,

	/* This must be the last value in the FeatureType. */
	MAX_FEATURE_INDEX
} FeatureType;

typedef int FeatureCounter[MAX_FEATURE_COUNT];

extern void SharedFeatureCounterShmemInit(void);
extern const char * GetFeatureCountersAsString(void);
extern void ResetFeatureCounters(void);
extern FeatureCounter *FeatureCounterBackendArray;

/*
 *  Given a feature id this method increments the feature usage count for the
 *  feature for the current backend process.
 *
 *  Postgres guarantees that MyBackendId is always between 1 and MaxBackend.
 */
static inline void
ReportFeatureUsage(int featureId)
{
	pg_write_barrier();
	FeatureCounterBackendArray[MyBackendId - 1][featureId]++;
}


#endif /* FEATURE_COUNTER_H */
