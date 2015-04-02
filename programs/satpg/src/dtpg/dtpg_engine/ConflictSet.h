#ifndef CONFLICTSET_H
#define CONFLICTSET_H

/// @file ConflictSet.h
/// @brief ConflictSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class ConflictSet ConflictSet.h "ConflictSet.h"
/// @brief ペアのハッシュ表
//////////////////////////////////////////////////////////////////////
class ConflictSet
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_id
  ConflictSet(ymuint max_id);

  /// @brief デストラクタ
  ~ConflictSet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする
  void
  clear();

  /// @brief 要素を追加する．
  void
  add(ymuint id1,
      ymuint id2);

  /// @brief 要素が含まれているかチェックする．
  bool
  check(ymuint id1,
	ymuint id2) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素番号の最大値
  ymuint mMaxId;

  HashSet<ymuint> mHash;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id
inline
ConflictSet::ConflictSet(ymuint max_id) :
  mMaxId(max_id)
{
}

// @brief デストラクタ
inline
ConflictSet::~ConflictSet()
{
}

// @brief クリアする
inline
void
ConflictSet::clear()
{
  mHash.clear();
}

// @brief 要素を追加する．
inline
void
ConflictSet::add(ymuint id1,
		 ymuint id2)
{
  ymuint key;
  if ( id1 < id2 ) {
    key = id1 * mMaxId + id2;
  }
  else {
    key = id2 * mMaxId + id1;
  }
  mHash.add(key);
}

// @brief 要素が含まれているかチェックする．
inline
bool
ConflictSet::check(ymuint id1,
		   ymuint id2) const
{
  ymuint key;
  if ( id1 < id2 ) {
    key = id1 * mMaxId + id2;
  }
  else {
    key = id2 * mMaxId + id1;
  }
  return mHash.check(key);
}

END_NAMESPACE_YM_SATPG

#endif // CONFLICTSET_H
