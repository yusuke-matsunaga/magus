#ifndef COMPTBL1_H
#define COMPTBL1_H

/// @file CompTbl1.h
/// @brief CompTbl1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CompTbl1 CompTbl1.h "CompTbl1.h"
/// @brief 1つの枝をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class CompTbl1 :
  public CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name 名前
  CompTbl1(BddMgrImpl* mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~CompTbl1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられるインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 検索を行なう
  /// @param[in] e1 オペランドの枝
  BddEdge
  get(BddEdge e1);

  /// @brief 結果を登録する
  /// @param[in] e1 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(BddEdge e1,
      BddEdge ans);

  /// @brief ガーベージコレクションが起きた時の処理を行なう．
  /// 参照されていないノードに関連したエントリを削除する．
  void
  sweep();

  /// @brief 内容をクリアする．
  virtual
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ関数
  /// @param[in] e1 オペランドの枝
  ymuint64
  hash_func(BddEdge e1);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 新しいサイズ
  bool
  resize(ymuint64 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーが1つのセル
  struct Cell
  {
    BddEdge mKey1;
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
CompTbl1::hash_func(BddEdge id1)
{
  return static_cast<ymuint64>(((id1.hash() * id1.hash()) >> 8) % table_size());
}

// id1をキーとして検索を行なう
inline
BddEdge
CompTbl1::get(BddEdge id1)
{
  Cell* tmp = mTable + hash_func(id1);
  if ( tmp->mKey1 != id1 ) {
    return BddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
CompTbl1::put(BddEdge id1,
	      BddEdge ans)
{
  if ( id1.is_invalid() || ans.is_invalid() ) {
    return;
  }
  if ( check_tablesize(mUsedNum) ) {
    if ( !resize(table_size() << 1) ) {
      return;
    }
  }
  Cell* tmp = mTable + hash_func(id1);
  if ( tmp->mKey1.is_error() ) ++ mUsedNum;
  tmp->mKey1 = id1;
  tmp->mAns = ans;
}

END_NAMESPACE_YM_BDD

#endif // COMPTBL1_H
