#ifndef LIBYM_MVN_VERILOG_SSAMGR_H
#define LIBYM_MVN_VERILOG_SSAMGR_H

/// @file libym_mvn/verilog/SsaMgr.h
/// @brief SsaMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"
#include "DeclHash.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class SsaMgr SsaMgr.h "SsaMgr.h"
/// @brief SSA のためのインスタンス番号生成を行うクラス
//////////////////////////////////////////////////////////////////////
class SsaMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] decl_hash VlDecl 用のハッシュ表
  SsaMgr(DeclHash& decl_hash);

  /// @brief デストラクタ
  ~SsaMgr();


public:

  /// @brief 登録する(単一要素の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] node 対応するノード
  void
  add(const VlDecl* decl,
      MvNode* node);

  /// @brief 登録する(配列の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] offset
  /// @param[in] node 対応するノード
  void
  add(const VlDecl* decl,
      ymuint offset,
      MvNode* node);

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には NULL を返す．
  MvNode*
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  MvNode*
  get(const VlDecl* decl,
      ymuint offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // VlDecl 用のハッシュ表
  DeclHash& mDeclHash;

  // VlDecl の ID をキーに MvNode の配列を格納する配列
  vector<vector<MvNode*> > mNodeArray;

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_SSAMGR_H
