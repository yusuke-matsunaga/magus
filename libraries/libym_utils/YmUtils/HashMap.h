#ifndef YMUTILS_HASHMAP_H
#define YMUTILS_HASHMAP_H

/// @file YmUtils/HashMap.h
/// @brief HashMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "HashBase.h"

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class HashMapCell HashMap.h "YmUtils/HashFunc.h"
/// @brief HashMap 用のセル
//////////////////////////////////////////////////////////////////////
template<typename Key_Type,
	 typename Value_Type>
struct HashMapCell :
  public HashBaseCell<Key_Type>
{
  HashMapCell() { }

  ~HashMapCell() { }

  /// @brief 値
  Value_Type mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class Ditionary HashMap.h "YmUtils/HashMap.h"
/// @brief 任意の型を保持する連想配列クラス
//////////////////////////////////////////////////////////////////////
template<typename Key_Type,
	 typename Value_Type>
class HashMap :
  public HashBase<Key_Type>
{
  typedef HashBaseCell<Key_Type> BaseCell;
  typedef HashMapCell<Key_Type, Value_Type> Cell;
public:

  /// @brief コンストラクタ
  HashMap();

  /// @brief デストラクタ
  ~HashMap();


 public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief キーで検索して要素を得る．
  /// @param[in] key キー
  /// @param[out] value 結果を格納する変数
  /// @retval true 要素が存在した．
  /// @retval false 要素が存在しなかった．
  bool
  find(const Key_Type& key,
       Value_Type& value) const;

  /// @brief キーで検索して要素の左辺値を得る．
  Value_Type&
  operator[](const Key_Type& key);

  /// @brief キーで検索して要素の左辺値を得る．
  const Value_Type&
  operator[](const Key_Type& key) const;

  /// @brief 要素を登録する．
  /// @param[in] key キー
  /// @param[in] value 登録する要素
  void
  add(const Key_Type& key,
      const Value_Type& value);

  /// @brief キーと要素のリストを得る．
  void
  key_value_list(vector<pair<Key_Type, Value_Type> >& key_value_list) const;

  /// @brief 要素のリストを得る．
  void
  value_list(vector<Value_Type>& value_list) const;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template<typename Key_Type,
	 typename Value_Type>
inline
HashMap<Key_Type, Value_Type>::HashMap()
{
}

// @brief デストラクタ
template<typename Key_Type,
	 typename Value_Type>
inline
HashMap<Key_Type, Value_Type>::~HashMap()
{
}

// @brief キーで検索して要素を得る．
// @param[in] key キー
// @param[out] value 結果を格納する変数
// @retval true 要素が存在した．
// @retval false 要素が存在しなかった．
template<typename Key_Type,
	 typename Value_Type>
inline
bool
HashMap<Key_Type, Value_Type>::find(const Key_Type& key,
				    Value_Type& value) const
{
  Cell* cell = reinterpret_cast<Cell*>(find_cell(key));
  if ( cell != NULL ) {
    value = cell->mValue;
    return true;
  }
  else {
    return false;
  }
}

// @brief キーで検索して要素の左辺値を得る．
template<typename Key_Type,
	 typename Value_Type>
inline
Value_Type&
HashMap<Key_Type, Value_Type>::operator[](const Key_Type& key)
{
  Cell* cell = reinterpret_cast<Cell*>(find_cell(key));
  ASSERT_COND( cell != NULL );
  return cell->mValue;
}

// @brief キーで検索して要素の左辺値を得る．
template<typename Key_Type,
	 typename Value_Type>
inline
const Value_Type&
HashMap<Key_Type, Value_Type>::operator[](const Key_Type& key) const
{
  Cell* cell = reinterpret_cast<Cell*>(find_cell(key));
  ASSERT_COND( cell != NULL );
  return cell->mValue;
}

// @brief 要素を登録する．
// @param[in] key キー
// @param[in] value 登録する要素
template<typename Key_Type,
	 typename Value_Type>
inline
void
HashMap<Key_Type, Value_Type>::add(const Key_Type& key,
				   const Value_Type& value)
{
  Cell* cell = new Cell;
  cell->mKey = key;
  cell->mValue = value;
  reg_cell(cell);
}

// @brief キーと要素のリストを得る．
template<typename Key_Type,
	 typename Value_Type>
inline
void
HashMap<Key_Type, Value_Type>::key_value_list(vector<pair<Key_Type, Value_Type> >& key_value_list) const
{
  vector<BaseCell*> tmp_list;
  cell_list(tmp_list);
  key_value_list.clear();
  key_value_list.reserve(tmp_list.size());
  for (ymuint i = 0; i < tmp_list.size(); ++ i) {
    Cell* cell = reinterpret_cast<Cell*>(tmp_list[i]);
    key_value_list.push_back(make_pair(cell->mKey, cell->mValue));
  }
}

// @brief 要素のリストを得る．
template<typename Key_Type,
	 typename Value_Type>
inline
void
HashMap<Key_Type, Value_Type>::value_list(vector<Value_Type>& value_list) const
{
  vector<BaseCell*> tmp_list;
  cell_list(tmp_list);
  value_list.clear();
  value_list.reserve(tmp_list.size());
  for (ymuint i = 0; i < tmp_list.size(); ++ i) {
    Cell* cell = reinterpret_cast<Cell*>(tmp_list[i]);
    value_list.push_back(cell->mValue);
  }
}

END_NAMESPACE_YM

#endif // YMUTILS_HASHMAP_H
