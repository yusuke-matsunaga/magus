#ifndef LIBYM_NETWORKS_VERILOG_ENV_H
#define LIBYM_NETWORKS_VERILOG_ENV_H

/// @file libym_networks/verilog/Env.h
/// @brief Env のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn.h"
#include "ym_verilog/vl/VlFwd.h"
#include "DeclHash.h"


BEGIN_NAMESPACE_YM_NETWORKS_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AssignInfo Env.h "Env.h"
/// @brief 代入情報を表すクラス
/// 具体的には代入の右辺と代入条件を表すノードの組
//////////////////////////////////////////////////////////////////////
struct AssignInfo
{
  /// @brief コンストラクタ
  /// @param[in] rhs 代入の右辺
  /// @param[in] cond 代入条件
  /// @param[in] block blocking 代入の時に true とするフラグ
  explicit
  AssignInfo(MvnNode* rhs = NULL,
	     MvnNode* cond = NULL,
	     bool block = false) :
    mRhs(rhs),
    mCond(cond),
    mBlock(block),
    mRefFlag(false) { }

  /// @brief 代入の右辺式を表すノード
  MvnNode* mRhs;

  /// @brief 代入条件を表すノード
  /// ただし常に代入する時は NULL
  MvnNode* mCond;

  /// @brief blocking 代入を表すフラグ
  bool mBlock;

  /// @brief 参照されたことを表すフラグ
  bool mRefFlag;

};


//////////////////////////////////////////////////////////////////////
/// @class Env Env.h "Env.h"
/// @brief const VlDecl と MvnNode の対応をとる連想配列
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
      MvnNode* node);

  /// @brief 登録する(配列の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] offset
  /// @param[in] node 対応するノード
  /// @param[in] block blocking 代入の時に true とするフラグ
  void
  add(const VlDeclArray* decl,
      ymuint offset,
      MvnNode* node);

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には NULL を返す．
  virtual
  MvnNode*
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  virtual
  MvnNode*
  get(const VlDeclArray* decl,
      ymuint offset) const;

  /// @brief ID番号に対応するノードを登録する．
  void
  add_by_id(ymuint id,
	    MvnNode* node);

  /// @brief ID番号に対応するノードを取り出す．
  MvnNode*
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

  // VlDecl の ID をキーに MvnNode の配列を格納する配列
  vector<MvnNode*> mNodeArray;

};


//////////////////////////////////////////////////////////////////////
/// @class ProcEnv Env.h "Env.h"
/// @brief プロセス内部の Env
//////////////////////////////////////////////////////////////////////
class ProcEnv :
  public Env
{
public:

  /// @brief コンストラクタ
  /// @param[in] global_env プロセスの外側の Env
  ProcEnv(const Env& global_env);

  /// @brief コピーコンストラクタ
  ProcEnv(const ProcEnv& tmp_env);

  /// @brief デストラクタ
  virtual
  ~ProcEnv();


public:

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief ID番号の最大値+1を返す．
  ymuint
  max_id() const;

  /// @brief 登録する(単一要素の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] node 代入式の右辺
  /// @param[in] block blocking 代入の時に true とするフラグ
  virtual
  void
  add(const VlDecl* decl,
      MvnNode* node,
      bool block);

  /// @brief 登録する(配列の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] offset
  /// @param[in] node 代入式の右辺
  /// @param[in] block blocking 代入の時に true とするフラグ
  virtual
  void
  add(const VlDeclArray* decl,
      ymuint offset,
      MvnNode* node,
      bool block);

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には NULL を返す．
  virtual
  MvnNode*
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  virtual
  MvnNode*
  get(const VlDeclArray* decl,
      ymuint offset) const;

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には NULL を返す．
  AssignInfo
  get_info(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  AssignInfo
  get_info(const VlDeclArray* decl,
      ymuint offset) const;

  /// @brief ID番号に対応するノードを登録する．
  /// @param[in] id ID番号
  /// @param[in] node 代入式の右辺
  /// @param[in] cond 代入条件
  /// @param[in] block blocking 代入の時に true とするフラグ
  void
  add_by_id(ymuint id,
	    MvnNode* node,
	    MvnNode* cond,
	    bool block);

  /// @brief ID番号に対応するノードを取り出す．
  AssignInfo
  get_from_id(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 外側の Env
  const Env& mGlobalEnv;

  // VlDecl の ID をキーに AssignInfo の配列を格納する配列
  vector<AssignInfo> mNodeArray;

};

END_NAMESPACE_YM_NETWORKS_VERILOG

#endif // LIBYM_NETWORKS_VERILOG_ENV_H
