#pragma once
#include <Eigen/src/Core/util/Macros.h>
#include <memory>
#ifndef LDSO_SLAM_GLOBAL_FUNCS_H_
#define LDSO_SLAM_GLOBAL_FUNCS_H_

#include "NumTypes.h"

namespace ldso 
{   
    // the global functions used in dso, for interpolation() and visualization(i.e rainbow, jet color ramps)
    namespace interanl
    {

        // vector operations (removing)
        template <typename T>
        inline void deleteOut(std::vector<T *>& v, const int i)
        {
            v[i] = v.back();
            v.pop_back();
        }

        template <typename T>
        inline void deleteOut(std::vector<shared_ptr<T>>& v, const int i)
        {
            v[i] = v.back();
            v.pop_back();
        }

        template <typename T>
        inline void deleteOut(std::vector<shared_ptr<T>>& v, const shared_ptr<T> &e)
        {
            for(shared_ptr<T> &t : v){
                if(t == e){
                    t = v.back();
                    v.pop_back();
                    break;
                }
            }
        }

        template <typename T>
        inline void deleteOutOrder(std::vector<T>& v, const T element)
        {
            int i=-1;
            for(unsigned int k=0; k<v.size(); k++){
                if(v[k] == element){
                    i = k;
                    break;
                }
            }
            assert(i!=-1);
            for(unsigned int k=i+1; k<v.size(); k++){
                v[k-1] = v[k];
            }
            v.pop_back();
        }

        // interpolations

        // Reads interpolated element from a float array
        EIGEN_ALWAYS_INLINE float
        getInterpolatedElement(const float *const mat, const float x, const float y, const int width)
        {
            int ix = (int)x;
            int iy = (int)y;
            

            float dx = x - ix;
            float dy = y - iy;
            float dxdy = dx * dy;
            
            const float *bp = mat + iy * width + ix;

        }

    }
}






#endif  // LDSO_SLAM_GLOBAL_FUNCS_H_