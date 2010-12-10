
/// @file libym_mvn/verilog/SsaMgr.cc
/// @brief SsaMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SsaMgr.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

// @brief コンストラクタ
// @param[in] decl_hash VlDecl 用のハッシュ表
SsaMgr::SsaMgr(DeclHash& decl_hash) :
  mDeclHash(decl_hash),
  mNodeArray(decl_hash.max_id())
{
}

// @brief デストラクタ
SsaMgr::~SsaMgr()
{
}

// @brief 登録する(単一要素の場合)
// @param[in] decl 宣言要素
// @param[in] node 対応するノード
void
SsaMgr::add(const VlDecl* decl,
	    MvNode* node)
{
  ymuint id = mDeclHash.get_id(decl);
  mNodeArray[id][0] = node;
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 対応するノード
void
SsaMgr::add(const VlDecl* decl,
	    ymuint offset,
	    MvNode* node)
{
  ymuint id = mDeclHash.get_id(decl);
  if ( mNodeArray[id].size() == 0 ) {
    ymuint n = 1;
    ymuint nd = decl->dimension();
    for (ymuint i = 0; i < nd; ++ i) {
      const VlRange* range = decl->range(i);
      n *= range->size();
    }
    mNodeArray[id].resize(n);
  }
  mNodeArray[id][offset] = node;
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
MvNode*
SsaMgr::get(const VlDecl* decl) const
{
  ymuint id = mDeclHash.get_id(decl);
  return mNodeArray[id][0];
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
MvNode*
SsaMgr::get(const VlDecl* decl,
	    ymuint offset) const
{
  ymuint id = mDeclHash.get_id(decl);
  return mNodeArray[id][offset];
}

END_NAMESPACE_YM_MVN_VERILOG
