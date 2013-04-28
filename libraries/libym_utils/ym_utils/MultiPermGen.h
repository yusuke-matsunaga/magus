#ifndef YM_UTILS_MULTIPERMGEN_H
#define YM_UTILS_MULTIPERMGEN_H

/// @file ym_utils/MultiPermGen.h
/// @brief MultiPermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiPermGen MultiPermGen.h "ym_utils/MultiPermGen.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの順列を生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiPermGen :
  public MultiGenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiPermGen(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiPermGen(const MultiPermGen& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiPermGen&
  operator=(const MultiPermGen& src);

  /// @brief デストラクタ
  ~MultiPermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YM_UTILS_MULTIPERMGEN_H
