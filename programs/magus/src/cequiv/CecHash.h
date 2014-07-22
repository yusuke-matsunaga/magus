#ifndef LIBYM_CEC_CECHASH_H
#define LIBYM_CEC_CECHASH_H

/// @file libym_cec/CecHash.h
/// @brief CecHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"


BEGIN_NAMESPACE_YM_CEC

class CecNode;

//////////////////////////////////////////////////////////////////////
/// @class CecHash CecHash.h "CecHash.h"
/// @brief CecNode のハッシュ表を実装するためのクラス
//////////////////////////////////////////////////////////////////////
class CecHash
{
public:

  /// @brief コンストラクタ
  CecHash();

  /// @brief デストラクタ
  ~CecHash();


public:

  /// @brief 要素数を得る．
  ymuint
  num() const;

  /// @brief テーブルサイズを得る．
  ymuint
  table_size() const;

  /// @brief 先頭の要素を得る．
  CecNode*
  elem(ymuint hash_val) const;

  /// @brief 拡張が必要か調べる．
  bool
  need_expand();

  /// @brief 内容を空にする．
  /// @note テーブルサイズはそのまま
  void
  clear();

  /// @brief 要素を追加する．
  void
  add_elem(ymuint hash_val,
	   CecNode* node,
	   CecNode*& link);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mNum;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // ハッシュ表
  CecNode** mHashTable;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 要素数を得る．
inline
ymuint
CecHash::num() const
{
  return mNum;
}

// @brief テーブルサイズを得る．
inline
ymuint
CecHash::table_size() const
{
  return mHashSize;
}

// @brief 先頭の要素を得る．
inline
CecNode*
CecHash::elem(ymuint hash_val) const
{
  return mHashTable[hash_val % mHashSize];
}

// @brief 拡張が必要か調べる．
inline
bool
CecHash::need_expand()
{
  if ( mNum >= mNextLimit ) {
    alloc_table(mHashSize << 1);
    return true;
  }
  return false;
}

// @brief 内容を空にする．
// @note テーブルサイズはそのまま
inline
void
CecHash::clear()
{
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 要素を追加する．
inline
void
CecHash::add_elem(ymuint hash_val,
		  CecNode* node,
		  CecNode*& link)
{
  ymuint idx = hash_val % mHashSize;
  link = mHashTable[idx];
  mHashTable[idx] = node;
  ++ mNum;
}

END_NAMESPACE_YM_CEC

#endif // LIBYM_CEC_CECHASH_H
