#ifndef YM_UTILS_COMBIGEN_H
#define YM_UTILS_COMBIGEN_H

/// @file ym_utils/CombiGen.h
/// @brief CombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CombiGen CombiGen.h "ym_utils/CombiGen.h"
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
CombiGen::CombiGen(ymuint n,
		   ymuint k) :
  GenBase(n, k)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
CombiGen::CombiGen(const CombiGen& src) :
  GenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
inline
const CombiGen&
CombiGen::operator=(const CombiGen& src)
{
  copy(src);
  return *this;
}

// デストラクタ
inline
CombiGen::~CombiGen()
{
}

// 末尾の時に true を返す．
inline
bool
CombiGen::is_end() const
{
  // 末尾の時には範囲外の値(= n())を持っている．
  return operator()(0) == n();
}

END_NAMESPACE_YM

#endif // YM_UTILS_COMBIGEN_H
