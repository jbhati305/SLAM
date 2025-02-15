#pragma once
#include <cstddef>
#include <string>
#ifndef LDSO_SLAM_DATASETREADER_H
#define LDSO_SLAM_DATASETREADER_H

#include <algorithm>
#include <boost/format.hpp>
#include <dirent.h>
#include <fstream>
#include <glog/logging.h>
#include <sstream>

// our own includes
#include "Settings.h"
/// TODO: #include "frontend/Undistort.h"
/// TODO: #include "frontend/ImageRW.h"
/// TODO: #include "frontend/ImageAndExposure.h"

#include "internal/GlobalFuncs.h"
#include "internal/GlobalCalib.h"

#if HAS_LIBZIP

#include "zip.h"

#endif

#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace ldso;
using namespace ldso::internal;

// HELPER FUNCTIONS for accessing the file system

// This function loops through the directory , adds "jpg" files to the "files"
// vector and returns the number of files
inline int getdir(std::string dir, std::vector<std::string>& files)
{
  DIR* dp;
  struct dirent* dirp;
  if ((dp = opendir(dir.c_str())) == NULL)
  {
    // Cannot open directory
    return -1;
  }

  while ((dirp = readdir(dp)) != NULL)
  {
    std::string name = std::string(dirp->d_name);
    if (name != "." && name != ".." && name.substr(name.size() - 3, name.size()) == "jpg")
    {
      // Add files which end with the .jpg extension
      files.push_back(name);
    }
  }
  closedir(dp);

  std::sort(files.begin(), files.end());
  if (dir.at(dir.length() - 1) != '/')
  {
    dir = dir + "/";
  }

  for (unsigned int i = 0; i < files.size(); i++)
  {
    if (files[i].at(0) != '/')
    {
      files[i] = dir + files[i];
    }
  }

  LOG(INFO) << "file size: " << files.size() << endl;
  return files.size();
}

struct PreImageItem
{
  int id;
  bool isQueud;

  /*
  ImageAndExposure* pt;

  inline PrepImageItem(int _id){
      id = _id;
      isQueud = false;
      pt = 0;
  }
  inline void release(){
      if(pt!=0){
          delete pt;
          pt = 0;
      }
  }
  */
};

class ImageFolderReader
{
public:
  enum DatasetType
  {
    TUM_MONO,  // WE WILL USE THIS
    KITTI,
    EUROC
  };

  ImageFolderReader(DatasetType datasetType, std::string path, std::string calibFile, std::string gammaFile,
                    std::string vignetteFile)
  {
    this->datasetType = datasetType;
    this->path = path;
    this->calibFile = calibFile;
    // this->gammaFile = gammaFile;
    // this->vignetteFile = vignetteFile;

#if HAS_LIBZIP
    ziparchive = 0;
    databuffer = 0;
#endif

    isZipped = (path.length() > 4 && path.substr(path.length() - 4) == ".zip");
    if (datasetType == TUM_MONO)
    {
      if (isZipped)
      {
#if HAS_LIBZIP
        int ziperror = 0;
        ziparchive = zip_open(path.c_str(), ZIP_RDONLY, &ziperror);
        if (ziperror != 0)
        {
          LOG(ERROR) << "ERROR: " << "Error opening zip file: " << path.c_str() << endl;
          exit(1);
        }

        files.clear();
        int numEntries = zip_get_num_entries(ziparchive, 0);
        for (int k = 0; k < numEntries; k++)
        {
          const char* name = zip_get_name(ziparchive, k, 0);
          std::string nameStr = std::string(name);
          if (nameStr == "." || nameStr == "..")
          {
            continue;
          }
          files.push_back(nameStr);
        }

        cout << "Got " << numEntries << " entries and" << (int)files.size() << " files." << endl;
        std::sort(files.begin(), files.end());

#else
        LOG(FATAL)
            << ("ERROR: cannot read .zip archive, as compiled without "
                "ziplib!\n");

#endif
      }
      else
      {
        getdir(path, files);
      }
    }

   ///TODO: undistort = Undistort::getUndistorterForFile(calibFile, gammaFile, vignetteFile);

    /// TODO:
    /*
    widthOrg = undistort->getOriginalSize()[0];
    heightOrg = undistort->getOriginalSize()[1];
    width = undistort->getSize()[0];
    height = undistort->getSize()[1];
    */

    // load timestamps if available
    if (datasetType == TUM_MONO)
    {
      loadTimestamps();
    }

    else if (datasetType == KITTI)
    {
      loadTimestampsKITTI();
    }

    else if (datasetType == EUROC)
    {
      loadTimestampsEUROC();
    }
  }

