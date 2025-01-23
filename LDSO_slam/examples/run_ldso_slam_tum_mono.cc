#include <iostream>
#include <thread>

#include <clocale>
#include <cmath>
#include <csignal>
#include <cstdio>  // TODO: this can be removed ig...
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>

#include <glog/logging.h>

#include "DatasetReader.h"
#include "Settings.h"

// TODO: add includes (Full system)
// #include "frontend/FullSystem.h"

using namespace std;
using namespace ldso;

// TODO: add global variables
string vignette = " ";
string gammaCalib = " ";  // photometric calibration
string calib = " ";       // geometric calibration
string source = " ";      // source of the images

string output_file = "./result.txt";
string vocPath = "./vocab/orbflow.dbow3";

double rescale = 1.0;
bool reversePlay = false;
bool disableROS = false;
int startIdx = 0;      // index of the first frame
int endIdx = 1000000;  // index of the last frame
bool prefetch = false;
float playbackSpeed = 0;  // 0 means maximum speed
bool preload = false;

bool useSampleOutput = false;

// Select Proper preset (number of feature points, calibration mode etc.)
void settingDefault(int preset)
{
  cout << "-----------Preset Setting-----------" << endl;
  if (preset == 0 || preset == 1)
  {
    cout << "Default Settings" << endl
         << "- " << (preset == 0 ? "No" : "1x") << " real-time enforcement" << endl
         << "- 2000 feature points" << endl
         << "- 5-7 active frames" << endl
         << "- 1-6 Levenberg Marquet iterations each KeyFrame" << endl
         << "- original image resolution" << endl;

    playbackSpeed = (preset == 0 ? 0 : 1);
    preload = preset == 1;
    setting_desiredImmatureDensity = 1500;
    setting_desiredPointDensity = 2000;
    setting_minFrames = 5;
    setting_maxFrames = 7;
    setting_minOptIterations = 1;
    setting_maxOptIterations = 6;
    setting_logStuff = false;
  }
  if (preset == 2 || preset == 3)
  {
    cout << "Fast Settings" << endl
         << "- " << (preset == 2 ? "No" : "5x") << " real-time enforcement" << endl
         << "- 800 feature points" << endl
         << "- 4-6 active frames" << endl
         << "- 1-4 Levenberg Marquet iterations each KeyFrame" << endl
         << "- 424 x 320 image resolution" << endl;

    playbackSpeed = (preset == 2 ? 0 : 5);
    preload = preset == 3;

    setting_desiredImmatureDensity = 600;
    setting_desiredPointDensity = 800;
    setting_minFrames = 4;
    setting_maxFrames = 6;
    setting_minOptIterations = 1;
    setting_maxOptIterations = 4;
    benchmarkSetting_width = 424;
    benchmarkSetting_height = 320;
    setting_logStuff = false;
  }
  cout << "------------------------------------" << endl;
}

