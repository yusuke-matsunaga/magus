#ifndef LIBYM_CELL_DOTLIB_PTNODE_H
#define LIBYM_CELL_DOTLIB_PTNODE_H

/// @file libym_cell/dotlib/PtNode.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtNode PtNode.h "PtNode.h"
/// @brief パース木の構造を表すためのクラス
//////////////////////////////////////////////////////////////////////
class PtNode
{
  friend class PtMgr;

protected:

  /// @brief コンストラクタ
  PtNode();

  /// @brief デストラクタ
  virtual
  ~PtNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供を追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] node 追加する子供のノード
  /// @note デフォルトの実装はなにもしない．
  virtual
  void
  add_child(const ShString& attr_name,
	    PtNode* node);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  /// @note simple attribute なら常に1
  virtual
  ymuint
  value_num() const = 0;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  virtual
  const PtValue*
  value(ymuint pos) const = 0;

  /// @brief 子供の属性名の個数を返す．
  /// @note デフォルトの実装は 0 を返す．
  virtual
  ymuint
  child_attr_num() const;

  /// @brief 子供の属性名を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < child_attr_num() )
  virtual
  ShString
  child_attr_name(ymuint pos) const;

  /// @brief 属性に対応した子供の要素数を返す．
  /// @param[in] attr_name 子供の属性名
  /// @note デフォルトの実装は 0 を返す．
  virtual
  ymuint
  child_num(const ShString& attr_name) const;

  /// @brief 属性に対応した子供を返す．
  /// @param[in] attr_name 子供の属性名
  /// @param[in] pos 位置番号 ( 0 <= pos < child_num(attr_name) )
  virtual
  const PtNode*
  child(const ShString& attr_name,
	ymuint pos) const;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTNODE_H
