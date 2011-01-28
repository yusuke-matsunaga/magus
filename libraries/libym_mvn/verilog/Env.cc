
/// @file libym_mvn/verilog/Env.cc
/// @brief Env の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Env.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

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
// @param[in] cond 条件
void
Env::add(const VlDecl* decl,
	 MvNode* node,
	 MvNode* cond)
{
  ymuint id = mDeclHash.get_id(decl);
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(AssignInfo());
  }
  AssignInfo& info = mNodeArray[id];
  info.mRhs = node;
  info.mCond = cond;
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 対応するノード
// @param[in] cond 条件
void
Env::add(const VlDeclArray* decl,
	 ymuint offset,
	 MvNode* node,
	 MvNode* cond)
{
  ymuint id = mDeclHash.get_id(decl, offset);
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(AssignInfo());
  }
  AssignInfo& info = mNodeArray[id];
  info.mRhs = node;
  info.mCond = cond;
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
AssignInfo
Env::get(const VlDecl* decl) const
{
  ymuint id = mDeclHash.get_id(decl);
  if ( mNodeArray.size() <= id ) {
    return AssignInfo(NULL, NULL);
  }
  return mNodeArray[id];
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
AssignInfo
Env::get(const VlDeclArray* decl,
	 ymuint offset) const
{
  ymuint id = mDeclHash.get_id(decl, offset);
  if ( mNodeArray.size() <= id ) {
    return AssignInfo(NULL, NULL);
  }
  return mNodeArray[id];
}

// @brief ID番号に対応するノードを登録する．
void
Env::add_by_id(ymuint id,
	       MvNode* node)
{
  mNodeArray[id][0] = node;
}

// @brief ID番号に対応するノードを取り出す．
MvNode*
Env::get_from_id(ymuint id) const
{
  return mNodeArray[id][0];
}

// @brief DeclHash を得る．
DeclHash&
Env::decl_hash() const
{
  return mDeclHash;
}


//////////////////////////////////////////////////////////////////////
// クラス TmpEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] global_env プロセスの外側の Env
TmpEnv::TmpEnv(const Env& global_env) :
  Env(global_env.decl_hash()),
  mGlobalEnv(global_env)
{
}

// @brief コピーコンストラクタ
TmpEnv::TmpEnv(const TmpEnv& tmp_env) :
  Env(tmp_env),
  mGlobalEnv(tmp_env.mGlobalEnv)
{
}

// @brief デストラクタ
TmpEnv::~TmpEnv()
{
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
AssignInfo
TmpEnv::get(const VlDecl* decl) const
{
  AssignInfo ans = Env::get(decl);
  if ( ans.mRhs == NULL ) {
    return mGlobalEnv.get(decl);
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
TmpEnv::get(const VlDeclArray* decl,
	    ymuint offset) const
{
  AssignInfo ans = Env::get(decl, offset);
  if ( ans.mRhs == NULL ) {
    return mGlobalEnv.get(decl, offset);
  }
  return ans;
}

END_NAMESPACE_YM_MVN_VERILOG
