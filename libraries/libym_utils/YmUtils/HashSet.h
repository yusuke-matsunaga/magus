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
  HashSet();

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

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template<typename Key_Type>
inline
HashSet<Key_Type>::HashSet()
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
  Cell* cell = reinterpret_cast<Cell*>(find_cell(key));
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
  reg_cell(cell);
}

END_NAMESPACE_YM

#endif // YMUTILS_HASHSET_H
