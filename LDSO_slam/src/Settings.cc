#include "Settings.h"
// #include <cstdio>  /// TODO: this can be removed later
#include <iostream>

using namespace std;

namespace ldso
{

// detail setting variables
int pyrLevelUsed = PYR_LEVELS;

float setting_keyframesPerSecond = 0;
bool setting_realTimeMaxKF = false;

float setting_maxShiftWeightT = 0.04f * (640 + 480);
float setting_maxShiftWeightR = 0.0f * (640 + 480);
float setting_maxShiftWeightRT = 0.02f * (640 + 480);

float setting_maxAffineWeight = 2.0;
float setting_kfGlobalWeight = 1.0;
float setting_idepthFixPrior = 50 * 50;

float setting_idepthFixPriorMargFac = 600 * 600;  // 30000*30000;
float setting_initialRotPrior = 1e11;
float setting_initialTransPrior = 1e10;
float setting_initialAffAPrior = 1e14;
float setting_initialAffBPrior = 1e14;
float setting_initialCalibHessian = 5e9;

int setting_solverMode = SOLVER_FIX_LAMBDA | SOLVER_ORTHOGONALIZE_X_LATER;
double setting_solverModeDelta = 0.00001;
float setting_minIdepthH_act = 100;
float setting_minIdepthH_marg = 50;
int setting_margPointVisWindow = 0;

// extern float setting_maxIdepth; // THIS IS MISSING in original codebase
float setting_maxPixSearch = 0.027;
float setting_desiredImmatureDensity = 1500;  // done
float setting_desiredPointDensity = 2000;     // done
float setting_minPointsRemaining = 0.05;
float setting_maxLogAffFactInWindow = 0.7;

int setting_minFrames = 5;
int setting_maxFrames = 7;
int setting_minFrameAge = 1;

int setting_minOptIterations = 1;
int setting_maxOptIterations = 6;

float setting_thOptIterations = 1.2;
float setting_outlierTH = 12 * 12;              // higher -> more strict
float setting_outlierTHSumComponent = 50 * 50;  // higher -> less strong gradient-based reweighting
float setting_outlierSmoothnessTH = 0;          // higher -> more strict

int setting_killOverexposed = 1000;
int setting_killOverexposedMode = 0;
int setting_pattern = 8;  // this is one of 10 implemented residual patterns
                          // see below

float setting_marWeightFac = 0.5 * 0.5;
int setting_discreteSearchItOnPointActivation = 0;  // change on some point
int setting_GNItsOnPointActivation = 3;
float setting_SmoothnessErrorPixelTH = 1000;
float setting_SmoothnessEMinInlierPercentage = 0.25;
float setting_SmoothnessEGoodInlierPercentage = 0.66;

float setting_minTraceQuality = 3;
int setting_minTraceTestRadius = 2;

float setting_reTrackThreshold = 1.5;
int setting_minGoodActiveResForMarg = 3;  // irrelevant
int setting_minGoodResForMarg = 4;        // irrelevant
int setting_minInlierVotesForMarg = 0;    // irrelevant
float setting_minRelBSForMarg = 0.00;     // irrelevant

// 0 = nothing
// 1 = apply inv. response
// 2 = apply inv. response and remove Vignette
int setting_photometricCalibration = 2;

bool setting_useExposure = true;
float setting_affineOptModeA = 1e12;
float setting_affineOptModeB = 1e8;
float setting_affineOptModeA_huberTH = 10000;
float setting_affineOptModeB_huberTH = 10000;

int setting_gammaWeightPixelSelect = 1;
bool setting_relinAlways = true;
bool setting_fixCalib = false;  // its a photmetric calibration ig...?
bool setting_activateAllOnMarg = false;
bool setting_forceAceptStep = true;  // may be it should be called AcceptStep??

float setting_useDepthWeightsCoarse = -1;
bool setting_dilateDoubleCoarse = false;
float setting_huberTH = 9;
bool setting_logStuff = true;  // whether to log stuff

float benchmarkSetting_fxfyfac = 0;
int benchmarkSetting_width = 0;
int benchmarkSetting_height = 0;
float benchmark_varNoise = 0;
float benchmark_varBlurNoise = 0;
int benchmark_noiseGridsize = 3;
float benchmark_initializerSlackFactor = 1;

float setting_frameEnergyTHConstWeight = 0.5;
float setting_frameEnergyTHN = 0.7f;
float setting_frameEnergyTHFacMean = 0.0;
float setting_frameEnergyTHFacMedian = 1.5;
float setting_overallEnergyTHWeight = 1;
float setting_coarseCuttoffTH = 20;
float setting_minGradHistCut = 0.5;
float setting_minGradHistAdd = 7;
float setting_fixGradTH = -1;

float setting_gradDownweightPerLevel = 0.75;
bool setting_selectDirectionDistribution = true;
int setting_pixelSelectionUseFast = 0;
float setting_trace_stepsize = 1.0;  // step size for initial discrete search
int setting_trace_GNIterations = 3;  // max number of iterations for GN optimization

float setting_trace_GNThreshold = 0.1;     // threshold for GN optimization
float setting_trace_extraSlackOnTH = 1.2;  // for energy based outlier check, be slightly more relaxed by this factor
float setting_trace_slackInterval = 1.5;   // if pixel interval is smaller than this, leave it be
float setting_trace_minImprovementFactor = 2;  // if pixel interval is smaller than this, leave it be

// here are some settings for the 3D viewer
bool setting_render_displaycoarseTrackingFull = false;
bool setting_render_renderWindowFrames = true;
bool setting_render_plotTrackingFull = false;
bool setting_render_display3D = true;
bool setting_render_displayResidual = true;
bool setting_render_displayVideo = true;
bool setting_render_displayDepth = true;

bool setting_render_fullResetRequested = false;
bool setting_debugout_runquiet = false;
bool disableAllDisplay = false;
bool disableReconfigure = false;
bool setting_onlyLogKFPoses = true;
bool debugSaveImages = false;
int sparsityFactor = 5;
bool goStepByStep = false;
// extern bool plotStereoImages = ; // Not defined in original code base
bool multiThreading = true;

float freeDebugParam1 = 1;
float freeDebugParam2 = 1;
float freeDebugParam3 = 1;
float freeDebugParam4 = 1;
float freeDebugParam5 = 1;

int benchmarkSpecialOption = 0;
bool setting_pause = false;
int setting_pointSelection = 1;  // 0-DSO's selection. 1-LDSO's selection, 2-Random selection


 bool setting_enableLoopClosing = true;
 bool setting_fastLoopClosing = true;
 bool setting_showLoopClosing = false;

 void handleKey(char k){

char kkk =k;
switch (kkk) {
    case 'd':
    case 'D':
        freeDebugParam5 = ((int)(freeDebugParam5 +1)) %10;
        cout << "new freeDebugParam5: " << freeDebugParam5 << endl;
        break;
    case 's':
    case 'S':
        freeDebugParam5 = ((int)(freeDebugParam5 -1 +10)) %10;
        cout << "new freeDebugParam5: " << freeDebugParam5 << endl;
        break;

}

 }


// patterns, see Settings.cc
extern int staticPattern[10][40][2];
extern int staticPatternNum[10];
extern int staticPatternPadding[10];

// use the ninth pattern (described in DSO paper) for residual calculation
#define patternP staticPattern[8]

}  // namespace ldso