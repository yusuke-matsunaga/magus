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
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  bool
  add_attr(const ShString& attr_name,
	   PtValue* value);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute を取得する．
  /// @param[in] attr_name 属性名
  /// @note 該当する属性がなければ NULL を返す．
  const PtValue*
  get_attr(const ShString& attr_name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名をキーにして attribute を格納するハッシュ表
  // おなじ属性の値が複数あったら PtValue->next() でつなげる．
  hash_map<ShString, PtValue*> mAttrMap;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTNODE_H
