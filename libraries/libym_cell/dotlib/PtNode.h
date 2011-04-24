#ifndef LIBYM_CELL_DOTLIB_PTNODE_H
#define LIBYM_CELL_DOTLIB_PTNODE_H

/// @file libym_cell/dotlib/PtNode.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "Token.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtNode PtNode.h "PtNode.h"
/// @brief パース木の構造を表すためのクラス
//////////////////////////////////////////////////////////////////////
class PtNode
{
public:

  /// @brief simple attribute 用のコンストラクタ
  /// @param[in] attr_name 属性名を表すトークン
  /// @param[in] value 値を表すトークン
  PtNode(Token attr_name,
	 Token value);

  /// @brief complex attribute/group statement 用のコンストラクタ
  /// @param[in] attr_name 属性名を表すトークン
  /// @param[in] value 値を表すトークンのリスト
  PtNode(Token attr_name,
	 const vector<Token>& value_list);

  /// @brief デストラクタ
  ~PtNode();

  /// @brief 子供を追加する．
  /// @param[in] node 追加する子供のノード
  void
  add_child(PtNode* node);

  /// @brief 値を追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  virtual
  void
  add_value(const string& attr_name,
	    const FileRegion& attr_loc,
	    const PtValue* value);

  /// @brief 値のリストを追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値のリスト
  virtual
  void
  add_value(const string& attr_name,
	    const FileRegion& attr_loc,
	    const vector<const PtValue*>& value_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性名を表すトークンを返す．
  Token
  attr_name() const;

  /// @brief 値の数を返す．
  /// @note simple attribute なら常に1
  ymuint
  value_num() const;

  /// @brief 値を表すトークンを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  Token
  value(ymuint pos) const;

  /// @brief 子供のノードの要素数を返す．
  ymuint
  child_num() const;

  /// @brief 子供のノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
  const PtNode*
  child(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名
  Token mAttrName;

  // 値のリスト
  vector<Token> mValueList;

  // 子供のノードのリスト
  vector<PtNode*> mChildList;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTNODE_H
