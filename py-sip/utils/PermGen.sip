
/// @file utils/PermGen.h
/// @brief PermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.

class PermGen : public GenBase {

%TypeHeaderCode
#include "utils/PermGen.h"
using namespace nsYm;
%End

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選択する要素数
  PermGen(unsigned int n,
	  unsigned int k);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  PermGen(const PermGen& src);

  /// @brief 次の要素を求める．
  void
  next();
  %MethodCode
  sipCpp->operator++();
  %End

};