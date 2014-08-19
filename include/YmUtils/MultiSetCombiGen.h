#ifndef YMUTILS_MULTISETCOMBIGEN_H
#define YMUTILS_MULTISETCOMBIGEN_H

/// @file YmUtils/MultiSetCombiGen.h
/// @brief MultiSetCombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetCombiGen MultiSetCombiGen.h "YmUtils/MultiSetCombiGen.h"
/// @brief 重複を許した集合の順列を作るクラス
//////////////////////////////////////////////////////////////////////
class MultiSetCombiGen :
  public MultiSetGenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetCombiGen(const vector<ymuint>& num_array,
		   ymuint k);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiSetCombiGen(const MultiSetCombiGen& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiSetCombiGen&
  operator=(const MultiSetCombiGen& src);

  /// @brief デストラクタ
  ~MultiSetCombiGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_MULTISETCOMBIGEN_H
