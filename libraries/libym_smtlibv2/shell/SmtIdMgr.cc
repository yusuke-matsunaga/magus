
/// @file SmtIdMgr.cc
/// @brief SmtIdMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtIdMgr.h"
#include "ym_smtlibv2/SmtId.h"
#include "SmtLibNode.h"
#include "SmtIndexId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtIdMgr
//
// よくあるハッシュ表の実装だが，面倒くさいのはハッシュ値の計算方法
// SmtId になる前は SmtLibNode のリストの形でインデックスが与えられる．
// これに対してハッシュ表を拡大する時の再ハッシュ時にはインデックスは
// 配列に入っている．
// 結局，名前のみ，名前＋SmtLibNodeのリスト，名前＋インデックスの配列
// の3種類の計算方法がある．
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtIdMgr::SmtIdMgr() :
  mAlloc(4096)
{
  mNum = 0;
  mTableSize = 0;
  mHashTable = NULL;

  expand_table(1024);
}

// @brief デストラクタ
SmtIdMgr::~SmtIdMgr()
{
  delete [] mHashTable;
}


BEGIN_NONAMESPACE

// インデックスが等しいかチェックする．
// 等しければ true を返す．
bool
check_index(SmtId* id,
	    const SmtLibNode* node)
{
  // サイズが等しいことはチェック済み
  ymuint index_size = id->index_size();
  for (ymuint i = 0; i < index_size; ++ i) {
    if ( id->index(i) != node->int_value() ) {
      return false;
    }
    node = node->sibling();
  }
  return true;
}

END_NONAMESPACE

// @brief 識別子に変換する．
// @param[in] name 名前
// @param[in] index_size インデックスの数．
// @param[in] index_node インデックスリストの先頭のノード
// @note index_node は正しい形をしていると仮定している．
const SmtId*
SmtIdMgr::new_id(const ShString& name,
		 ymuint index_size,
		 const SmtLibNode* index_node)
{
  ymuint h = hash(name, index_node);
  ymuint idx = h % mTableSize;
  for (SmtId* id = mHashTable[idx]; id != NULL; id = id->mLink) {
    if ( id->name() == name && id->index_size() == index_size &&
	 check_index(id, index_node) ) {
      return id;
    }
  }

  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
    idx = h % mTableSize;
  }

  SmtId* id = NULL;
  if ( index_node == NULL ) {
    void* p = mAlloc.get_memory(sizeof(SmtId));
    id = new (p) SmtId(name);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtIndexId) + sizeof(ymint32) * (index_size - 1));
    SmtIndexId* id1 = new (p) SmtIndexId(name);
    id1->mIndexNum = index_size;
    const SmtLibNode* node = index_node;
    for (ymuint i = 0; i < index_size; ++ i) {
      id1->mIndexList[i] = node->int_value();
    }
  }

  id->mId = mNum;
  ++ mNum;

  id->mLink = mHashTable[idx];
  mHashTable[idx] = id;

  return id;
}

// @brief ハッシュ表を拡大する．
// @param[in] req_size 新しいサイズ
void
SmtIdMgr::expand_table(ymuint req_size)
{
  ymuint old_size = mTableSize;
  SmtId** old_table = mHashTable;

  mTableSize = req_size;
  mHashTable = new SmtId*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (SmtId* id = old_table[i]; id != NULL; ) {
	SmtId* tmp_id = id;
	id = id->mLink;

	ymuint h = hash(tmp_id);
	tmp_id->mLink = mHashTable[h];
	mHashTable[h] = tmp_id;
      }
    }
    delete [] old_table;
  }
}

// @brief ハッシュ値を計算する．
// @param[in] name 名前
// @param[in] index_node インデックスリストの先頭のノード
ymuint
SmtIdMgr::hash(const ShString& name,
	       const SmtLibNode* index_node)
{
  ymuint h = name.hash();
  for (; index_node != NULL; index_node = index_node->sibling()) {
    h = h * 127 + index_node->int_value();
  }
  return h;
}

// @brief ハッシュ値を計算する．
// @param[in] id 識別子のインスタンス
ymuint
SmtIdMgr::hash(const SmtId* id)
{
  ymuint h = id->name().hash();
  for (ymuint i = 0; i < id->index_size(); ++ i) {
    h = h * 127 + id->index(i);
  }
  return h;
}

END_NAMESPACE_YM_SMTLIBV2
