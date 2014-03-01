#ifndef COMPTBL2E_H
#define COMPTBL2E_H

/// @file CompTbl2e.h
/// @brief CompTbl2e のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl.h"
#include "BddEdge.h"
#include "logic/Expr.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CompTbl2e CompTbl2e.h "CompTbl2e.h"
/// @brief 2つのBDDの枝をキーとしてBDDの枝とExprを結果として格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class CompTbl2e :
  public CompTbl
{
public:

  // コンストラクタ
  CompTbl2e(BddMgrImpl* mgr,
	    const char* name = 0);

  // デストラクタ
  ~CompTbl2e();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられるインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 検索を行なう
  /// @param[in] e1, e2 オペランドの枝
  /// @param[out] ans_cov 結果の論理式
  BddEdge
  get(BddEdge e1,
      BddEdge e2,
      Expr& ans_cov);

  /// @brief 結果を登録する
  /// @param[in] e1, e2 オペランドの枝
  /// @param[in] ans_e 結果の枝
  /// @param[in] ans_cov 結果の論理式
  void
  put(BddEdge e1,
      BddEdge e2,
      BddEdge ans_e,
      const Expr& ans_cov);

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

  struct Cell
  {
    BddEdge mKey1;
    BddEdge mKey2;
    BddEdge mAnsBdd;
    Expr* mAnsCov;
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
CompTbl2e::hash_func(BddEdge id1,
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
CompTbl2e::get(BddEdge id1,
	       BddEdge id2,
	       Expr& ans_cov)
{
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 ) {
    return BddEdge::make_error();
  }
  else {
    ans_cov = *(tmp->mAnsCov);
    return tmp->mAnsBdd;
  }
}

// 結果を登録する
inline
void
CompTbl2e::put(BddEdge id1,
	       BddEdge id2,
	       BddEdge ans_bdd,
	       const Expr& ans_cov)
{
  if ( id1.is_invalid() || id2.is_invalid() || ans_bdd.is_invalid() ) {
    return;
  }
  if ( check_tablesize(mUsedNum) ) {
    if ( !resize(table_size() << 1) ) {
      return;
    }
  }
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1.is_error() ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mAnsBdd = ans_bdd;
  delete tmp->mAnsCov;
  tmp->mAnsCov = new Expr(ans_cov);
}

END_NAMESPACE_YM_BDD

#endif // COMPTBL2E_H
