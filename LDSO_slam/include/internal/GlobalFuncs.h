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

            // Idea: we use pointer arithmetic to access a specific pixel value
            // at a certain pixel in image we take some pixels around it and interpolate
            const float *bp = mat + iy * width + ix;

            float res = dxdy * bp[1+width]
                        + (dx - dxdy) * bp[width]
                        + (dy - dxdy) * bp[1]
                        + (1 - dx - dy + dxdy) * bp[0];
            
            return res;
        }

        // Get interpolated value from Vector4f mat
        // return interpolated Vector3f by throwing away the last dimension
        EIGEN_ALWAYS_INLINE Eigen::Vector3f
        getInterpolatedElement43(const Eigen::Vector4f *const mat, const float x, const float y, const int width)
        {
            int ix = (int)x;
            int iy = (int)y;
            

            float dx = x - ix;
            float dy = y - iy;
            float dxdy = dx * dy;

            
            const Eigen::Vector4f *bp = mat + iy * width + ix;

            Eigen::Vector3f res = dxdy * *(const Eigen::Vector3f *) (bp+1+width)
                        + (dx - dxdy) * *(const Eigen::Vector3f *) (bp+width)
                        + (dy - dxdy) * *(const Eigen::Vector3f *) (bp+1)
                        + (1 - dx - dy + dxdy) * *(const Eigen::Vector3f *) (bp);
            
            return res;
        }

        // get interpolated value from Vector3f mat
        // return interpolated Vector3f
        EIGEN_ALWAYS_INLINE Eigen::Vector3f
        getInterpolatedElement33(const Eigen::Vector3f *const mat, const float x, const float y, const int width)
        {
            int ix = (int)x;
            int iy = (int)y;
            
            float dx = x - ix;
            float dy = y - iy;
            float dxdy = dx * dy;

            const Eigen::Vector3f *bp = mat + iy * width + ix;

            Eigen::Vector3f res = dxdy * *(const Eigen::Vector3f *) (bp+1+width)
                        + (dx - dxdy) * *(const Eigen::Vector3f *) (bp+width)
                        + (dy - dxdy) * *(const Eigen::Vector3f *) (bp+1)
                        + (1 - dx - dy + dxdy) * *(const Eigen::Vector3f *) (bp);
            
            return res;
        }

        // interpolation with "and" mask
        EIGEN_ALWAYS_INLINE Eigen::Vector3f
        getInterpolatedElement33OvenAnd(const Eigen::Vector3f *const mat ,const bool *overMat,
                                         const float x, const float y, const int width, bool &over_out)
        {
            int ix = (int)x;
            int iy = (int)y;
            
            float dx = x - ix;
            float dy = y - iy;
            float dxdy = dx * dy;

            const Eigen::Vector3f *bp = mat + iy * width + ix;
            const bool *bbp = overMat + iy * width + ix;
            over_out = bbp[0] && bbp[1] && bbp[width] && bbp[1+width];

            Eigen::Vector3f res = dxdy * *(const Eigen::Vector3f *) (bp+1+width)
                        + (dx - dxdy) * *(const Eigen::Vector3f *) (bp+width)
                        + (dy - dxdy) * *(const Eigen::Vector3f *) (bp+1)
                        + (1 - dx - dy + dxdy) * *(const Eigen::Vector3f *) (bp);
            
            return res;
        }

        // interpolation with "or" mask
        EIGEN_ALWAYS_INLINE Eigen::Vector3f
        getInterpolatedElement33OvenOr(const Eigen::Vector3f *const mat ,const bool *overMat,
                                         const float x, const float y, const int width, bool &over_out)
        {
            int ix = (int)x;
            int iy = (int)y;
            
            float dx = x - ix;
            float dy = y - iy;
            float dxdy = dx * dy;

            const Eigen::Vector3f *bp = mat + iy * width + ix;
            const bool *bbp = overMat + iy * width + ix;
            over_out = bbp[0] || bbp[1] || bbp[width] || bbp[1+width];

            Eigen::Vector3f res = dxdy * *(const Eigen::Vector3f *) (bp+1+width)
                        + (dx - dxdy) * *(const Eigen::Vector3f *) (bp+width)
                        + (dy - dxdy) * *(const Eigen::Vector3f *) (bp+1)
                        + (1 - dx - dy + dxdy) * *(const Eigen::Vector3f *) (bp);
            
            return res;
        }

        //
        EIGEN_ALWAYS_INLINE float
        getInterpolatedElement31(const Eigen::Vector3f *const mat ,
                                         const float x, const float y, const int width)
        {
            int ix = (int)x;
            int iy = (int)y;
            
            float dx = x - ix;
            float dy = y - iy;
            float dxdy = dx * dy;

            const Eigen::Vector3f *bp = mat + iy * width + ix;
            
            float res = dxdy * (*(const Eigen::Vector3f *) (bp+1+width))[0]
                                + (dx - dxdy) * (*(const Eigen::Vector3f *) (bp+width))[0]
                                + (dy - dxdy) * (*(const Eigen::Vector3f *) (bp+1))[0]
                                + (1 - dx - dy + dxdy) *(*(const Eigen::Vector3f *) (bp))[0];
            
            return res;
        }


        //BiLinear interpolation for Vector3f
        EIGEN_ALWAYS_INLINE Eigen::Vector3f
        getInterpolatedElement13BiLin(const float *const mat ,
                                         const float x, const float y, const int width)
        {
            int ix = (int)x;
            int iy = (int)y;
            
            const float *bp = mat + iy * width + ix;
            float tl = *(bp); // top left
            float tr = *(bp+1); // top right
            float bl = *(bp+width); // bottom left
            float br = *(bp+1+width); // bottom right

            float dx = x - ix;
            float dy = y - iy;

            float topInt = dx * tr + (1-dx) * tl; // top interpolation
            float botInt = dx * br + (1-dx) * bl; // bottom interpolation
            float leftInt = dy * bl + (1-dy) * tl; // left interpolation
            float rightInt = dy * br + (1-dy) * tr; // right interpolation
            
            Eigen::Vector3f res(dx* rightInt + (1-dx) * leftInt,
                                rightInt-leftInt,
                                botInt-topInt);
            return res;
        }


        //BiLinear interpolation for Vector3f
        EIGEN_ALWAYS_INLINE Eigen::Vector3f
        getInterpolatedElement33BiLin(const Eigen::Vector3f *const mat ,
                                         const float x, const float y, const int width)
        {
            int ix = (int)x;
            int iy = (int)y;
            
            const Eigen::Vector3f *bp = mat + iy * width + ix;

            float tl = (*(bp))[0]; // top left
            float tr = (*(bp+1))[0]; // top right
            float bl = (*(bp+width))[0]; // bottom left
            float br = (*(bp+1+width))[0]; // bottom right

            float dx = x - ix;
            float dy = y - iy;

            float topInt = dx * tr + (1-dx) * tl; // top interpolation
            float botInt = dx * br + (1-dx) * bl; // bottom interpolation
            float leftInt = dy * bl + (1-dy) * tl; // left interpolation
            float rightInt = dy * br + (1-dy) * tr; // right interpolation
            
            Eigen::Vector3f res(dx* rightInt + (1-dx) * leftInt,
                                rightInt-leftInt,
                                botInt-topInt);
            return res;
        }


        //cubic interpolation 
        EIGEN_ALWAYS_INLINE float
        getInterpolatedElement11Cub(const Eigen::Vector3f *const mat ,
                                         const float x, const float y, const int width)
        {
            // for x=0, this 
            int ix = (int)x;
            int iy = (int)y;
            
            const Eigen::Vector3f *bp = mat + iy * width + ix;

            float tl = (*(bp))[0]; // top left
            float tr = (*(bp+1))[0]; // top right
            float bl = (*(bp+width))[0]; // bottom left
            float br = (*(bp+1+width))[0]; // bottom right

            float dx = x - ix;
            float dy = y - iy;

            float topInt = dx * tr + (1-dx) * tl; // top interpolation
            float botInt = dx * br + (1-dx) * bl; // bottom interpolation
            float leftInt = dy * bl + (1-dy) * tl; // left interpolation
            float rightInt = dy * br + (1-dy) * tr; // right interpolation
            
            Eigen::Vector3f res(dx* rightInt + (1-dx) * leftInt,
                                rightInt-leftInt,
                                botInt-topInt);
            return res;
        }

    }
}






#endif  // LDSO_SLAM_GLOBAL_FUNCS_H_