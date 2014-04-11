
/// @file RandHashGen.sip
/// @brief RandHashGen の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class RandHashGen
{
%TypeHeaderCode
#include "RandHashGen.h"
using nsYm::nsIgf::RandHashGen;
%End

  /// @brief ランダムにハッシュ関数を作る．
  /// @param[in] input_num 入力数
  /// @param[in] output_num 出力数
  /// @param[in] max_degree
  InputFunc*
  gen_func(unsigned int input_num,
	   unsigned int output_num,
	   unsigned int max_degree);

};