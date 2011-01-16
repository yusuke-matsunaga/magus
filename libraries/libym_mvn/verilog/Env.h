#ifndef LIBYM_MVN_VERILOG_ENV_H
#define LIBYM_MVN_VERILOG_ENV_H

/// @file libym_mvn/verilog/Env.h
/// @brief Env のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"
#include "DeclHash.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class Env Env.h "Env.h"
/// @brief const VlDecl と MvNode の対応をとる連想配列
//////////////////////////////////////////////////////////////////////
class Env
{
public:

  /// @brief コンストラクタ
  /// @param[in] decl_hash 宣言要素のハッシュ表
  Env(DeclHash& decl_hash);

  /// @brief コピーコンストラクタ
  /// @param[in] src もとのオブジェクト
  Env(const Env& src);

  /// @brief デストラクタ
  virtual
  ~Env();


public:

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief ID番号の最大値+1を返す．
  ymuint
  max_id() const;

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
  virtual
  MvNode*
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  virtual
  MvNode*
  get(const VlDecl* decl,
      ymuint offset) const;

  /// @brief ID番号に対応するノードを登録する．
  void
  add_by_id(ymuint id,
	    MvNode* node);

  /// @brief ID番号に対応するノードを取り出す．
  MvNode*
  get_from_id(ymuint id) const;

  /// @brief DeclHash を得る．
  DeclHash&
  decl_hash() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // VlDecl 用のハッシュ表
  DeclHash& mDeclHash;

  // VlDecl の ID をキーに MvNode の配列を格納する配列
  vector<vector<MvNode*> > mNodeArray;

};


//////////////////////////////////////////////////////////////////////
/// @class TmpEnv Env.h "Env.h"
/// @brief プロセス内部の Env
//////////////////////////////////////////////////////////////////////
class TmpEnv :
  public Env
{
public:

  /// @brief コンストラクタ
  /// @param[in] global_env プロセスの外側の Env
  TmpEnv(const Env& global_env);

  /// @brief コピーコンストラクタ
  TmpEnv(const TmpEnv& tmp_env);

  /// @brief デストラクタ
  virtual
  ~TmpEnv();

public:

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には NULL を返す．
  virtual
  MvNode*
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  virtual
  MvNode*
  get(const VlDecl* decl,
      ymuint offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 外側の Env
  const Env& mGlobalEnv;

};


END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_ENV_H
