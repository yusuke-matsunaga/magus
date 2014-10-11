#ifndef YMUTILS_HASHBASE_H
#define YMUTILS_HASHBASE_H

/// @file YmUtils/HashBase.h
/// @brief HaseBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmUtils/HashFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class HashBaseCell HashBase.h "YmUtils/HashBase.h"
/// @brief HashBase 用のセル
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
struct HashBaseCell
{
  HashBaseCell() { }

  ~HashBaseCell() { }

  /// @brief キー
  Key_Type mKey;

  /// @brief 次のセルへのリンク
  HashBaseCell* mLink;
};


//////////////////////////////////////////////////////////////////////
/// @class HashBase HashBase.h "YmUtils/HashBase.h"
/// @brief ハッシュ表の基底クラス
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
class HashBase
{
  typedef HashBaseCell<Key_Type> Cell;

protected:

  /// @brief コンストラクタ
  HashBase();

  /// @brief デストラクタ
  ~HashBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief 要素の有無をチェックする．
  /// @param[in] key 調べる要素のキー
  /// @retval true key というキーを持つ要素が存在する．
  /// @retval false key というキーを持つ要素が存在しない．
  bool
  check(Key_Type key) const;

  /// @brief 要素を削除する．
  /// @param[in] key 削除する要素のキー
  ///
  /// 該当の要素が存在しない場合にはなにもしない．
  void
  erase(Key_Type key);

  /// @brief 登録されているキーのリストを返す．
  /// @param[out] key_list キーのリストを格納するリスト
  void
  key_list(vector<Key_Type>& key_list) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // HashSet/HashMapで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief キーが一致する要素を返す．
  /// @param[in] key キー
  /// @return key と一致する要素を返す．
  ///
  /// 見つからなければ NULL を返す．
  Cell*
  find_cell(Key_Type key) const;

  /// @brief 登録されているセルのリストを返す．
  /// @param[out] cell_list セルのリストを格納するリスト
  void
  cell_list(vector<Cell*>& cell_list) const;

  /// @brief 要素を登録する．
  /// @param[in] cell 登録する要素
  ///
  /// 同じ名前の要素がすでに登録されていたら動作は不定
  void
  reg_cell(Cell* cell);

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 表のサイズ
  void
  alloc_table(ymuint req_size);

