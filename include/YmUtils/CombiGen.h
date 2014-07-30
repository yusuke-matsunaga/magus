#ifndef YMUTILS_COMBIGEN_H
#define YMUTILS_COMBIGEN_H

/// @file YmUtils/CombiGen.h
/// @brief CombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CombiGen CombiGen.h "YmUtils/CombiGen.h"
/// @ingroup GeneratorGroup
/// @brief 組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class CombiGen :
  public GenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  CombiGen(ymuint n,
	   ymuint k);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  CombiGen(const CombiGen& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const CombiGen&
  operator=(const CombiGen& src);

  /// @brief デストラクタ
  ~CombiGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_COMBIGEN_H
