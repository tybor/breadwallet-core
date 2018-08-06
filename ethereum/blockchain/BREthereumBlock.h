//
//  BBREthereumBlock.h
//  breadwallet-core Ethereum
//
//  Created by Ed Gamble on 3/23/2018.
//  Copyright (c) 2018 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BR_Ethereum_Block_H
#define BR_Ethereum_Block_H

#include <limits.h>
#include "../base/BREthereumBase.h"
#include "BREthereumTransaction.h"
#include "BREthereumLog.h"
#include "BREthereumAccountState.h"
#include "BREthereumBloomFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BREthereumBlockHeaderRecord *BREthereumBlockHeader;
typedef struct BREthereumBlockRecord *BREthereumBlock;

//
// Block Header
//
extern void
blockHeaderRelease (BREthereumBlockHeader header);


/**
 * Check if the block header is internally consistent
 *
 * @param header
 * @return ETHEREUM_BOOLEAN_TRUE if valid
 */
extern BREthereumBoolean
blockHeaderIsValid (BREthereumBlockHeader header);


/**
 * Check if the block header is consistent with the parent.  If `parent` is NULL, then
 * `header` is consisder consistent
 *
 * @param header
 * @param parent
 * @param parentOmmersCount
 * @parem genesis
 * @return ETHEREUM_BOOLEAN_TRUE if consistent
 */
extern BREthereumBoolean
blockHeaderIsConsistent (BREthereumBlockHeader header,
                         BREthereumBlockHeader parent,
                         size_t parentOmmersCount,
                         BREthereumBlockHeader genesis);
extern BREthereumBlockHeader
blockHeaderRlpDecode (BRRlpItem item,
                      BREthereumRlpType type,
                      BRRlpCoder coder);

extern BRRlpItem
blockHeaderRlpEncode (BREthereumBlockHeader header,
                      BREthereumBoolean withNonce,
                      BREthereumRlpType type,
                      BRRlpCoder coder);

extern BREthereumHash
blockHeaderGetHash (BREthereumBlockHeader header);

extern uint64_t
blockHeaderGetNumber (BREthereumBlockHeader header);

extern UInt256
blockHeaderGetDifficulty (BREthereumBlockHeader header);

extern uint64_t
blockHeaderGetGasUsed (BREthereumBlockHeader header);

extern uint64_t
blockHeaderGetTimestamp (BREthereumBlockHeader header);

extern BREthereumHash
blockHeaderGetParentHash (BREthereumBlockHeader header);

// ...

extern uint64_t
blockHeaderGetNonce (BREthereumBlockHeader header);

extern BREthereumBoolean
blockHeaderMatch (BREthereumBlockHeader header,
                  BREthereumBloomFilter filter);

extern BREthereumBoolean
blockHeaderMatchAddress (BREthereumBlockHeader header,
                         BREthereumAddress address);

// Support BRSet
extern size_t
blockHeaderHashValue (const void *h);

// Support BRSet
extern int
blockHeaderHashEqual (const void *h1, const void *h2);    

extern void
blockHeaderReleaseForSet (void *ignore, void *item);

// Support sorting
extern BREthereumComparison
blockHeaderCompare (BREthereumBlockHeader h1,
                    BREthereumBlockHeader h2);
    
//
// Block
//
extern BREthereumBlock
blockCreateMinimal(BREthereumHash hash,
                   uint64_t number,
                   uint64_t timestamp);

extern BREthereumBlock
blockCreateFull (BREthereumBlockHeader header,
                 BREthereumBlockHeader ommers[], size_t ommersCount,
                 BREthereumTransaction transactions[], size_t transactionCount);

extern BREthereumBlock
blockCreate (BREthereumBlockHeader header);

extern void
blockUpdateBody (BREthereumBlock block,
                 BREthereumBlockHeader *ommers,
                 BREthereumTransaction *transactions);

extern BREthereumBlockHeader
blockHeaderCopy (BREthereumBlockHeader source);

extern void
blockRelease (BREthereumBlock block);

extern BREthereumBoolean
blockIsValid (BREthereumBlock block,
              BREthereumBoolean skipHeaderValidation);

extern BREthereumBlockHeader
blockGetHeader (BREthereumBlock block);

extern unsigned long
blockGetTransactionsCount (BREthereumBlock block);

extern BREthereumTransaction
blockGetTransaction (BREthereumBlock block, size_t index);

extern BREthereumBoolean
blockTransactionsAreValid (BREthereumBlock block);

extern unsigned long
blockGetOmmersCount (BREthereumBlock block);

extern BREthereumBlockHeader
blockGetOmmer (BREthereumBlock block, unsigned int index);

extern BREthereumHash
blockGetHash (BREthereumBlock block);

extern uint64_t
blockGetNumber (BREthereumBlock block);

extern uint64_t
blockGetConfirmations (BREthereumBlock block);

extern uint64_t
blockGetTimestamp (BREthereumBlock block);

extern void
blockLinkLogsWithTransactions (BREthereumBlock block);
    
extern BRRlpItem
blockRlpEncode (BREthereumBlock block,
                BREthereumNetwork network,
                BREthereumRlpType type,
                BRRlpCoder coder);
    
extern BREthereumBlock
blockRlpDecode (BRRlpItem item,
                BREthereumNetwork network,
                BREthereumRlpType type,
                BRRlpCoder coder);
    
// Support BRSet
extern size_t
blockHashValue (const void *h);

// Support BRSet
extern int
blockHashEqual (const void *h1, const void *h2);

extern void
blockReleaseForSet (void *ignore, void *item);


