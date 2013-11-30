#ifndef COMPTBL3_H
#define COMPTBL3_H

/// @file CompTbl3.h
/// @brief CompTbl3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// class CompTbl3 CompTbl3.h "CompTbl3.h"
/// @brief 3つの枝をキーとする演算結果テーブル．
/// if-then-else演算で用いられる
//////////////////////////////////////////////////////////////////////
class CompTbl3 :
  public CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name 名前
  CompTbl3(BddMgrImpl* mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  ~CompTbl3();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられるインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 検索を行なう
  /// @param[in] e1, e2, e3 オペランドの枝
  BddEdge
  get(BddEdge e1,
      BddEdge e2,
      BddEdge e3);

  /// @brief 登録を行なう．
  /// @param[in] e1, e2, e3 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(BddEdge id1,
      BddEdge id2,
      BddEdge id3,
      BddEdge ans);

  /// @brief ガーベージコレクションが起きた時の処理を行なう．
  /// 参照されていないノードに関連したエントリを削除する．
  void
  sweep();

  /// @brief クリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ関数
  /// @param[in] e1, e2, e3 オペランドの枝
  ymuint64
  hash_func(BddEdge e1,
	    BddEdge e2,
	    BddEdge e);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 新しいサイズ
  bool
  resize(ymuint64 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーが3つのセル
  struct Cell
  {
    BddEdge mKey1;
    BddEdge mKey2;
    BddEdge mKey3;
    BddEdge mAns;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テーブルの本体
  Cell* mTable;

  // 使用されているセルの数
  ymuint64 mUsedNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// ハッシュ関数
inline
ymuint64
CompTbl3::hash_func(BddEdge id1,
		    BddEdge id2,
		    BddEdge id3)
{
  ymuint64 v1 = id1.hash();
  ymuint64 v2 = id2.hash();
  ymuint64 v3 = id3.hash();
  return (v1 + v2 + v3 + (v1 >> 2) + (v2 >> 4) + (v3 >> 6)) % table_size();
}

// 検索を行なう．
inline
BddEdge
CompTbl3::get(BddEdge id1,
	      BddEdge id2,
	      BddEdge id3)
{
  Cell* tmp = mTable + hash_func(id1, id2, id3);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 || tmp->mKey3 != id3 ) {
    return BddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 登録を行なう．
inline
void
CompTbl3::put(BddEdge id1,
	      BddEdge id2,
	      BddEdge id3,
	      BddEdge ans)
{
  if ( id1.is_invalid() ||
       id2.is_invalid() ||
       id3.is_invalid() ||
       ans.is_invalid() ) {
    return;
  }
  if ( check_tablesize(mUsedNum) ) {
    if ( !resize(table_size() << 1) ) {
      return;
    }
  }
  Cell* tmp = mTable + hash_func(id1, id2, id3);
  if ( tmp->mKey1.is_error() ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mKey3 = id3;
  tmp->mAns = ans;
}

END_NAMESPACE_YM_BDD

#endif // COMPTBL3_H
