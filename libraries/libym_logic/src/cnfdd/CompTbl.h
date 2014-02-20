#ifndef COMPTBL_H
#define COMPTBL_H

/// @file CompTbl.h
/// @brief CompTbl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/cnfdd_nsdef.h"
#include "CNFddNode.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class CompTbl CompTbl.h "CompTbl.h"
/// @brief 演算結果テーブルの基本クラス
//////////////////////////////////////////////////////////////////////
class CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr 親の CNFddMgrImpl
  /// @param[in] name 名前(デバッグ用)
  CompTbl(CNFddMgrImpl& mgr,
	  const char* name);

  /// @brief デストラクタ
  virtual
  ~CompTbl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ロックされていないノードに関係したセルをきれいにする．
  virtual
  void
  sweep() = 0;

  /// @brief 内容をクリアする．
  virtual
  void
  clear() = 0;

  /// @brief 使用されているセル数を返す．
  ymuint64
  used_num() const;

  /// @brief テーブルサイズを返す．
  ymuint64
  table_size() const;

  /// @brief 最大のテーブルサイズを設定する．
  /// @param[in] max_size 設定する値
  void
  max_size(ymuint64 max_size);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス向けの便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief next_limitを更新する
  void
  update_next_limit();

  /// @brief テーブルを拡張すべき時には true を返す．
  bool
  check_tablesize() const;

  /// @brief CNFddMgr からメモリを確保する．
  void*
  allocate(ymuint64 size);

  /// @brief CNFddMgr にメモリを返す．
  void
  deallocate(void* ptr,
	     ymuint64 size);

  /// @brief ログ出力用のストリームを得る．
  ostream&
  logstream() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の CNFddMgr
  CNFddMgrImpl& mMgr;

  // 使用されているセルの数
  ymuint64 mUsedNum;

  // テーブルサイズ
  // セルの個数であって，バイト数ではないので注意
  ymuint64 mTableSize;

  // テーブルサイズ - 1 の値，ハッシュのマスクに用いる．
  ymuint64 mTableSize_1;

  // ほとんどデバッグ用の名前
  string mName;

  // ただし，テーブルのサイズはこれ以上大きくしない．
  ymuint64 mMaxSize;

  // mUsedがこの値を越えたらテーブルを拡張する
  ymuint64 mNextLimit;

  // 同じ CNFddMgr に属している次のテーブル
  CompTbl* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class CompTbl1 CompTbl.h "CompTbl.h"
