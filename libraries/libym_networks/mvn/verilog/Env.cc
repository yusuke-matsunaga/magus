
/// @file libym_networks/verilog/Env.cc
/// @brief Env の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Env.h"
#include "AsyncControl.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_NETWORKS_VERILOG

// @brief コンストラクタ
// @param[in] decl_hash VlDecl 用のハッシュ表
Env::Env(DeclHash& decl_hash) :
  mDeclHash(decl_hash),
  mNodeArray(decl_hash.max_id())
{
}

// @brief コピーコンストラクタ
Env::Env(const Env& src) :
  mDeclHash(src.mDeclHash),
  mNodeArray(src.mNodeArray)
{
}

// @brief デストラクタ
Env::~Env()
{
}

// @brief 内容をクリアする．
void
Env::clear()
{
  mNodeArray.clear();
  mNodeArray.resize(mDeclHash.max_id());
}

// @brief ID番号の最大値+1を返す．
ymuint
Env::max_id() const
{
  return mDeclHash.max_id();
}

// @brief 登録する(単一要素の場合)
// @param[in] decl 宣言要素
// @param[in] node 対応するノード
void
Env::add(const VlDecl* decl,
	 MvnNode* node)
{
  ymuint id = mDeclHash.get_id(decl);
  add_by_id(id, node);
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 対応するノード
void
Env::add(const VlDeclArray* decl,
	 ymuint offset,
	 MvnNode* node)
{
  ymuint id = mDeclHash.get_id(decl, offset);
  add_by_id(id, node);
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
MvnNode*
Env::get(const VlDecl* decl) const
{
  ymuint id = mDeclHash.get_id(decl);
  return get_from_id(id);
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
MvnNode*
Env::get(const VlDeclArray* decl,
	 ymuint offset) const
{
  ymuint id = mDeclHash.get_id(decl, offset);
  return get_from_id(id);
}

// @brief ID番号に対応するノードを登録する．
void
Env::add_by_id(ymuint id,
	       MvnNode* node)
{
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(NULL);
  }
  mNodeArray[id] = node;
}

// @brief ID番号に対応するノードを取り出す．
MvnNode*
Env::get_from_id(ymuint id) const
{
  if ( mNodeArray.size() <= id ) {
    return NULL;
  }
  return mNodeArray[id];
}

// @brief DeclHash を得る．
DeclHash&
Env::decl_hash() const
{
  return mDeclHash;
}


//////////////////////////////////////////////////////////////////////
// クラス ProcEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] global_env プロセスの外側の Env
ProcEnv::ProcEnv(const Env& global_env) :
  Env(global_env.decl_hash()),
  mGlobalEnv(global_env),
  mNodeArray(global_env.max_id())
{
}

// @brief コピーコンストラクタ
ProcEnv::ProcEnv(const ProcEnv& src) :
  Env(src.decl_hash()),
  mGlobalEnv(src.mGlobalEnv),
  mNodeArray(src.mNodeArray)
{
}

// @brief デストラクタ
ProcEnv::~ProcEnv()
{
}

// @brief 内容をクリアする．
void
ProcEnv::clear()
{
  mNodeArray.clear();
  mNodeArray.resize(max_id());
}

// @brief ID番号の最大値+1を返す．
ymuint
ProcEnv::max_id() const
{
  return decl_hash().max_id();
}

// @brief 登録する(単一要素の場合)
// @param[in] decl 宣言要素
// @param[in] node 代入式の右辺
// @param[in] block blocking 代入の時に true とするフラグ
void
ProcEnv::add(const VlDecl* decl,
	     MvnNode* node,
	     bool block)
{
  ymuint id = decl_hash().get_id(decl);
  add_by_id(id, node, NULL, block);
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 代入式の右辺
// @param[in] block blocking 代入の時に true とするフラグ
void
ProcEnv::add(const VlDeclArray* decl,
	     ymuint offset,
	     MvnNode* node,
	     bool block)
{
  ymuint id = decl_hash().get_id(decl, offset);
  add_by_id(id, node, NULL, block);
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
MvnNode*
ProcEnv::get(const VlDecl* decl) const
{
  ymuint id = decl_hash().get_id(decl);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs != NULL &&
       ans.mBlock == true &&
       ans.mCond == NULL ) {
    return ans.mRhs;
  }
  return mGlobalEnv.get_from_id(id);
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
MvnNode*
ProcEnv::get(const VlDeclArray* decl,
	     ymuint offset) const
{
  ymuint id = decl_hash().get_id(decl, offset);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs != NULL &&
       ans.mBlock == true &&
       ans.mCond == NULL ) {
    return ans.mRhs;
  }
  return mGlobalEnv.get_from_id(id);
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
AssignInfo
ProcEnv::get_info(const VlDecl* decl) const
{
  ymuint id = decl_hash().get_id(decl);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs == NULL ) {
    MvnNode* node = mGlobalEnv.get(decl);
    return AssignInfo(node);
  }
  return ans;
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
AssignInfo
ProcEnv::get_info(const VlDeclArray* decl,
		  ymuint offset) const
{
  ymuint id = decl_hash().get_id(decl, offset);
  AssignInfo ans = get_from_id(id);
  if ( ans.mRhs == NULL ) {
    MvnNode* node = mGlobalEnv.get(decl, offset);
    return AssignInfo(node);
  }
  return ans;
}

// @brief ID番号に対応するノードを登録する．
// @param[in] id ID番号
// @param[in] node 代入式の右辺
// @param[in] cond 代入条件
// @param[in] block blocking 代入の時に true とするフラグ
void
ProcEnv::add_by_id(ymuint id,
		   MvnNode* node,
		   MvnNode* cond,
		   bool block)
{
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(AssignInfo());
  }
  mNodeArray[id] = AssignInfo(node, cond, block);
}

// @brief ID番号に対応するノードを取り出す．
AssignInfo
ProcEnv::get_from_id(ymuint id) const
{
  if ( mNodeArray.size() <= id ) {
    return AssignInfo();
  }
  return mNodeArray[id];
}

// @brief コンストラクタ
// @param[in] global_env プロセスの外側の Env
AsyncControl::AsyncControl(const Env& global_env) :
  mNode(NULL),
  mPol(0),
  mEnv(global_env)
{
}

END_NAMESPACE_YM_NETWORKS_VERILOG