  /// @brief 要素を登録する．
  /// @param[in] cell 登録する要素
  ///
  /// 同じ名前の要素がすでに登録されていたら動作は不定
  /// reg_cell() との違いはハッシュ表のサイズチェックをしない．
  void
  _reg_cell(Cell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のサイズ
  ymuint mHashSize;

  // ハッシュ表
  Cell** mHashTable;

  // 拡大する目安
  ymuint mNextLimit;

  // 要素数
  ymuint mNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template<typename Key_Type>
inline
HashBase<Key_Type>::HashBase()
{
  mHashSize = 0;
  mHashTable = NULL;
  mNum = 0;
  alloc_table(1024);
}

// @brief デストラクタ
template<typename Key_Type>
inline
HashBase<Key_Type>::~HashBase()
{
  clear();
  delete [] mHashTable;
}

// @brief 内容をクリアする．
template<typename Key_Type>
inline
void
HashBase<Key_Type>::clear()
{
  for (ymuint i = 0; i < mHashSize; ++ i) {
    for (Cell* cell = mHashTable[i]; cell != NULL; ) {
      Cell* tmp = cell;
      cell = tmp->mLink;
      delete tmp;
    }
    mHashTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 要素数を返す．
template<typename Key_Type>
inline
ymuint
HashBase<Key_Type>::num() const
{
  return mNum;
}

// @brief 要素の有無をチェックする．
// @param[in] key 調べる要素のキー
// @retval true key というキーを持つ要素が存在する．
// @retval false key というキーを持つ要素が存在しない．
template<typename Key_Type>
inline
bool
HashBase<Key_Type>::check(Key_Type key) const
{
  return find_cell(key) != NULL;
}

// @brief 要素を削除する．
// @param[in] key 削除する要素のキー
//
// 該当の要素が存在しない場合にはなにもしない．
template<typename Key_Type>
inline
void
HashBase<Key_Type>::erase(Key_Type key)
{
  HashFunc<Key_Type> hash_func;
  ymuint h = hash_func(key) % mHashSize;
  Cell** pprev = &mHashTable[h];
  Cell* cell;
  while ( (cell = *pprev) != NULL ) {
    if ( cell->mKey == key ) {
      // 見つけた
      *pprev = cell->mLink;
      delete cell;
      -- mNum;
      break;
    }
    pprev = &cell->mLink;
  }
}

// @brief 登録されているキーのリストを返す．
// @param[out] key_list キーのリストを格納するリスト
template<typename Key_Type>
inline
void
HashBase<Key_Type>::key_list(vector<Key_Type>& key_list) const
{
  key_list.clear();
  key_list.reserve(num());
  for (ymuint i = 0; i < mHashSize; ++ i) {
    for (Cell* cell = mHashTable[i]; cell != NULL;
	 cell = cell->mLink) {
      key_list.push_back(cell->mKey);
    }
  }
}

// @brief キーが一致する要素を返す．
// @param[in] key キー
// @return key と一致する要素を返す．
//
// 見つからなければ NULL を返す．
template<typename Key_Type>
inline
HashBaseCell<Key_Type>*
HashBase<Key_Type>::find_cell(Key_Type key) const
{
  HashFunc<Key_Type> hash_func;
  ymuint h = hash_func(key) % mHashSize;
  for (Cell* cell = mHashTable[h]; cell != NULL;
       cell = cell->mLink) {
    if ( cell->mKey == key ) {
      return cell;
    }
  }
  return NULL;
}

// @brief 登録されているセルのリストを返す．
// @param[out] cell_list セルのリストを格納するリスト
template<typename Key_Type>
inline
void
HashBase<Key_Type>::cell_list(vector<Cell*>& cell_list) const
{
  cell_list.clear();
  cell_list.reserve(num());
  for (ymuint i = 0; i < mHashSize; ++ i) {
    for (Cell* cell = mHashTable[i]; cell != NULL;
	 cell = cell->mLink) {
      cell_list.push_back(cell);
    }
  }
}

// @brief 要素を登録する．
// @param[in] cell 登録する要素
//
// 同じ名前の要素がすでに登録されていたら動作は不定
template<typename Key_Type>
inline
void
HashBase<Key_Type>::reg_cell(Cell* cell)
{
  if ( check(cell->mKey) ) {
    return;
  }
  if ( mNum >= mNextLimit ) {
    alloc_table(mHashSize * 2);
  }
  _reg_cell(cell);
}

// @brief ハッシュ表を確保する．
// @param[in] req_size 表のサイズ
template<typename Key_Type>
inline
void
HashBase<Key_Type>::alloc_table(ymuint req_size)
{
  ymuint old_size = mHashSize;
  Cell** old_table = mHashTable;
  mHashSize = req_size;
  mNextLimit = static_cast<ymuint>(mHashSize * 1.8);
  mHashTable = new Cell*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  for (ymuint i = 0; i < old_size; ++ i) {
    for (Cell* cell = old_table[i];
	 cell != NULL; ) {
      Cell* next = cell->mLink;
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
template<typename Key_Type>
inline
void
HashBase<Key_Type>::_reg_cell(Cell* cell)
{
  HashFunc<Key_Type> hash_func;
  ymuint h = hash_func(cell->mKey) % mHashSize;
  cell->mLink = mHashTable[h];
  mHashTable[h] = cell;
  ++ mNum;
}

END_NAMESPACE_YM

#endif // YMUTILS_HASHBASE_H
