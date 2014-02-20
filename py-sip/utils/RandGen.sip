
/// @file utils/RandGen.h
/// @brief RandGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

class RandGen {

%TypeHeaderCode
#include "utils/RandGen.h"
using namespace nsYm;
%End

public:

  /// @brief シードで初期化する．
  /// @param[in] s シード
  /// @note 以下，原文\n
  /// initializes mt[N] with a seed
  void
  init(unsigned int s);

  /// @brief キーの配列で初期化する．
  /// @param[in] init_key キーの配列
  /// @param[in] key_length init_key の要素数
  /// @note 以下，原文\n
  /// initialize by an array with array-length
  /// init_key is the array for initializing keys
  /// key_length is its length
  /// slight change for C++, 2004/2/26
  void
  init_by_array(SIP_PYTUPLE args);
  %MethodCode
  unsigned int n = PyList_GET_SIZE(a0);
  unsigned int* init_key = new unsigned int[n];
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GET_ITEM(a0, i);
    unsigned long val = 0;
    if ( !PyArg_ParseTuple(obj1, "k", &val) ) {
      return NULL;
    }
    init_key[i] = val;
  }

  sipCpp->init_by_array(init_key, n);
  %End

  /// @brief 32ビット符号無し整数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,0xffffffff]-interval
  unsigned int
  int32();

  /// @brief 31ビット整数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,0x7fffffff]-interval
  int
  int31();

  /// @brief unsigned long の整数の乱数を発生させる．
  unsigned long
  ulong();

  /// @brief [0,1]の区間の実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// These real versions are due to Isaku Wada, 2002/01/09 added
  /// generates a random number on [0,1]-real-interval
  double
  real1();

  /// @brief [0,1)の区間の実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,1)-real-interval
  double
  real2();

  /// @brief (0,1)の区間の実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on (0,1)-real-interval
  double
  real3();

  /// @brief [0,1)の区間の53ビットの実数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,1) with 53-bit resolution
  double
  res53();

};