  ~ImageFolderReader()
  {
#if HAS_LIBZIP
    if (ziparchive != 0)
    {
      zip_close(ziparchive);
    }
    if (databuffer != 0)
    {
      delete databuffer;
    }
#endif  // HAS_LIBZIP

   /// TODO: delete undistort;
  };

  // Helper Methods
  /// TODO:
  /*
  Eigen::VectorXf getOriginalCalib()
  {
    return undistort->getOriginalParameter().cast<float>();
  }

  Eigen::Vector2i getOriginalDimensions()
  {
    return undistort->getOriginalSize();
  }
  */

  void getCalibMono(Eigen::Matrix3f& K, int& w, int& h)
  {
    /// TODO:
    /*
    K = undistort->getK().cast<float>();
    w = undistort->getSize()[0];
    h = undistort->getSize()[1];
    */
  }

 
  void setGlobalCalibration()
  {
    int w_out, h_out;
    Eigen::Matrix3f K;
    getCalibMono(K, w_out, h_out);
    setGlobalCalib(w_out, h_out, K);
  }
  

  int getNumImages()
  {
    return files.size();
  }

  double getTimestamp(int id)
  {
    if (timestamps.size() == 0)
    {
      return id * 0.1f;
    }
    if (id >= (int)timestamps.size())
    {
      return 0;
    }
    if (id < 0)
    {
      return 0;
    }
    return timestamps[id];
  }

  void prepImage(int id, bool as8U = false)
  {
    /// TODO: Empty in original codebase
  }

/// TODO:
/*
  MinimalImageB* getImageRaw(int id)
  {
    return getImageRaw_internal(id, 0);
  }

  ImageAndExposure* getImage(int id, bool foreceLoadDirectly = false)
  {
    return getImage_internal(id, 0);
  }
  */

  inline float* getPhotometricGamma()
  {
/// TODO:
/*
    if (undistort == 0 || undistort->photometricUndist == 0)
    {
      return 0;
    }
    return undistort->photometricUndist->getG();  // returns the gamma value
  */
  return 0;
  }

  /// TODO: Undistort* undistort;

private:

/// TODO:
/*
  MinimalImageB* getImageRaw_internal(int id, int unused)
  {
    if (!isZipped)
    {
      return IOWrap::readImageBW_8U(files[id]);
    }
    else
    {
#if HAS_LIBZIP
      if (databuffer == 0)
      {
        databuffer = new char[widthOrg * heightOrg * 6 + 10000];
      }

      zip_file_t* file = zip_fopen(ziparchive, files[id].c_str(), 0);
      long readbytes = zip_fread(file, databuffer, (long)widthOrg * heightOrg * 6);

      if (readbytes != widthOrg * heightOrg * 6)
      {
        cout << "read " << readbytes << "/" << (long)widthOrg * heightOrg * 6 + 10000 << " bytes for file "
             << files[id].c_str() << ". increase buffer!!" << endl;

        delete[] databuffer;
        databuffer = new char[(long)widthOrg * heightOrg * 30];
        file = zip_fopen(ziparchive, files[id].c_str(), 0);
        readbytes = zip_fread(file, databuffer, (long)widthOrg * heightOrg * 30);

        if (readbytes > (long)widthOrg * heightOrg * 30)
        {
          cout << "Buffer still too small (read " << readbytes << "/" << (long)widthOrg * heightOrg * 30
               << " bytes for file " << files[id].c_str() << ". increase buffer!!" << endl;
          exit(1);
        }
      }

      return IOWrap::readStreamBW_8U(databuffer, widthOrg, heightOrg);
#else
      LOG(FATAL) << "ERROR: cannot read .zip archive, as compiled without ziplib!\n";
      exit(1);

#endif
    }
  }
  */

  /// TODO:
  /*
  ImageAndExposure* getImage_internal(int id, int unused)
  {
    MinimalImageB* minimg = getImageRaw_internal(id, 0);

    ImageAndExposure* imgexp = undistort->undistort<unsigned char>(minimg, (exposure.size() == 0 ? 1.0f : exposure[id]),
                                                                   (timestamps.size() == 0 ? 0.0f : timestamps[id]));
    delete minimg;
    return imgexp;
  }
  */

