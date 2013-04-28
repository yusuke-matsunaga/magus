#ifndef YM_UTILS_PERMGEN_H
#define YM_UTILS_PERMGEN_H

/// @file ym_utils/PermGen.h
/// @brief PermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PermGen PermGen.h "ym_utils/PermGen.h"
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

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
inline
PermGen::PermGen(ymuint n,
		 ymuint k) :
  GenBase(n, k)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
PermGen::PermGen(const PermGen& src) :
  GenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
inline
const PermGen&
PermGen::operator=(const PermGen& src)
{
  copy(src);
  return *this;
}

// デストラクタ
inline
PermGen::~PermGen()
{
}

// 末尾の時に true を返す．
inline
bool
PermGen::is_end() const
{
  return operator()(0) == n();
}

END_NAMESPACE_YM

#endif // YM_UTILS_PERMGEN_H
