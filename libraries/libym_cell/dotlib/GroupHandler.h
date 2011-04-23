#ifndef LIBYM_CELL_DOTLIB_GROUPHANDLER_H
#define LIBYM_CELL_DOTLIB_GROUPHANDLER_H

/// @file libym_cell/dotlib/GroupHandler.h
/// @brief GroupHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class GroupHandler DotlibHandler.h "DotlibHandler.h"
/// @brief group attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class GroupHandler :
  public DotlibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] parent 親のハンドラ
  GroupHandler(DotlibParser& parser,
	       GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~GroupHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_token 属性名を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(Token attr_token);

  /// @brief ハンドラの登録を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] handler 対応付けるハンドラ
  /// @note エラーが起きたら false を返す．
  bool
  reg_handler(const string& attr_name,
	      DotlibHandler* handler);

  /// @brief ハンドラを取り出す．
  /// @param[in] attr_name 属性名
  /// @note なければ NULL を返す．
  DotlibHandler*
  find_handler(const string& attr_name);


public:
  //////////////////////////////////////////////////////////////////////
  // 他のクラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する PtNode を返す．
  PtNode*
  pt_node();


protected:
  //////////////////////////////////////////////////////////////////////
  // 自身と継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する PtNode を設定する．
  void
  set_pt_node(PtNode* ndoe);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_token 属性名を表すトークン
  /// @param[in] value_list 値を表すトークンのリスト
  virtual
  bool
  begin_group(Token attr_token,
	      const vector<Token>& value_list);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハンドラの連想配列
  hash_map<string, DotlibHandler*> mHandlerMap;

  // 対応する PtNode
  PtNode* mPtNode;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DOTLIBHANDLER_H
