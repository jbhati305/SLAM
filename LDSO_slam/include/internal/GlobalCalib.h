#pragma once
#ifndef LDSO_SLAM_GLOBALCALIB_H
#define LDSO_SLAM_GLOBALCALIB_H


#include "NumTypes.h"
#include "Settings.h"

// ldso::internal namespace

namespace ldso 
{
    namespace internal {
        //Calibration
        // Images->width, Images->height, K->intrinsic matrix

        extern int wG[PYR_LEVELS], hG[PYR_LEVELS];
        extern float fxG[PYR_LEVELS], fyG[PYR_LEVELS], 
                    cxG[PYR_LEVELS], cyG[PYR_LEVELS];

        
        extern float fxiG[PYR_LEVELS], fyiG[PYR_LEVELS], 
                    cxiG[PYR_LEVELS], cyiG[PYR_LEVELS];

        extern Eigen::Matrix3f KG[PYR_LEVELS], KiG[PYR_LEVELS];

        extern float wM3G; // w-3
        extern float hM3G; // h-3

        // set each level's calibration
        void setGlobalCalib(int w, int h, const Eigen::Matrix3f& K);
    }
}


#endif  // LDSO_SLAM_GLOBALCALIB_H