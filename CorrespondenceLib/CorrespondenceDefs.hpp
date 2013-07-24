#ifndef CORRESPONDENCEDEFS_HPP
#define CORRESPONDENCEDEGS_HPP

#include <stdint.h>
#include <vector>
#include <memory>

namespace correspondence
{
  typedef uint8_t byte;

  struct Image
  {
    int rows, cols, stride, pxStep;
    byte* data;

    Image(const int _rows, const int _cols, const int _pxStep)
      : rows(_rows),
        cols(_cols),
        pxStep(_pxStep),
        stride((_cols & 16) + 16)
    {
      data = (byte*)_mm_malloc(stride * rows * pxStep, 16);
      zeroMem();
    }

    //Copies the supplied data into this object's aligned memory
    Image(const int _rows, const int _cols, const int _pxStep, const byte* _data,
      const int _dataLen)
      : rows(_rows),
        cols(_cols),
        pxStep(_pxStep),
        stride(_cols + (_cols & 16) + 16)
    {
      data = (byte*)_mm_malloc(stride * rows * pxStep, 16);
      byte* dataPtr = data;
      int offset = 0;
      for(int i = 0; i < rows; ++i)
      {
        memcpy(dataPtr, _data + offset, _cols);
        offset += _cols * sizeof(byte);
        dataPtr += stride;
      }
    }

    void zeroMem()
    {
      //TODO check if SSE;

      __m128i* mem = (__m128i*)data;
      for(int i = 0; i < rows*stride*pxStep; i += 16)
      {
        *mem = _mm_setzero_si128();
        ++mem;;
      }
      
    }

    inline byte* at(const int _row, const int _col) const
    {
      //Note: only handles a positive value for stride
      return data + _row * stride + _col;
    };
  };

  struct Feature
  {
    int x, y, idx;
  };

  struct Match
  {
    Feature feature1, feature2;
    int dist;
  };

  struct FeatureList
  {
    std::vector<Feature> allFeatures;
    std::vector<Feature> nonmaxFeatures;

    //Element for each row of the image, value indicates the first element of that row 
    std::vector<int> rowIdxs;
  };

  enum eSamplingWindow
  {
    SPARSE_8,//8, 12 or 16 sampling points
    SPARSE_12,
    SPARSE_16,
    DENSE_3,//various sizes of dense sampling windows
    DENSE_5,
    DENSE_7,
    DENSE_9,
    DENSE_11,
    DENSE_13
  };

  enum eMatchMode
  {
    FLOW,
    STEREO
  };

  struct MatchingParams
  {
    int filterDist;
    int maxDisparity;
    int epipolarRange;
  };

  struct CensusCfg
  {
    //Use what sampling pattern
    eSamplingWindow type;
    //Dimension of the square sampling pattern.  e.g. the SPARSE_16 pattern has an equivalent window of a 9x9
    int windowSize;
    //The LUT of the sampling pattern
    std::vector<int> pattern;
    //Dense or Sparse
    eMatchMode matchMode;
    //sampling pattern of the correlation window
    eSamplingWindow corrType;
    //The lookup table of the correlation window
    std::vector<int> corrPattern;
  };

  struct Descriptors
  {
    Image tfmdIm;
    eSamplingWindow type;
  };
};//namespace correspondence
#endif
