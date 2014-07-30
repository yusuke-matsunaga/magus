#ifndef YMUTILS_MULTICOMBIGEN_H
#define YMUTILS_MULTICOMBIGEN_H

/// @file YmUtils/MultiCombiGen.h
/// @brief MultiCombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiCombiGen MultiCombiGen.h "YmUtils/MultiCombiGen.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの組み合わせを生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiCombiGen :
  public MultiGenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiCombiGen(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiCombiGen(const MultiCombiGen& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiCombiGen&
  operator=(const MultiCombiGen& src);

  /// @brief デストラクタ
  ~MultiCombiGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_MULTICOMBIGEN_H
