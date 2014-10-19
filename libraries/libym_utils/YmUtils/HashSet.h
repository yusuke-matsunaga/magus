#ifndef YMUTILS_HASHSET_H
#define YMUTILS_HASHSET_H

/// @file YmUtils/HashSet.h
/// @brief HashSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "HashBase.h"


BEGIN_NAMESPACE_YM

template<typename Key_Type>
class HashSetIterator;

//////////////////////////////////////////////////////////////////////
/// @class HashSet HashSet.h "YmUtils/HashSet.h"
/// @brief ハッシュを用いた集合を表すクラス
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
class HashSet :
  public HashBase<Key_Type>
{
  typedef HashBaseCell<Key_Type> Cell;
public:

  /// @brief コンストラクタ
  /// @param[in] size 表の初期サイズ
  HashSet(ymuint size = 1024);

  /// @brief デストラクタ
  ~HashSet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief キーで検索して要素を得る．
  /// @param[in] key キー
  /// @retval true 要素が存在した．
  /// @retval false 要素が存在しなかった．
  bool
  find(const Key_Type& key) const;

  /// @brief 要素を登録する．
  /// @param[in] key キー
  void
  add(const Key_Type& key);

  /// @brief 先頭の反復子を返す．
  HashSetIterator<Key_Type>
  begin() const;

  /// @brief 末尾の反復子を返す．
  HashSetIterator<Key_Type>
  end() const;

};


//////////////////////////////////////////////////////////////////////
/// @class HashSetIterator HashSet.h "YmUtils/HashSet.h"
/// @brief HashSet の反復子
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
class HashSetIterator
{
  friend class HashSet<Key_Type>;
public:

  /// @brief 空のコンストラクタ
  HashSetIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  HashSetIterator(const HashSetIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const HashSetIterator&
  operator=(const HashSetIterator& src);

  /// @brief デストラクタ
  ~HashSetIterator();

  /// @brief キーを返す．
  Key_Type
  key() const;

  /// @brief 一つ進める(前置演算子)
  /// @return 進めた後の反復子を返す．
  HashSetIterator
  operator++();

  /// @brief 一つ進める(後置演算子)
  /// @return 進める前の反復子を返す．
  /// @note int は後置演算子を表すためのダミー
  HashSetIterator
  operator++(int);

  /// @brief 等価比較演算子
  bool
  operator==(const HashSetIterator& src) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const HashSetIterator& src) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定するコンストラクタ
  HashSetIterator(HashBaseIterator<Key_Type> base_obj);


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
template<typename Key_Type>
inline
HashSet<Key_Type>::HashSet(ymuint size) :
  HashBase<Key_Type>(size)
{
}

// @brief デストラクタ
template<typename Key_Type>
inline
HashSet<Key_Type>::~HashSet()
{
}

// @brief キーで検索して要素を得る．
// @param[in] key キー
// @retval true 要素が存在した．
// @retval false 要素が存在しなかった．
template<typename Key_Type>
inline
bool
HashSet<Key_Type>::find(const Key_Type& key) const
{
  Cell* cell = reinterpret_cast<Cell*>(HashBase<Key_Type>::find_cell(key));
  if ( cell != NULL ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 要素を登録する．
// @param[in] key キー
template<typename Key_Type>
inline
void
HashSet<Key_Type>::add(const Key_Type& key)
{
  Cell* cell = new Cell;
  cell->mKey = key;
  HashBase<Key_Type>::reg_cell(cell);
}

// @brief 先頭の反復子を返す．
template<typename Key_Type>
inline
HashSetIterator<Key_Type>
HashSet<Key_Type>::begin() const
{
  return HashSetIterator<Key_Type>(HashBase<Key_Type>::begin());
}

// @brief 末尾の反復子を返す．
template<typename Key_Type>
inline
HashSetIterator<Key_Type>
HashSet<Key_Type>::end() const
{
  return HashSetIterator<Key_Type>(HashBase<Key_Type>::end());
}

// @brief 空のコンストラクタ
template<typename Key_Type>
inline
HashSetIterator<Key_Type>::HashSetIterator()
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
template<typename Key_Type>
inline
HashSetIterator<Key_Type>::HashSetIterator(const HashSetIterator& src) :
  mBase(src.mBase)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
template<typename Key_Type>
inline
const HashSetIterator<Key_Type>&
HashSetIterator<Key_Type>::operator=(const HashSetIterator& src)
{
  mBase = src.mBase;
}

// @brief デストラクタ
template<typename Key_Type>
inline
HashSetIterator<Key_Type>::~HashSetIterator()
{
}

// @brief キーを返す．
template<typename Key_Type>
inline
Key_Type
HashSetIterator<Key_Type>::key() const
{
  HashBaseCell<Key_Type>* cell = mBase.cell();
  return cell->mKey;
}

// @brief 一つ進める(前置演算子)
// @return 進めた後の反復子を返す．
template<typename Key_Type>
inline
HashSetIterator<Key_Type>
HashSetIterator<Key_Type>::operator++()
{
  ++ mBase;
  return *this;
}

// @brief 一つ進める(後置演算子)
// @return 進める前の反復子を返す．
// @note int は後置演算子を表すためのダミー
template<typename Key_Type>
inline
HashSetIterator<Key_Type>
HashSetIterator<Key_Type>::operator++(int)
{
  HashSetIterator cur(*this);
  ++ mBase;
  return cur;
}

// @brief 等価比較演算子
template<typename Key_Type>
inline
bool
HashSetIterator<Key_Type>::operator==(const HashSetIterator& src) const
{
  return mBase == src.mBase;
}

// @brief 非等価比較演算子
template<typename Key_Type>
inline
bool
HashSetIterator<Key_Type>::operator!=(const HashSetIterator& src) const
{
  return !operator==(src);
}

// @brief 内容を指定するコンストラクタ
template<typename Key_Type>
inline
HashSetIterator<Key_Type>::HashSetIterator(HashBaseIterator<Key_Type> base_obj) :
  mBase(base_obj)
{
}

END_NAMESPACE_YM

#endif // YMUTILS_HASHSET_H
