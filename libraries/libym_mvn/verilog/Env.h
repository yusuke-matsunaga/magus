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
/// @class AssignInfo Env.h "Env.h"
/// @brief 代入情報を表すクラス
/// 具体的には代入の右辺と代入条件を表すノードの組
//////////////////////////////////////////////////////////////////////
struct AssignInfo
{
  /// @brief コンストラクタ
  /// @param[in] rhs 代入の右辺
  /// @param[in] cond 代入条件
  explicit
  AssignInfo(MvNode* rhs = NULL,
	     MvNode* cond = NULL);

  /// @brief 代入の右辺式を表すノード
  MvNode* mRhs;

  /// @brief 代入条件を表すノード
  /// ただし常に代入する時は NULL
  MvNode* mCond;

};

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
  /// @param[in] cond 条件
  void
  add(const VlDecl* decl,
      MvNode* node,
      MvNode* cond = NULL);

  /// @brief 登録する(配列の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] offset
  /// @param[in] node 対応するノード
  /// @param[in] cond 条件
  void
  add(const VlDeclArray* decl,
      ymuint offset,
      MvNode* node,
      MvNode* cond = NULL);

  /// @brief マージする．
  void
  merge(const Env& then_env,
	const Env& else_env,
	MvMgr* mgr,
	MvModule* parent_module,
	MvNode* cond);

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には NULL を返す．
  virtual
  AssignInfo
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  virtual
  AssignInfo
  get(const VlDeclArray* decl,
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
  vector<AssignInfo> mNodeArray;

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
  AssignInfo
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  virtual
  AssignInfo
  get(const VlDeclArray* decl,
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
