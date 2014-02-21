
/// @file utils/CombiGen.sip
/// @brief CombiGen の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.

class CombiGen : public GenBase {

%TypeHeaderCode
#include "utils/CombiGen.h"
using namespace nsYm;
%End

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  CombiGen(unsigned int n,
	   unsigned int k);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  CombiGen(const CombiGen& src);

  /// @brief 次の要素を求める．
  void
  next();
  %MethodCode
  sipCpp->operator++();
  %End

};