#ifndef YM_UTILS_MULTISETCOMBIGEN_H
#define YM_UTILS_MULTISETCOMBIGEN_H

/// @file MultiSetCombiGen.h
/// @brief MultiSetCombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetCombiGen MultiSetCombiGen.h "MultiSetCombiGen.h"
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

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
inline
MultiSetCombiGen::MultiSetCombiGen(const vector<ymuint>& num_array,
				   ymuint k) :
  MultiSetGenBase(num_array, k)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
MultiSetCombiGen::MultiSetCombiGen(const MultiSetCombiGen& src) :
  MultiSetGenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
inline
const MultiSetCombiGen&
MultiSetCombiGen::operator=(const MultiSetCombiGen& src)
{
  copy(src);
  return *this;
}

// @brief デストラクタ
inline
MultiSetCombiGen::~MultiSetCombiGen()
{
}

// @brief 末尾のチェック
// @return 末尾の時に true を返す．
inline
bool
MultiSetCombiGen::is_end() const
{
  return operator()(0) == group_num();
}

END_NAMESPACE_YM

#endif // YM_UTILS_MULTISETCOMBIGEN_H
