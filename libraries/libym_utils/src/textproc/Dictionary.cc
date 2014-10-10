
/// @file Dictionary.cc
/// @brief Dictionary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/Dictionary.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス DictBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DictBase::DictBase()
{
  mHashSize = 0;
  mHashTable = NULL;
  mNum = 0;
  alloc_table(1024);
}

// @brief デストラクタ
DictBase::~DictBase()
{
  clear();
  delete [] mHashTable;
}

// @brief 内容をクリアする．
void
DictBase::clear()
{
  for (ymuint i = 0; i < mHashSize; ++ i) {
    for (_Cell* cell = mHashTable[i]; cell != NULL; ) {
      _Cell* tmp = cell;
      cell = tmp->mLink;

      delete tmp;
    }
    mHashTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 要素数を返す．
ymuint
DictBase::num() const
{
  return mNum;
}

// @brief 登録されている名前のリストを返す．
// @param[out] name_list 名前のリストを格納するリスト
void
DictBase::name_list(vector<string>& name_list) const
{
  name_list.clear();
  name_list.reserve(num());
  for (ymuint i = 0; i < mHashSize; ++ i) {
    for (_Cell* cell = mHashTable[i]; cell != NULL;
	 cell = cell->mLink) {
      name_list.push_back(cell->mName);
    }
  }
  sort(name_list.begin(), name_list.end());
}

BEGIN_NONAMESPACE

// @brief 名前からハッシュ値を返す．
// @param[in] name 名前
ymuint32
hash_func(const string& name)
{
  ymuint32 h = 0;
  for (ymuint i = 0; i < name.size(); ++ i) {
    h = h * 33 + name[i];
  }
  return h;
}

END_NONAMESPACE

// @brief 名前の一致する要素を返す．
// @param[in] name 名前
// @return name と一致する要素を返す．
//
// 見つからなければ NULL を返す．
DictBase::_Cell*
DictBase::find_cell(const string& name) const
{
  ymuint32 h = hash_func(name) % mHashSize;
  for (_Cell* cell = mHashTable[h]; cell != NULL;
       cell = cell->mLink) {
    if ( cell->mName == name ) {
      return cell;
    }
  }
  return NULL;
}

// @brief 要素を登録する．
// @param[in] cell 登録する要素
//
// 同じ名前の要素がすでに登録されていたら動作は不定
void
DictBase::reg_cell(_Cell* cell)
{
  if ( mNum >= mNextLimit ) {
    alloc_table(mHashSize * 2);
  }
  _reg_cell(cell);
}

// @brief 登録されているセルのリストを得る．
// @param[out] cell_list 結果を格納するリスト
void
DictBase::cell_list(vector<_Cell*>& cell_list) const
{
  cell_list.clear();
  cell_list.reserve(num());
  for (ymuint i = 0; i < mHashSize; ++ i) {
    for (_Cell* cell = mHashTable[i]; cell != NULL;
	 cell = cell->mLink) {
      cell_list.push_back(cell);
    }
  }
}

// @brief ハッシュ表を確保する．
// @param[in] req_size 表のサイズ
void
DictBase::alloc_table(ymuint req_size)
{
  ymuint32 old_size = mHashSize;
  _Cell** old_table = mHashTable;
  mHashSize = req_size;
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
  mHashTable = new _Cell*[mHashSize];
  for (ymuint32 i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  for (ymuint32 i = 0; i < old_size; ++ i) {
    for (_Cell* cell = old_table[i];
	 cell != NULL; ) {
      _Cell* next = cell->mLink;
      _reg_cell(cell);
      cell = next;
    }
  }
  delete [] old_table;
}

// @brief 要素を登録する．
// @param[in] cell 登録する要素
//
// 同じ名前の要素がすでに登録されていたら動作は不定
// reg_cell() との違いはハッシュ表のサイズチェックをしない．
void
DictBase::_reg_cell(_Cell* cell)
{
  ymuint32 h = hash_func(cell->mName) % mHashSize;
  cell->mLink = mHashTable[h];
  mHashTable[h] = cell;
  ++ mNum;
}

END_NAMESPACE_YM
