
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
  init(ymuint32 s);

  /// @brief キーの配列で初期化する．
  /// @param[in] init_key キーの配列
  /// @param[in] key_length init_key の要素数
  /// @note 以下，原文\n
  /// initialize by an array with array-length
  /// init_key is the array for initializing keys
  /// key_length is its length
  /// slight change for C++, 2004/2/26
  void
  init_by_array(ymuint32 init_key[],
		ymuint key_length);

  /// @brief 32ビット符号無し整数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,0xffffffff]-interval
  ymuint32
  int32();

  /// @brief 31ビット整数の乱数を発生させる．
  /// @note 以下，原文\n
  /// generates a random number on [0,0x7fffffff]-interval
  ymint32
  int31();

  /// @brief unsigned long の整数の乱数を発生させる．
  ymulong
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