  inline void loadTimestamps()
  {
    LOG(INFO) << "Loading timestamps from TUM Mono dataset" << endl;
    std::ifstream tr;
    std::string timesFile = path.substr(0, path.find_last_of('/')) + "/times.txt";

    tr.open(timesFile.c_str());
    if (!tr)
    {
      // try last directory
      timesFile = path.substr(0, path.find("images")) + "times.txt";

      LOG(INFO) << "trying!!" << timesFile << endl;
      tr.close();
      tr.open(timesFile.c_str());
    }

    while (!tr.eof() && tr.good())
    {
      std::string line;
      char buf[1000];
      tr.getline(buf, 1000);

      int id;
      double stamp;
      float exposure = 0;

      if (3 == sscanf(buf, "%d %lf %f", &id, &stamp, &exposure))
      {
        timestamps.push_back(stamp);
        exposures.push_back(exposure);
      }

      else if (2 == sscanf(buf, "%d %lf", &id, &stamp))
      {
        timestamps.push_back(stamp);
        exposures.push_back(exposure);
      }
    }
    tr.close();
    LOG(INFO) << "files: " << files.size() << " timestamps: " << timestamps.size()
              << " exposure times: " << exposures.size() << endl;

    // check if exposures are correct, (possibly skip)
    bool exposuresGood = ((int)exposures.size() == (int)getNumImages());
    for (int i = 0; i < (int)exposures.size(); i++)
    {
      if (exposures[i] == 0)
      {
        // fix!
        float sum = 0, num = 0;
        if (i > 0 && exposures[i - 1] > 0)
        {
          sum += exposures[i - 1];
          num++;
        }

        if (i + 1 < (int)exposures.size() && exposures[i + 1] > 0)
        {
          sum += exposures[i + 1];
          num++;
        }

        if (num > 0)
        {
          exposures[i] = sum / num;
        }
      }

      if (exposures[i] == 0)
      {
        // We have not fixed the exposure values
        exposuresGood = false;
      }
    }

    if ((int)getNumImages() != (int)timestamps.size())
    {
      // Number of images does not match number of timestamps
      cout << "Set timestamps and exposures to zero!" << endl;
      exposures.clear();
      timestamps.clear();
    }
    if ((int)getNumImages() != (int)exposures.size() || !exposuresGood)
    {
      // Number of images does not match number of exposures OR exposures bad!
      cout << "Set exposures to zero!" << endl;
      exposures.clear();
    }

    LOG(INFO) << "Got " << (int)getNumImages() << " images and " << (int)timestamps.size() << " timestamps and "
         << (int)exposures.size() << " exposures." << endl;

  }

  inline void loadTimestampsEUROC()
  {
    LOG(INFO) << "Loading timestamps from EUROC dataset" << endl;
    std::ifstream tr;
    std::string timesFile = path + "/data.csv";

    tr.open(timesFile.c_str());
    if (!tr)
    {
      LOG(INFO) << "cannot find timestamp file! Something maybe wrong in the "
                   "dataset setting!"
                << endl;
      LOG(INFO) << timesFile << endl;
      return;
    }

    while (!tr.eof() && tr.good())
    {
      std::string line;
      char buf[1000];
      tr.getline(buf, 1000);

      double stamp;
      char filename[256];
      if (line[0] == '#')
      {
        continue;  // skip comments
      }

      if (2 == sscanf(buf, "%lf,%s", &stamp, filename))
      {
        timestamps.push_back(stamp * 1e-9);
        files.push_back(path + "/data/" + string(filename));
      }
    }
    tr.close();
    LOG(INFO) << "Load total " << timestamps.size() << " data entries." << endl;
  }

  inline void loadTimestampsKITTI()
  {
    LOG(INFO) << "Loading timestamps from KITTI dataset" << endl;
    std::ifstream tr;
    std::string timesFile = path + "/times.txt";

    tr.open(timesFile.c_str());
    if (!tr)
    {
      LOG(INFO) << "cannot find timestamp file at" << timesFile
                << " Something maybe wrong in the "
                   "dataset setting!"
                << endl;
      LOG(INFO) << timesFile << endl;
      return;
    }

    while (!tr.eof() && tr.good())
    {
      char buf[1000];
      tr.getline(buf, 1000);

      char filename[256];
      if (buf[0] == 0)
      {
        break;  // skip comments
      }

      double stamp = atof(buf);

      if (std::isnan(stamp))
      {
        break;
      }
      timestamps.push_back(stamp * 1e-9);
    }
    tr.close();

    boost::format fmt("%s/image_0/%06d.png");
    for (size_t i = 0; i < timestamps.size(); i++)
    {
      files.push_back((fmt % path % i).str());
    }

    LOG(INFO) << "Load total " << timestamps.size() << " data entries." << endl;
  }


  /// TODO: std::vector<ImageAndExposure *> preloadedImages;
  std::vector<std::string> files;
  std::vector<double> timestamps;
  std::vector<float> exposures;
  DatasetType datasetType;

  int width, height;
  int widthOrg, heightOrg;

  std::string  path;
  std::string calibFile;

  bool isZipped;

  #if HAS_LIBZIP
          zip_t* ziparchive;
          char* databuffer;
  #endif


};

#endif  // LDSO_SLAM_DATASETREADER_H