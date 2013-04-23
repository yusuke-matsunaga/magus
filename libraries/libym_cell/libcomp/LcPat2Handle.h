#ifndef LCPAT2HANDLE_H
#define LCPAT2HANDLE_H

/// @file LcPat2Handle.h
/// @brief LcPat2Handle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcPat2Handle LcPat2Handle.h "LcPat2Handle.h"
/// @brief LcPat2Node と極性を表すクラス
//////////////////////////////////////////////////////////////////////
class LcPat2Handle
{
public:

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  explicit
  LcPat2Handle(LcPat2Node* node = NULL,
	       bool inv = false);

  /// @brief デストラクタ
  ~LcPat2Handle();


public:

  /// @brief ノードを取り出す．
  LcPat2Node*
  node() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 値を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  void
  set(LcPat2Node* node,
      bool inv);

  /// @brief 等価比較演算子
  bool
  operator==(const LcPat2Handle& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタと反転属性を合わせたもの
  ympuint mData;

};

/// @brief 非等価比較演算子
bool
operator!=(const LcPat2Handle& left,
	   const LcPat2Handle& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] inv 反転属性
inline
LcPat2Handle::LcPat2Handle(LcPat2Node* node,
			   bool inv)
{
  set(node, inv);
}

// @brief デストラクタ
inline
LcPat2Handle::~LcPat2Handle()
{
}

// @brief ノードを取り出す．
inline
LcPat2Node*
LcPat2Handle::node() const
{
  return reinterpret_cast<LcPat2Node*>(mData & ~1UL);
}

// @brief 反転属性を取り出す．
inline
bool
LcPat2Handle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief 値を設定する．
// @param[in] node ノード
// @param[in] inv 反転属性
inline
void
LcPat2Handle::set(LcPat2Node* node,
		  bool inv)
{
  // bool に対する & 1UL は不必要だが念のため．
  mData = reinterpret_cast<ympuint>(node) | (static_cast<ympuint>(inv) & 1UL);
}

// @brief 等価比較演算子
inline
bool
LcPat2Handle::operator==(const LcPat2Handle& right) const
{
  return mData == right.mData;
}

// @brief 非等価比較演算子
inline
bool
operator!=(const LcPat2Handle& left,
	   const LcPat2Handle& right)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCPAT2HANDLE_H