//
// MARK: - Block Next (Chaining)
//
    
#define BLOCK_NEXT_NONE   ((BREthereumBlock) 0)

extern BREthereumBlock
blockGetNext (BREthereumBlock block);

extern BREthereumBlock // old 'next'
blockSetNext (BREthereumBlock block,
              BREthereumBlock next);

extern BREthereumBoolean
blockHasNext (BREthereumBlock block);

static inline BREthereumBlock // old 'next'
blockClrNext (BREthereumBlock block) {
    return blockSetNext(block, BLOCK_NEXT_NONE);
}

//
// MARK: - Block Status
//

typedef enum {
    BLOCK_REQUEST_NOT_NEEDED,
    BLOCK_REQEUST_PENDING,
    BLOCK_REQUEST_COMPLETE,
    BLOCK_REQUEST_ERROR
} BREthereumBlockRequestState;

typedef struct {
    BREthereumHash hash;

    BREthereumBlockRequestState transactionRequest;
    BREthereumTransaction *transactions;

    BREthereumBlockRequestState logRequest;
    BREthereumLog *logs;

    BREthereumBlockRequestState accountStateRequest;
    BREthereumAccountState accountState;

    BREthereumBoolean error;
} BREthereumBlockStatus;

/**
 * Get the block's status
 */
extern BREthereumBlockStatus
blockGetStatus (BREthereumBlock block);

extern BREthereumBoolean
blockHasStatusComplete (BREthereumBlock block);

extern BREthereumBoolean
blockHasStatusError (BREthereumBlock block);

extern void
blockReportStatusError (BREthereumBlock block,
                        BREthereumBoolean error);

//
// Transaction Request
//
extern BREthereumBoolean
blockHasStatusTransactionsRequest (BREthereumBlock block,
                                   BREthereumBlockRequestState request);

extern void
blockReportStatusTransactionsRequest (BREthereumBlock block,
                                      BREthereumBlockRequestState request);

/**
 * Set the transactions in the block's status.  The transactions are
 * BRArrayOf(BREthereumTransaction) and the transactions are stolen (the array is now owned
 * by `block`; the transactions are owned by another).
 */
extern void
blockReportStatusTransactions (BREthereumBlock block,
                               BREthereumTransaction *transactions);

//
// Log Request
//
extern BREthereumBoolean
blockHasStatusLogsRequest (BREthereumBlock block,
                           BREthereumBlockRequestState request);

extern void
blockReportStatusLogsRequest (BREthereumBlock block,
                              BREthereumBlockRequestState request);

/**
 * Set the logs in the block's status.  Handling of `logs` is identical to handling of
 * `transactions` - see above
 */
extern void
blockReportStatusLogs (BREthereumBlock block,
                       BREthereumLog *log);


//
// Account State Reqeust
//
extern BREthereumBoolean
blockHasStatusAccountStateRequest (BREthereumBlock block,
                                   BREthereumBlockRequestState request);

extern void
blockReportStatusAccountStateRequest (BREthereumBlock block,
                                      BREthereumBlockRequestState request);

/**
 * Set the account state in the block's status.
 */
extern void
blockReportStatusAccountState (BREthereumBlock block,
                               BREthereumAccountState accountState);

    //
    //
    //
extern BREthereumBoolean
blockHasStatusTransaction (BREthereumBlock block,
                           BREthereumTransaction transaction);

extern BREthereumBoolean
blockHasStatusLog (BREthereumBlock block,
                   BREthereumLog log);

//
// MARK: - Block Decoding for LES
//

/**
 * Return BRArrayOf(BREthereumBlockHeader) w/ array owned by caller.
 */
extern BREthereumBlockHeader *
blockOmmersRlpDecode (BRRlpItem item,
                      BREthereumNetwork network,
                      BREthereumRlpType type,
                      BRRlpCoder coder);

/**
 * Return BRArrayOf(BREthereumTransaction) w/ array owned by caller
 */
extern BREthereumTransaction *
blockTransactionsRlpDecode (BRRlpItem item,
                            BREthereumNetwork network,
                            BREthereumRlpType type,
                            BRRlpCoder coder);
//
// MARK: Genesis Blocks
//

/**
 * Return a newly-allocaed block header duplicating the genesis block header for `network`.
 */
extern BREthereumBlockHeader
networkGetGenesisBlockHeader (BREthereumNetwork network);

/**
 * Returh a newly-allocated block duplicating the generic block's header for `network`.
 */
extern BREthereumBlock
networkGetGenesisBlock (BREthereumNetwork network);

//
// MARK: Block Checkpoint
//

/**
 * A Block Checkpoint ...
 */
typedef struct {
    uint64_t number;
    BREthereumHash hash;
    uint64_t timestamp;
} BREthereumBlockCheckpoint;

#define BLOCK_CHECKPOINT_LAST_NUMBER      (UINT64_MAX)
#define BLOCK_CHECKPOINT_LAST_TIMESTAMP   (UINT64_MAX)

extern const BREthereumBlockCheckpoint *
blockCheckpointLookupLatest (BREthereumNetwork network);

extern const BREthereumBlockCheckpoint *
blockCheckpointLookupByNumber (BREthereumNetwork network,
                               uint64_t number);

extern const BREthereumBlockCheckpoint *
blockCheckpointLookupByTimestamp (BREthereumNetwork network,
                                  uint64_t timestamp);

extern BREthereumBlockHeader
blockCheckpointCreatePartialBlockHeader (const BREthereumBlockCheckpoint *checkpoint);

#ifdef __cplusplus
}
#endif

#endif /* BR_Ethereum_Block_H */