// TODO: add parsing functions
void parseArgument(char* arg)
{
  // seperating the argument name and value
  int option;
  float foption;
  char buffer[100];
  if (1 == sscanf(arg, "sampleoutput=%d", &option))
  {
    if (option == 1)
    {
      cout << "SAMPLE OUTPUT ENABLED!!" << endl;
      useSampleOutput = true;
    }
    return;
  }
  if (1 == sscanf(arg, "quiet=%d", &option))
  {
    if (option == 1)
    {
      setting_debugout_runquiet = true;
      cout << "QUIET MODE ENABLED!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "preset=%d", &option))
  {
    settingDefault(option);
    return;
  }
  if (1 == sscanf(arg, "rec=%d", &option))
  {
    if (option == 0)
    {
      disableReconfigure = true;
      cout << "DISABLE RECONFIGURE!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "noros=%d", &option))
  {
    if (option == 1)
    {
      disableReconfigure = true;
      disableROS = true;
      cout << "DISABLE ROS (AND RECONFIGURE)!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "nolog=%d", &option))
  {
    if (option == 1)
    {
      setting_logStuff = false;
      cout << "DISABLE LOGGING!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "reversePlay=%d", &option))
  {
    if (option == 1)
    {
      reversePlay = true;
      cout << "REVERSE PLAY!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "nogui=%d", &option))
  {
    if (option == 1)
    {
      disableAllDisplay = true;
      cout << "NO GUI!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "nomt=%d", &option))
  {
    if (option == 1)
    {
      multiThreading = false;
      cout << "NO MULTITHREADING!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "prefetch=%d", &option))
  {
    if (option == 1)
    {
      prefetch = true;
      cout << "PREFETCH!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "start=%d", &option))
  {
    if (option == 1)
    {
      startIdx = option;
      cout << "START AT " << startIdx << "!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "end=%d", &option))
  {
    if (option == 1)
    {
      endIdx = option;
      cout << "END AT " << endIdx << "!!" << endl;
    }
    return;
  }
  if (1 == sscanf(arg, "loopclosing=%d", &option))
  {
    if (option == 1)
    {
      setting_enableLoopClosing = true;
    }
    else
    {
      setting_enableLoopClosing = false;
    }
    cout << "LOOP CLOSING " << (setting_enableLoopClosing ? "ENABLED" : "DISABLED") << endl;
    return;
  }

  if (1 == sscanf(arg, "files=%s", buffer))
  {
    source = buffer;
    cout << "DATA SOURCE: " << source.c_str() << " !!" << endl;
    return;
  }
  if (1 == sscanf(arg, "vocab=%s", buffer))
  {
    vocPath = buffer;
    cout << "loading vocabulary from: " << vocPath.c_str() << " !!" << endl;
    return;
  }
  if (1 == sscanf(arg, "calib=%s", buffer))
  {
    calib = buffer;
    cout << "loading calib from: " << calib.c_str() << " !!" << endl;
    return;
  }
  if (1 == sscanf(arg, "vignette=%s", buffer))
  {
    vignette = buffer;
    cout << "loading vignette from: " << vignette.c_str() << " !!" << endl;
    return;
  }
  if (1 == sscanf(arg, "gamma=%s", buffer))
  {
    gammaCalib = buffer;
    cout << "loading gammaCalib from: " << gammaCalib.c_str() << " !!" << endl;
    return;
  }
  if (1 == sscanf(arg, "rescale=%f", &foption))
  {
    playbackSpeed = foption;
    cout << "playbackSpeed: " << playbackSpeed << " !!" << endl;
    return;
  }
  if (1 == sscanf(arg, "output=%s", buffer))
  {
    output_file = buffer;
    LOG(INFO) << "Output set to: " << output_file.c_str() << " !!" << endl;
    return;
  }

  if (1 == sscanf(arg, "save=%d", &option))
  {
    if (option == 1)
    {
      /// TODO:  debugSaveImages = option;

      /// TODO: Do we really need these system calls?
      if (42 == system("rm -rf images_out"))
      {
        cout << "System call returned 42- What are the odds?. This is only "
                "here to shut up the compiler"
             << endl;
      }
      if (42 == system("mkdir images_out"))
      {
        cout << "System call returned 42- What are the odds?. This is only "
                "here to shut up the compiler"
             << endl;
      }
      if (42 == system("rm -rf images_out"))
      {
        cout << "System call returned 42- What are the odds?. This is only "
                "here to shut up the compiler"
             << endl;
      }
      if (42 == system("rm -rf images_out"))
      {
        cout << "System call returned 42- What are the odds?. This is only "
                "here to shut up the compiler"
             << endl;
      }
      if (42 == system("mkdir images_out"))
      {
        cout << "System call returned 42- What are the odds?. This is only "
                "here to shut up the compiler"
             << endl;
      }
      cout << "SAVE IMAGES!!" << endl;
    }
    return;
  }

  if (1 == sscanf(arg, "mode=%d", &option))
  {
    if (option == 0)
    {
      cout << "PHOTOMETRIC MODE WITH CALIBRATION!!" << endl;
    }
    if (option == 1)
    {
      cout << "PHOTOMETRIC MODE WITHOUT CALIBRATION!!" << endl;
      setting_photometricCalibration = 0;
      setting_affineOptModeA = 0;  // -1 FIX, >=0: OPTIMIZE (with prior, if >0)
      setting_affineOptModeB = 0;  // -1 FIX, >=0: OPTIMIZE (with prior, if >0)
    }
    if (option == 2)
    {
      cout << "PHOTOMETRIC MODE WITH PERFECT IMAGES!!" << endl;
      setting_photometricCalibration = 0;
      setting_affineOptModeA = 0;  // -1 FIX, >=0: OPTIMIZE (with prior, if >0)
      setting_affineOptModeB = 0;  // -1 FIX, >=0: OPTIMIZE (with prior, if >0)
      setting_minGradHistAdd = 3;
    }
    return;
  }
  cout << "could not parse argument: " << arg << " !!!" << endl;
}

int main(int argc, char** argv)
{
  std::cout << "Hello, World!" << std::endl;

  FLAGS_colorlogtostderr = true;
  // parse command line arguments
  for (int i = 1; i < argc; i++)
  {
    // cout << "Argument " << i << " is " << argv[i] << endl;
    parseArgument(argv[i]);
  }

  // Check settings coflict

  if (setting_enableLoopClosing && (setting_pointSelection != 1))
  {
    LOG(ERROR) << "Loop closing is enabled but the point selection strategy is "
                  "not set to LDSO, use setting_pointSelection=1! Please!!"
               << endl;
    exit(-1);
  }

  if (setting_showLoopClosing == true)
  {
    LOG(WARNING) << "show loop closing results, The program will be paused "
                    "when a loop is detected!!"
                 << endl;
  }

  /// TODO: shared_ptr<ImageFolderReader> reader(new
  /// ImageFolderReader(ImageFolderReader::TUM_MONO, source, calib,
  // gammaCalib,vignette));

  /// TODO: reader->setGlobalCalibration();
  /// TODO:
  /*
if(setting_photometricCalibration >0 && reader->getPhotometricGamma() == 0){
  LOG(ERROR)<< "ERROR: Don't have photometric calibration. Need to use
commandline options mode 1 or 2!!"<<endl; exit(1);
}
*/

  // for stepping through the images
  int lstart = startIdx;
  int lend = endIdx;
  int linc = 1;

  if (reversePlay)
  {
    LOG(INFO) << "Reverse play enabled!!";
    lstart = endIdx - 1;

    /// TODO:
    /*
    if(lstart>= reader->getNumImages()){
      lstart = reader->getNumImages()-1;
    }
    */
    lend = startIdx;
    linc = -1;
  }

  // Load the ORB-Vocabulary used for loop closure
  /// TODO:
  /*
shared_ptr<ORBVocabulary> voc(new ORBVocabulary());
voc->load(vocPath);
*/

  // Initailize the FullSystem which is the core of our SLAM system
  /// TODO:
  /*
shared_ptr<FullSystem> fullSystem(new FullSystem(voc));
fullSystem->setGammaCalibration(reader->photometricGamma());
fullSystem->linearizeOperation = (playbackSpeed == 0);
*/

  // Initialize Graphical User Interface using Pangolin
  /// TODO:
  /*
shared_ptr<PangolinDSOViewer> viewer = nullptr;
if(!disableAllDisplay){
  viewer= shared_ptr<PangolinDSOViewer>(new PangolinDSOViewer(wG[0], hG[0],
false)); // initialize the pangolin window with specific width and height
fullSystem->setViewer(viewer);

}
else{
  LOG(INFO) << "Visualization is Disabled!!";
}
*/

  // Start the main loop WHICH RUNS ON A SEPARATE THREAD
  std::thread runthread([&]() {
    std::vector<int> idsToPlay;
    std::vector<double> timesToPlayAt;

    // Prepare the correct timestamps for each image:
    /// TODO:
    /*
    for (int i = lstart; i >= 0 && i < reader->getNumImages() && linc * i < linc
    * lend; i += linc)
    {
      idsToPlay.push_back(i);
      if (timesToPlay.size() == 0)
      {
        timestampsToPlay.push_back((double)0);
      }
      else
      {
        double tsThis = reader->getTimestamp(idsToPlay[idsToPlay.size() - 1]);
        double tsPrev = reader->getTimestamp(idsToPlay[idsToPlay.size() - 2]);
        timestampsToPlay.push_back(timestampsToPlay.back() + fabs(tsThis -
    tsPrev) / playbackSpeed);
      }
    }
    */

    // Load all images we need in a datastructure in out memory
    /// TODO:
    /*
    std::vector<ImageAndExposure*> preloadedImages;
    if (prelaod)
    {
      cout << "Preloading images..." << endl;
      for (int ii = 0; ii < (int)idsToPlay.size(); ii++)
      {
        preloadedImages.push_back(reader->getImage(ii));
      }
    }
    */

    // Start a stop watch in order to measure performance on a dataset
    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);
    clock_t started = clock();
    double sInitializerOffset = 0;

    // Here is the main loop for visual slam implementation - Loop through all
    // the images
    for (int ii = 0; ii < (int)idsToPlay.size(); ii++)
    {
      // Load the image from the reader
      
      while (setting_pause == true)
      {
        usleep(5000);
      }
      

      /// TODO:
      /*
      if (!fullSystem->initialized)
      {
        gettimeofday(&tv_start, NULL) started = clock();
        sInitializerOffset = timesToPlayAt[ii];
      }
      */

      int i = idsToPlay[ii];

      /// TODO:
      /*
      ImageAndExposure* img;  // The main datatype for the image
      if (preload)
      {
        img = preloadedImages[ii];
      }
      else
      {
        img = reader->getImage(i);
      }
      */

      bool skipFrame = false;
      if (playbackSpeed != 0)
      {
        struct timeval tv_now;
        gettimeofday(&tv_now, NULL);
        double sSinceStart = sInitializerOffset + ((tv_now.tv_sec - tv_start.tv_sec) +
                                                   (tv_now.tv_usec - tv_start.tv_usec) / (1000.0f * 1000.0f));
        if (sSinceStart < timesToPlayAt[ii])
        {
          /// TODO: Can be removed as it is commented anyway
          // usleep((int)) ((timesToPlayAt[ii] - sSinceStart) * 1000.0f *
          // 1000.0f);
        }
        else if (sSinceStart > timesToPlayAt[ii] + 0.5 + 0.1 * (ii % 2))
        {
          cout << "SKIPFRAM " << ii << "(play at " << timesToPlayAt[ii] << ", now it is " << sSinceStart << ")" << endl;
          skipFrame = true;
        }
      }

      if (!skipFrame)
      {
        // Here the real nice magic is happening!!
        /// TODO: fullSystem->addActiveFrame(img, i);
      }
      /// TODO: delete img;

      // Clean up the memory
      /// TODO:
      /*
      if(fullSystem->initFailed || setting_fullResetRequested){

        if(ii<250|| setting_fullResetRequested){
          LOG(INFO)<< "Init failed, resetting!!";
          fullSystem->shared_ptr<FullSystem>(new FullSystem(voc));
          fullSystem->setGammaFuntion(reader->photometricGamma());
          fullSystem->linearizeOperation = (playbackSpeed == 0);
          if(viewer){
            viewer->reset();
            sleep(1);
            fullSystem->setViewer(viewer);
          }
          setting_fullResetRequested = false;
        }

      }
      */

      /// TODO:
      /*
      if(fullSystem->isLost){
        LOG(INFO)<< "LOST!!";  // if the camera is fast moving or does not
      provide enough information, environment is changing very drastically
      break;
      }
      */
    }

    /// TODO:  fullSystem->blockUntilMappingIsFinished();

    // End the stop watch
    clock_t ended = clock();
    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    // Statistics: Useful for data analysis
    /// TODO: fullSystem->printResult(output_file, true);
    /// TODO: fullSystem->printResult(output_file + ".noloop", false);

    /// TODO: SEGMENATION FAULT
    /*
    int numFramesProcessed = abs(idsToPlay[0] - idsToPlay.back());
    double numSecondsProcessed = 1234.0;  // TODO:
    fabs(reader->getTimestamp(idsToPlay[0]) -
    reader->getTimestamp(idsToPlay.back())); double MilliSecondsTakenSingle =
    1000.0f * (ended - started) / (float)(CLOCKS_PER_SEC); double
    MilliSecondsTakenMT = sInitializerOffset + ((tv_end.tv_sec -
    tv_start.tv_sec) * 1000.0f + (tv_end.tv_usec - tv_start.tv_usec) / 1000.0f);

    cout << endl
         << "-------------------------------" << endl
         << numFramesProcessed << "Frames (" << numSecondsProcessed /
    numSecondsProcessed << " fps)" << endl
         << MilliSecondsTakenSingle / numFramesProcessed << "ms per frame
    (single);" << endl
         << MilliSecondsTakenMT / (float)numFramesProcessed << "ms per frame
    (MT);" << endl
         << 1000 / (MilliSecondsTakenSingle / numFramesProcessed) << "x (single
    core);" << endl
         << 1000 / (MilliSecondsTakenMT / numFramesProcessed) << "x (multi
    core);" << "-------------------------------"
         << endl;
    */

    /// TODO:
    /*
        if (setting_logStuff)
        {
          std::ofstream tmlog;
          tmlog.open("logs/time.txt", std::ios::trunc | std::ios::out);
          tmlog << 1000.0f * (ended - started) / (float)(CLOCKS_PER_SEC *
       reader->getNumImages()) << " "
                << ((tv_end.tv_sec - tv_start.tv_sec) * 1000.0f +
       (tv_end.tv_usec - tv_start.tv_usec) / 1000.0f) /
                       (float)reader->getNumImages()
                << " " << endl;
          tmlog.flush();
          tmlog.close();
        }
        */
  });

  /// TODO:
  /*
  if(viewer){
    viewer->run(); //mac os should keep this in the main thread
  }
  */
  runthread.join();  // this will take a while...

  // TODO: viewer->saveAsPLYFille("./pointcloud.ply");
  LOG(INFO) << "EXIT NOW!!";

  return 0;
}