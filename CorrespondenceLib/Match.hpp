#ifndef MATCH_HPP
#define MATCH_HPP

#include "CorrespondenceDefs.hpp"

void matchDense(const correspondence::Image& censusIm1, const correspondence::Image& censusIm2,
                const correspondence::eMatchMode, const correspondence::eSamplingPattern, 
                std::vector<correspondence::Match>& rMatches);

void matchSparse(const correspondence::Image& censusIm1, const correspondence::Image& censusIm2,
                 const correspondence::CensusCfg& cfg, const correspondence::FeatureList& kps1, 
                 correspondence::FeatureList& kps2, std::vector<correspondence::Match>& rMatches);

void getPotentialMatches(const correspondence::Feature& kp1, correspondence::FeatureList& kps2, 
                         const correspondence::CensusCfg& cfg, std::vector<correspondence::KpRow>& rPotMatches);


/** Use this if !SSSE3 and !SSE4
  *
  */
uint32_t calcHammingDist(const uint16_t _1, const uint16_t _2);

/** Use this if SSSE3, if !SSSE3, but SSE4, use _mm_popcount_u32, instead
  *
  */
int calcHammingDistSSE(__m128i* _1, __m128i* _2, __m128i* _mask_lo, __m128i* _mask_popcnt);

uint32_t calcSHD(const correspondence::Image& im1, const correspondence::Image& im2,
                 const correspondence::Feature& kp1, const correspondence::Feature& kp2);

correspondence::Match matchFeature(const correspondence::Image& im1, const correspondence::Feature& kp1, 
                                   const std::vector<correspondence::KpRow>& kps2);

#endif
