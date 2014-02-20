#ifndef UTILS_PERMGEN_H
#define UTILS_PERMGEN_H

/// @file utils/PermGen.h
/// @brief PermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PermGen PermGen.h "utils/PermGen.h"
/// @ingroup GeneratorGroup
/// @brief 順列生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class PermGen :
  public GenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選択する要素数
  PermGen(ymuint n,
	  ymuint k);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  PermGen(const PermGen& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const PermGen&
  operator=(const PermGen& src);

  /// @brief デストラクタ
  ~PermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // UTILS_PERMGEN_H
