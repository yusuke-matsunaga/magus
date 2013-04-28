#ifndef YM_UTILS_MULTISETPERMGEN_H
#define YM_UTILS_MULTISETPERMGEN_H

/// @file MultiSetPermGen.h
/// @brief MultiSetPermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetPermGen MultiSetPermGen.h "MultiSetPermGen.h"
/// @brief 重複を許した集合の組み合わせを作るクラス
//////////////////////////////////////////////////////////////////////
class MultiSetPermGen :
  public MultiSetGenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetPermGen(const vector<ymuint>& num_array,
		   ymuint k);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiSetPermGen(const MultiSetPermGen& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiSetPermGen&
  operator=(const MultiSetPermGen& src);

  /// @brief デストラクタ
  ~MultiSetPermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YM_UTILS_MULTISETPERMGEN_H
