#ifndef COMPTBL2_H
#define COMPTBL2_H

/// @file CompTbl2.h
/// @brief CompTbl2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CompTbl2 CompTbl2.h "CompTbl2.h"
/// @brief 2つの枝をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class CompTbl2 :
  public CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name 名前
  CompTbl2(BddMgrImpl* mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  ~CompTbl2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられるインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 検索を行なう
  /// @param[in] e1, e2 オペランドの枝
  BddEdge
  get(BddEdge e1,
      BddEdge e2);

  /// @brief 結果を登録する
  /// @param[in] e1, e2 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(BddEdge e1,
      BddEdge e2,
      BddEdge ans);

  /// @brief ガーベージコレクションが起きた時の処理を行なう．
  /// 参照されていないノードに関連したエントリを削除する．
  void
  sweep();

  /// @brief 内容をクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ関数
  /// @param[in] e1, e2 オペランドの枝
  ymuint64
  hash_func(BddEdge e1,
	    BddEdge e2);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 新しいサイズ
  bool
  resize(ymuint64 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーが2つのセル
  struct Cell {
    BddEdge mKey1;
    BddEdge mKey2;
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
CompTbl2::hash_func(BddEdge id1,
		    BddEdge id2)
{
  ymuint64 v1 = id1.hash();
  ymuint64 v2 = id2.hash();
  return (v1 + v2 + v2 + (v1 >> 2) + (v2 >> 4)) % table_size();
  //    return size_t((id1 * (id2 + 2)) >> 2) & mTableSize_1;
}

// id1, id2をキーとして検索を行なう
inline
BddEdge
CompTbl2::get(BddEdge id1,
	      BddEdge id2)
{
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 ) {
    return BddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
CompTbl2::put(BddEdge id1,
	      BddEdge id2,
	      BddEdge ans)
{
  if ( id1.is_invalid() || id2.is_invalid() || ans.is_invalid() ) {
    return;
  }
  if ( check_tablesize(mUsedNum) ) {
    if ( !resize(table_size() << 1) ) {
      return;
    }
  }
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1.is_error() ) ++ mUsedNum;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mAns = ans;
}

END_NAMESPACE_YM_BDD

#endif // COMPTBL2_H