/// @brief 1つの枝をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class CompTbl1 :
  public CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr 親の CNFddMgrImpl
  /// @param[in] name 名前
  CompTbl1(CNFddMgrImpl& mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~CompTbl1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief id1をキーとして検索を行なう
  CNFddEdge
  get(CNFddEdge id1);

  /// @brief 結果を登録する
  void
  put(CNFddEdge id1,
      CNFddEdge ans);

  /// @brief ガーベージコレクションが起きた時の処理を行なう．
  virtual
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
  ymuint64
  hash_func(CNFddEdge id1);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 設定する値
  bool
  resize(ymuint64 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーが1つのセル
  struct Cell
  {
    CNFddEdge mKey1;
    CNFddEdge mAns;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
/// @class CompTbl1n CompTbl.h "CompTbl.h"
/// @brief 1つの枝と数値をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class CompTbl1n :
  public CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr 親の CNFddMgrImpl
  /// @param[in] name 名前
  CompTbl1n(CNFddMgrImpl& mgr,
	    const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~CompTbl1n();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief id1, id2 をキーとして検索を行なう
  CNFddEdge
  get(CNFddEdge id1,
      ymuint id2);

  /// @brief 結果を登録する
  void
  put(CNFddEdge id1,
      ymuint id2,
      CNFddEdge ans);

  /// @brief ガーベージコレクションが起きた時の処理を行なう．
  virtual
  void
  sweep();

  /// @brief 内容をクリアする．
  virtual
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ関数
  ymuint64
  hash_func(CNFddEdge id1,
	    ymuint id2);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 設定する値
  bool
  resize(ymuint64 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーが1つのセル
  struct Cell
  {
    CNFddEdge mKey1;
    ymuint32 mKey2;
    CNFddEdge mAns;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
/// @class CompTbl2 CompTbl.h "CompTbl.h"
/// @brief 2つの枝をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class CompTbl2 :
  public CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr 親の CNFddMgrImpl
  /// @param[in] name 名前
  CompTbl2(CNFddMgrImpl& mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~CompTbl2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief id1, id2をキーとして検索を行なう
  CNFddEdge
  get(CNFddEdge id1,
      CNFddEdge id2);

  /// @brief 結果を登録する
  void
  put(CNFddEdge id1,
      CNFddEdge id2,
      CNFddEdge ans);

  /// @brief ガーベージコレクションが起きた時の処理を行なう．
  virtual
  void
  sweep();

  /// @brief 内容をクリアする．
  virtual
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ関数
  ymuint64
  hash_func(CNFddEdge id1,
	    CNFddEdge id2);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 新しい値
  bool
  resize(ymuint64 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // キーが2つのセル
  struct Cell {
    CNFddEdge mKey1;
    CNFddEdge mKey2;
    CNFddEdge mAns;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// テーブルを拡張すべき時には true を返す．
inline
bool
CompTbl::check_tablesize() const
{
  return mUsedNum > mNextLimit && mTableSize < mMaxSize;
}

// ハッシュ関数
inline
ymuint64
CompTbl1::hash_func(CNFddEdge id1)
{
  return ymuint64((id1.hash() * id1.hash()) >> 8) & mTableSize_1;
}

// id1をキーとして検索を行なう
inline
CNFddEdge
CompTbl1::get(CNFddEdge id1)
{
  Cell* tmp = mTable + hash_func(id1);
  if ( tmp->mKey1 != id1 ) {
    return CNFddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
CompTbl1::put(CNFddEdge id1,
	      CNFddEdge ans)
{
  if ( id1.is_invalid() || ans.is_invalid() ) {
    return;
  }
  if ( check_tablesize() ) {
    if ( !resize(mTableSize << 1) ) {
      return;
    }
  }
  Cell* tmp = mTable + hash_func(id1);
  if ( tmp->mKey1.is_error() ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mAns = ans;
}

// ハッシュ関数
inline
ymuint64
CompTbl1n::hash_func(CNFddEdge id1,
		     ymuint id2)
{
  return ymuint64(((id1.hash() * id1.hash()) >> 8) + id2) & mTableSize_1;
}

// id1をキーとして検索を行なう
inline
CNFddEdge
CompTbl1n::get(CNFddEdge id1,
	      ymuint id2)
{
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 ) {
    return CNFddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
CompTbl1n::put(CNFddEdge id1,
	      ymuint id2,
	      CNFddEdge ans)
{
  if ( id1.is_invalid() || ans.is_invalid() ) {
    return;
  }
  if ( check_tablesize() ) {
    if ( !resize(mTableSize << 1) ) {
      return;
    }
  }
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1.is_error() ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mAns = ans;
}

// ハッシュ関数
inline
ymuint64
CompTbl2::hash_func(CNFddEdge id1,
		    CNFddEdge id2)
{
  ymuint64 v1 = id1.hash();
  ymuint64 v2 = id2.hash();
  return ymuint64(v1 + v2 + v2 + (v1 >> 2) + (v2 >> 4)) & mTableSize_1;
  //    return size_t((id1 * (id2 + 2)) >> 2) & mTableSize_1;
}

// id1, id2をキーとして検索を行なう
inline
CNFddEdge
CompTbl2::get(CNFddEdge id1,
	      CNFddEdge id2)
{
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 ) {
    return CNFddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
CompTbl2::put(CNFddEdge id1,
	      CNFddEdge id2,
	      CNFddEdge ans)
{
  if ( id1.is_invalid() || id2.is_invalid() || ans.is_invalid() ) {
    return;
  }
  if ( check_tablesize() ) {
    if ( !resize(mTableSize << 1) ) {
      return;
    }
  }
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1.is_error() ) mUsedNum ++;
  tmp->mKey1 = id1;
  tmp->mKey2 = id2;
  tmp->mAns = ans;
}

END_NAMESPACE_YM_CNFDD

#endif // COMPTBL_H
