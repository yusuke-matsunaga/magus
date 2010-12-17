
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
void
Env::add(const VlDecl* decl,
	 MvNode* node)
{
  ymuint id = mDeclHash.get_id(decl);
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(vector<MvNode*>(1));
  }
  vector<MvNode*>& tmp = mNodeArray[id];
  if ( tmp.size() != 1 ) {
    tmp.resize(1);
  }
  tmp[0] = node;
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 対応するノード
void
Env::add(const VlDecl* decl,
	 ymuint offset,
	 MvNode* node)
{
  ymuint id = mDeclHash.get_id(decl);
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(vector<MvNode*>(1));
  }
  vector<MvNode*>& tmp = mNodeArray[id];
  ymuint n = 1;
  ymuint nd = decl->dimension();
  for (ymuint i = 0; i < nd; ++ i) {
    const VlRange* range = decl->range(i);
    n *= range->size();
  }
  if ( tmp.size() == n ) {
    tmp.resize(n);
  }
  tmp[offset] = node;
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
MvNode*
Env::get(const VlDecl* decl) const
{
  ymuint id = mDeclHash.get_id(decl);
  if ( mNodeArray.size() <= id ) {
    return NULL;
  }
  return mNodeArray[id][0];
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
MvNode*
Env::get(const VlDecl* decl,
	 ymuint offset) const
{
  ymuint id = mDeclHash.get_id(decl);
  if ( mNodeArray.size() <= id ) {
    return NULL;
  }
  const vector<MvNode*>& tmp = mNodeArray[id];
  if ( tmp.size() <= offset ) {
    return NULL;
  }
  return tmp[offset];
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

END_NAMESPACE_YM_MVN_VERILOG
