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
/// @brief パース木の構造を表すための基底クラス
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
  // 内容を設定する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief simple attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_simple_attr(const ShString& attr_name,
		  const PtValue* value);

  /// @brief complex attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_complex_attr(const ShString& attr_name,
		   const vector<const PtValue*>& value_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名をキーにして simple attribute を格納するハッシュ表
  // おなじ属性の値が複数あったら PtValue->next() でつなげる．
  hash_map<ShString, PtValue*> mSimpleAttrMap;
};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTNODE_H
