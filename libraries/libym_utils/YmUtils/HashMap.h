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

template<typename Key_Type,
	 typename Value_Type>
class HashMapIterator;

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
/// @class HashMap HashMap.h "YmUtils/HashMap.h"
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
  /// @param[in] size 表の初期サイズ
  HashMap(ymuint size = 1024);

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

  /// @brief 先頭の反復子を返す．
  HashMapIterator<Key_Type, Value_Type>
  begin() const;

  /// @brief 末尾の反復子を返す．
  HashMapIterator<Key_Type, Value_Type>
  end() const;

};


//////////////////////////////////////////////////////////////////////
/// @class HashMapIterator HashMap.h "YmUtils/HashMap.h"
/// @brief HashMap の反復子
//////////////////////////////////////////////////////////////////////
template<typename Key_Type,
	 typename Value_Type>
class HashMapIterator
{
  friend class HashMap<Key_Type, Value_Type>;
public:

  /// @brief 空のコンストラクタ
  HashMapIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  HashMapIterator(const HashMapIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const HashMapIterator&
  operator=(const HashMapIterator& src);

  /// @brief デストラクタ
  ~HashMapIterator();

  /// @brief キーを返す．
  Key_Type
  key() const;

  /// @brief 値を返す．
  Value_Type
  value() const;

  /// @brief 一つ進める(前置演算子)
  /// @return 進めた後の反復子を返す．
  HashMapIterator
  operator++();

  /// @brief 一つ進める(後置演算子)
  /// @return 進める前の反復子を返す．
  /// @note int は後置演算子を表すためのダミー
  HashMapIterator
  operator++(int);

  /// @brief 等価比較演算子
  bool
  operator==(const HashMapIterator& src) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const HashMapIterator& src) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定するコンストラクタ
  HashMapIterator(HashBaseIterator<Key_Type> base_obj);

  /// @brief ポインタを進める．
  void
  next_ptr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の反復子
  HashBaseIterator<Key_Type> mBase;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template<typename Key_Type,
	 typename Value_Type>
inline
HashMap<Key_Type, Value_Type>::HashMap(ymuint size) :
  HashBase<Key_Type>(size)
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

// @brief 先頭の反復子を返す．
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>
HashMap<Key_Type, Value_Type>::begin() const
{
  return HashMapIterator<Key_Type, Value_Type>(HashBase<Key_Type>::begin());
}

// @brief 末尾の反復子を返す．
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>
HashMap<Key_Type, Value_Type>::end() const
{
  return HashMapIterator<Key_Type, Value_Type>(HashBase<Key_Type>::end());
}

// @brief 空のコンストラクタ
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>::HashMapIterator()
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>::HashMapIterator(const HashMapIterator& src) :
  mBase(src.mBase)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
template<typename Key_Type,
	 typename Value_Type>
inline
const HashMapIterator<Key_Type, Value_Type>&
HashMapIterator<Key_Type, Value_Type>::operator=(const HashMapIterator& src)
{
  mBase = src.mBase;
}

// @brief デストラクタ
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>::~HashMapIterator()
{
}

// @brief キーを返す．
template<typename Key_Type,
	 typename Value_Type>
inline
Key_Type
HashMapIterator<Key_Type, Value_Type>::key() const
{
  HashBaseCell<Key_Type>* cell = mBase.cell();
  return cell->mKey;
}

// @brief 値を返す．
template<typename Key_Type,
	 typename Value_Type>
inline
Value_Type
HashMapIterator<Key_Type, Value_Type>::value() const
{
  HashMapCell<Key_Type, Value_Type>* cell
    = reinterpret_cast<HashMapCell<Key_Type, Value_Type>*>(mBase.cell());
  return cell->mValue;
}

// @brief 一つ進める(前置演算子)
// @return 進めた後の反復子を返す．
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>
HashMapIterator<Key_Type, Value_Type>::operator++()
{
  ++ mBase;
  return *this;
}

// @brief 一つ進める(後置演算子)
// @return 進める前の反復子を返す．
// @note int は後置演算子を表すためのダミー
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>
HashMapIterator<Key_Type, Value_Type>::operator++(int)
{
  HashMapIterator cur(*this);
  ++ mBase;
  return cur;
}

// @brief 等価比較演算子
template<typename Key_Type,
	 typename Value_Type>
inline
bool
HashMapIterator<Key_Type, Value_Type>::operator==(const HashMapIterator& src) const
{
  return mBase == src.mBase;
}

// @brief 非等価比較演算子
template<typename Key_Type,
	 typename Value_Type>
inline
bool
HashMapIterator<Key_Type, Value_Type>::operator!=(const HashMapIterator& src) const
{
  return !operator==(src);
}

// @brief 内容を指定するコンストラクタ
template<typename Key_Type,
	 typename Value_Type>
inline
HashMapIterator<Key_Type, Value_Type>::HashMapIterator(HashBaseIterator<Key_Type> base_obj) :
  mBase(base_obj)
{
}

END_NAMESPACE_YM

#endif // YMUTILS_HASHMAP_H
