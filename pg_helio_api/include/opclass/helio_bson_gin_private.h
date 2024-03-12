/*-------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation.  All rights reserved.
 *
 * include/opclass/helio_bson_gin_private.h
 *
 * Private declarations of the bson gin methods shared across files
 * that implement the GIN index extensibility
 *
 *-------------------------------------------------------------------------
 */

#ifndef HELIO_BSON_GIN_PRIVATE_H
#define HELIO_BSON_GIN_PRIVATE_H

#include "opclass/helio_gin_common.h"
#include "planner/mongo_query_operator.h"
#include "operators/bson_expr_eval.h"
#include "opclass/helio_gin_index_term.h"
#include "opclass/helio_gin_index_mgmt.h"

/*
 * Arguments to Extract query used by mongo operators wrapped in a struct.
 */
typedef struct BsonExtractQueryArgs
{
	pgbson *query;

	int32 *nentries;

	bool **partialmatch;

	Pointer **extra_data;

	bytea *options;

	IndexTermCreateMetadata termMetadata;
} BsonExtractQueryArgs;

/*
 * Holds state pertinent to index term evaluation for
 * Expression based $elemMatch.
 */
typedef struct BsonElemMatchIndexExprState
{
	/* The query expression to be evaluated */
	ExprEvalState *expression;

	/* Whether or not the query expression is empty */
	bool isEmptyExpression;
} BsonElemMatchIndexExprState;


Datum * GinBsonExtractQueryCore(BsonIndexStrategy strategy,
								BsonExtractQueryArgs *args);
int32_t GinBsonComparePartialCore(BsonIndexStrategy strategy, BsonIndexTerm *queryValue,
								  BsonIndexTerm *compareValue, Pointer extraData);
bool GinBsonConsistentCore(BsonIndexStrategy strategy, bool *check,
						   Pointer *extra_data, int32_t numKeys, bool *recheck,
						   Datum *queryKeys, bytea *options);
int32_t GinBsonComparePartialElemMatchExpression(BsonIndexTerm *queryValue,
												 BsonIndexTerm *compareValue,
												 BsonElemMatchIndexExprState *exprState);

/* $elemMatch methods */

Datum * GinBsonExtractQueryElemMatch(BsonExtractQueryArgs *args);
int32_t GinBsonComparePartialElemMatch(BsonIndexTerm *queryValue,
									   BsonIndexTerm *compareValue,
									   Pointer extraData);
bool GinBsonElemMatchConsistent(bool *checkArray, Pointer *extraData, int32_t numKeys);

/* Shared with exclusion ops */

/*
 * Defines the behavior of index term traversal for a given path when walking a document
 */
typedef enum IndexTraverseOption
{
	/* The path is invalid and no terms should be generated in that tree. */
	IndexTraverse_Invalid,

	/* The path may have valid descendants that could generate terms on the index */
	/* do not consider the path, but recurse down nested objects and arrays for terms */
	IndexTraverse_Recurse,

	/* the path is a match and should be added to the index. */
	IndexTraverse_Match,
} IndexTraverseOption;


/* Index path options */
IndexTraverseOption GetSinglePathIndexTraverseOption(void *contextOptions,
													 const char *currentPath,
													 uint32_t currentPathLength,
													 bson_type_t bsonType);
IndexTraverseOption GetWildcardProjectionPathIndexTraverseOption(void *contextOptions,
																 const char *currentPath,
																 uint32_t
																 currentPathLength,
																 bson_type_t
																 bsonType);
IndexTraverseOption GetSinglePathIndexTraverseOptionCore(const char *indexPath,
														 uint32_t indexPathLength,
														 const char *currentPath,
														 uint32_t currentPathLength,
														 bool isWildcard);

/*
 * Defines a function that provides instruction on how to handle a path 'currentPath' given an index option
 * specification via contextOptions.
 */
typedef IndexTraverseOption (*GetIndexTraverseOptionFunc)(void *contextOptions, const
														  char *currentPath, uint32_t
														  currentPathLength,
														  bson_type_t valueType);

/*
 * Context object used to keep track of state while generating terms for the index.
 */
typedef struct
{
	/* After computation, this holds the total number of terms that should be generated by this document and index spec. */
	int32_t totalTermCount;

	/* holds the actual index term datums. */
	struct
	{
		/* The entry array of index terms.*/
		Datum *entries;

		/* The size in Datum entries available in entries. */
		int32_t entryCapacity;
	} terms;


	/* the current position into the entries that should be written into. */
	int32_t index;

	/* Any index configuration options used to determine whether to generate terms or not */
	void *options;

	/* A function that provides instruction on whether a path should be indexed or not. */
	GetIndexTraverseOptionFunc traverseOptionsFunc;

	/* Whether or not to generate the not found term for a path */
	bool generateNotFoundTerm;

	/* metadata including truncation limit for index terms. */
	IndexTermCreateMetadata termMetadata;

	/* Whether a root truncation term has already been created for this document */
	bool hasTruncatedTerms;

	/*
	 * Whether or not the path has array ancestors in the pre paths:
	 * for a path a.b.c
	 * if a, or b are arrays then this returns true.
	 * For wildcard indexes, returns true if any path had an array ancestor
	 */
	bool hasArrayAncestors;
} GenerateTermsContext;


/* Exports for the core index processing layer. */
void GenerateSinglePathTermsCore(pgbson *bson, GenerateTermsContext *context,
								 BsonGinSinglePathOptions *singlePathOptions);

void GenerateWildcardPathTermsCore(pgbson *bson, GenerateTermsContext *context,
								   BsonGinWildcardProjectionPathOptions *wildcardOptions);

void GenerateTerms(pgbson *bson, GenerateTermsContext *context, bool addRootTerm);

Datum *GinBsonExtractQueryUniqueIndexTerms(PG_FUNCTION_ARGS);
Datum *GinBsonExtractQueryOrderBy(PG_FUNCTION_ARGS);
int32_t GinBsonComparePartialOrderBy(BsonIndexTerm *queryValue,
									 BsonIndexTerm *compareValue);

IndexTermCreateMetadata GetIndexTermMetadata(void *indexOptions);

#endif
