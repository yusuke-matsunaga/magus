#ifndef BMMVAR_H
#define BMMVAR_H

/// @file BmmVar.h
/// @brief BmmVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

class BddMgrModern;
class BmmNode;

//////////////////////////////////////////////////////////////////////
// 変数の情報を格納しておくクラス
//////////////////////////////////////////////////////////////////////
class BmmVar
{
  friend class BddMgrModern;

public:
  typedef BmmNode Node;

public:

  // 変数番号を得る．
  VarId
  varid() const
  {
    return mId;
  }

  // レベルを得る．
  ymuint
  level() const
  {
    return mLevel;
  }

private:

  // コンストラクタ
  BmmVar(BddMgrModern* mgr,
	 VarId id);

  // デストラクタ
  ~BmmVar();

  // gc 用の sweep 処理
  void
  sweep();

  // 節点テーブルが縮小可能なら縮小する．
  void
  shrink(double load_limit);

  // ノードを登録する．
  void
  reg_node(ymuint64 pos,
	   BmmNode* node);

  // 節点テーブルを拡張する
  // メモリアロケーションに失敗したら false を返す．
  bool
  resize(ymuint64 new_size);

  // 次のリミット値を計算する
  void
  set_next_limit_size(double load_limit);

  // ログ出力用のストリームを得る．
  ostream&
  logstream() const;

  // 節点テーブル用のメモリを確保する．
  // size はバイト単位ではなくエントリ数
  BmmNode**
  alloc_nodetable(ymuint64 size);

  // 節点テーブル用のメモリを解放する．
  // size はバイト単位ではなくエントリ数
  void
  dealloc_nodetable(BmmNode** table,
		    ymuint64 size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の BddMgr
  BddMgrModern* mMgr;

  // 変数番号
  // これは不変
  VarId mId;

  // レベル
  // これは可変
  ymuint32 mLevel;

  // 作業用のマーク
  int mMark;

  // compose用にBDDの枝を入れておくメンバ
  BddEdge mCompEdge;

  // ハッシュ表中の次の要素を指すポインタ
  BmmVar* mLink;

  //////////////////////////////////////////////////////////////////////
  // 節点テーブル関係のメンバ
  //////////////////////////////////////////////////////////////////////

  // テーブルサイズ
  ymuint64 mTableSize;

  // テーブルサイズ - 1
  ymuint64 mTableSize_1;

  // ノード数がこの数を越えたらテーブルサイズを拡張する．
  ymuint64 mNextLimit;

  // テーブル本体
  BmmNode** mNodeTable;

  // ノード数
  ymuint64 mNodeNum;

};

END_NAMESPACE_YM_BDD

#endif // BMMVAR_H
