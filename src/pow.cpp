// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "auxpow.h"
#include "arith_uint256.h"
#include "chain.h"
#include "dogecoin.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

// Determine if the for the given block, a min difficulty setting applies
bool AllowMinDifficultyForBlock(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // check if the chain allows minimum difficulty blocks
    if (!params.fPowAllowMinDifficultyBlocks)
        return false;

    // Dogecoin: Magic number at which reset protocol switches
    // check if we allow minimum difficulty at this block-height
    if (pindexLast->nHeight < 157500)
        return false;

    // Allow for a minimum block time if the elapsed time > 2*nTargetSpacing
    return (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2);
}
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
	//unsigned int nProofOfWorkLimit = bnProofOfWorkLimit.GetCompact();
	unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Luckycoin difficulty adjustment protocol switch
    //bool fNewDifficultyProtocol = (pindexLast->nHeight+1 >= 69360 || fTestNet);
    bool fNewDifficultyProtocol = (pindexLast->nHeight+1 >= 69360 || false);

    //int64 nTargetTimespanCurrent = fNewDifficultyProtocol ? nTargetTimespan : (nTargetTimespan*12);
    //int64 nInterval = nTargetTimespanCurrent / nTargetSpacing;
    //int64_t nTargetTimespanCurrent = fNewDifficultyProtocol ? nTargetTimespan : (nTargetTimespan*12);
    int64_t nTargetTimespanCurrent = fNewDifficultyProtocol ? params.nPowTargetTimespan : (params.nPowTargetTimespan*12);
	
    //int64_t nInterval = nTargetTimespanCurrent / nTargetSpacing;
    int64_t nInterval = nTargetTimespanCurrent / params.nPowTargetSpacing;
	

    // Only change once per interval
    if ((pindexLast->nHeight+1) % nInterval != 0)
    {

        return pindexLast->nBits;
    }

    // Luckycoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = nInterval-1;
    if ((pindexLast->nHeight+1) != nInterval)
        blockstogoback = nInterval;

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    //int64 nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    //printf(" nActualTimespan = %"PRI64d" before bounds\n", nActualTimespan);

    //int64 nActualTimespanMin = fNewDifficultyProtocol ? (nTargetTimespanCurrent - nTargetTimespanCurrent/4) : (nTargetTimespanCurrent/4);
    //int64 nActualTimespanMax = fNewDifficultyProtocol ? (nTargetTimespanCurrent + nTargetTimespanCurrent/4) : (nTargetTimespanCurrent*4);
    int64_t nActualTimespanMin = fNewDifficultyProtocol ? (nTargetTimespanCurrent - nTargetTimespanCurrent/4) : (nTargetTimespanCurrent/4);
    int64_t nActualTimespanMax = fNewDifficultyProtocol ? (nTargetTimespanCurrent + nTargetTimespanCurrent/4) : (nTargetTimespanCurrent*4);
	
	if(pindexLast->nHeight+1 > 10000)
	{
        if (nActualTimespan < nActualTimespanMin)
            nActualTimespan = nActualTimespanMin;
        if (nActualTimespan > nActualTimespanMax)
            nActualTimespan = nActualTimespanMax;
	}
	else if(pindexLast->nHeight+1 > 5000)
	{
        if (nActualTimespan < nActualTimespanMin/2)
            nActualTimespan = nActualTimespanMin/2;
        if (nActualTimespan > nActualTimespanMax)
            nActualTimespan = nActualTimespanMax;
	}
	else
	{
        if (nActualTimespan < nActualTimespanMin/4)
            nActualTimespan = nActualTimespanMin/4;
        if (nActualTimespan > nActualTimespanMax)
            nActualTimespan = nActualTimespanMax;
	}

    // Retarget
    //CBigNum bnNew;
	arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespanCurrent;

    //if (bnNew > bnProofOfWorkLimit) bnNew = bnProofOfWorkLimit;
    if (bnNew > UintToArith256(params.powLimit)) bnNew = UintToArith256(params.powLimit);

    /// debug print
    //printf("GetNextWorkRequired RETARGET\n");
    //printf("nTargetTimespan = %"PRI64d" nActualTimespan = %"PRI64d"\n", nTargetTimespanCurrent, nActualTimespan);
    //printf("Before: %08x %s\n", pindexLast->nBits, CBigNum().SetCompact(pindexLast->nBits).getuint256().ToString().c_str());
    //printf("After: %08x %s\n", bnNew.GetCompact(), bnNew.getuint256().ToString().c_str());

    return bnNew.GetCompact();
}
/*
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Dogecoin: Special rules for minimum difficulty blocks with Digishield
    if (AllowDigishieldMinDifficultyForBlock(pindexLast, pblock, params))
    {
        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2* nTargetSpacing minutes
        // then allow mining of a min-difficulty block.
        return nProofOfWorkLimit;
    }

    // Only change once per difficulty adjustment interval
    bool fNewDifficultyProtocol = (pindexLast->nHeight >= 145000);
    const int64_t difficultyAdjustmentInterval = fNewDifficultyProtocol
                                                 ? 1
                                                 : params.DifficultyAdjustmentInterval();
    if ((pindexLast->nHeight+1) % difficultyAdjustmentInterval != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Litecoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = difficultyAdjustmentInterval-1;
    if ((pindexLast->nHeight+1) != difficultyAdjustmentInterval)
        blockstogoback = difficultyAdjustmentInterval;

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - blockstogoback;
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateDogecoinNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}
*/
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    //if (UintToArith256(hash) > bnTarget)
    //    return false;

    return true;
}
