#pragma once
#ifndef LDSO_SLAM_SETTINGS_H
#define LDSO_SLAM_SETTINGS_H

/// a lot of parameters can be set in LDSO
namespace ldso
{
const int PYR_LEVELS = 6;   // number of pyramid levels, NOTE not all are used during tracking
const int NUM_THREADS = 6;  // number of threads for tracking

// THE config bits in solver
const int SOLVER_SVD = 1;                      // use SVD 0000 0001
const int SOLVER_ORTHOGONALIZE_SYSTEM = 2;     //  0000 0010
const int SOLVER_ORTHOGONALIZE_POINTMARG = 4;  //  0000 0100
const int SOLVER_ORTHOGONALIZE_FULL = 8;       //  0000 1000

const int SOLVER_SVD_CUT7 = 16;
const int SOLVER_REMOVE_POSEPRIOR = 32;
const int SOLVER_USE_GN = 64;  // GAUSS-NEWTON
const int SOLVER_FIX_LAMBDA = 128;
const int SOLVER_ORTHOGONALIZE_X = 256;
const int SOLVER_MOMENTUM = 512;
const int SOLVER_STEPMOMENTUM = 1024;
const int SOLVER_ORTHOGONALIZE_X_LATER = 2048;

// constants to scale the parameters in optimization
const float SCALE_IDEPTH = 1.0f;    // scale internal value to idepth
const float SCALE_XI_ROT = 1.0f;    // Rotation
const float SCALE_XI_TRANS = 0.5f;  // Translation
const float SCALE_F = 50.0f;        // focal length
const float SCALE_C = 50.0f;        // camera center
const float SCALE_W = 1.0f;         // no idea what this is??
const float SCALE_A = 10.0f;        // Photometric Parameter
const float SCALE_B = 1000.0f;      // Photometric Parameter

// detail setting variables
extern int pyrLevelUsed;
extern float setting_keyframesPerSecond;
extern bool setting_realTimeMaxKF;
extern float setting_maxShiftWeightT;
extern float setting_maxShiftWeightR;
extern float setting_maxShiftWeightRT;

extern float setting_maxAffineWeight;
extern float setting_kfGlobalWeight;
extern float setting_idepthFixPrior;
extern float setting_idepthFixPriorMargFac;
extern float setting_initialRotPrior;
extern float setting_initialTransPrior;
extern float setting_initialAffAPrior;
extern float setting_initialAffBPrior;
extern float setting_initialCalibHessian;

extern int setting_solverMode;
extern double setting_solverModeDelta;
extern float setting_minIdepthH_act;
extern float setting_minIdepthH_marg;
extern int setting_margPointVisWindow;

extern float setting_maxIdepth;
extern float setting_maxPixSearch;
extern float setting_desiredImmatureDensity;  // done
extern float setting_desiredPointDensity;     // done
extern float setting_minPointsRemaining;
extern float setting_maxLogAffFactInWindow;

extern int setting_minFrames;
extern int setting_maxFrames;
extern int setting_minFrameAge;

extern int setting_minOptIterations;
extern int setting_maxOptIterations;

extern float setting_thOptIterations;
extern float setting_outlierTH;
extern float setting_outlierTHSumComponent;
extern float setting_outlierSmoothnessTH;  // higher -> more strict

extern int setting_killOverexposed;
extern int setting_killOverexposedMode;
extern int setting_pattern;  // this is one of 10 implemented residual patterns see below

extern float setting_marWeightFac;
extern int setting_discreteSearchItOnPointActivation;  // change on some point
extern int setting_GNItsOnPointActivation;
extern float setting_SmoothnessErrorPixelTH;
extern float setting_SmoothnessEMinInlierPercentage;
extern float setting_SmoothnessEGoodInlierPercentage;

extern float setting_minTraceQuality;
extern int setting_minTraceTestRadius;

extern float setting_reTrackThreshold;
extern int setting_minGoodActiveResForMarg;
extern int setting_minGoodResForMarg;
extern int setting_minInlierVotesForMarg;
extern float setting_minRelBSForMarg;
extern int setting_photometricCalibration;

extern bool setting_useExposure;
extern float setting_affineOptModeA;
extern float setting_affineOptModeB;
extern float setting_affineOptModeA_huberTH;
extern float setting_affineOptModeB_huberTH;

extern int setting_gammaWeightPixelSelect;
extern bool setting_relinAlways;
extern bool setting_fixCalib;
extern bool setting_activateAllOnMarg;
extern bool setting_forceAceptStep;  // may be it should be called AcceptStep??

extern float setting_useDepthWeightsCoarse;
extern bool setting_dilateDoubleCoarse;
extern float setting_huberTH;
extern bool setting_logStuff;  // whether to log stuff

extern float benchmarkSetting_fxfyfac;
extern int benchmarkSetting_width;
extern int benchmarkSetting_height;
extern float benchmark_varNoise;
extern float benchmark_varBlurNoise;
extern int benchmark_noiseGridsize;
extern float benchmark_initializerSlackFactor;

extern float setting_frameEnergyTHConstWeight;
extern float setting_frameEnergyTHN;
extern float setting_frameEnergyTHFacMean;
extern float setting_frameEnergyTHFacMedian;
extern float setting_overallEnergyTHWeight;
extern float setting_coarseCuttoffTH;
extern float setting_minGradHistCut;
extern float setting_minGradHistAdd;
extern float setting_fixGradTH;

extern float setting_gradDownweightPerLevel;
extern bool setting_selectDirectionDistribution;
extern int setting_pixelSelectionUseFast;
extern float setting_trace_stepsize;
extern int setting_trace_GNIterations;

extern float setting_trace_GNThreshold;
extern float setting_trace_extraSlackOnTH;
extern float setting_trace_slackInterval;
extern float setting_trace_minImprovementFactor;

// here are some settings for the 3D viewer
extern bool setting_render_displaycoarseTrackingFull;
extern bool setting_render_renderWindowFrames;
extern bool setting_render_plotTrackingFull;
extern bool setting_render_display3D;
extern bool setting_render_displayResidual;
extern bool setting_render_displayVideo;
extern bool setting_render_displayDepth;

extern bool setting_render_fullResetRequested;
extern bool setting_debugout_runquiet;
extern bool disableAllDisplay;
extern bool disableReconfigure;
extern bool setting_onlyLogKFPoses;
extern bool debugSaveImages;
extern int sparsityFactor;
extern bool goStepByStep;
extern bool plotStereoImages;
extern bool multiThreading;

extern float freeDebugParam1;
extern float freeDebugParam2;
extern float freeDebugParam3;
extern float freeDebugParam4;
extern float freeDebugParam5;

extern int benchmarkSpecialOption;
extern bool setting_pause;
extern int setting_pointSelection;  // 0-DSO's selection. 1-LDSO's selection 2-Random selection

const int patternNum = 8;
const int patternPadding = 2;

// patterns, see Settings.cc
extern int staticPattern[10][40][2];
extern int staticPatternNum[10];
extern int staticPatternPadding[10];

// loop closing setting
// whether to enable loop closing, if set to false, the program will run as original DSO
// enabled by default
extern bool setting_enableLoopClosing;

// if fast loop closing is enabled, not all the keyframes will be checked for loop closing
// and we will also have lesss constraints in the optimization
// NOTE: if you don't enable fast loopclosing, you would problably wait for a long time for the optimization to finish
// loops will not be immedialtely closed after detection!!
extern bool setting_fastLoopClosing;

// whether to show the loop closing results, if true will show the loop keyframes in a OpenCV window
// this is only for visualization purposes
extern bool setting_showLoopClosing;

// use the ninth pattern (described in DSO paper) for residual calculation
#define patternP staticPattern[8]

}  // namespace ldso

#endif  // LDSO_SLAM_SETTINGS_H