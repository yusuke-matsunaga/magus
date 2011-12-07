#ifndef COMPTBL_H
#define COMPTBL_H

/// @file CompTbl.h
/// @brief CompTbl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "ZddNode.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class CompTbl CompTbl.h "CompTbl.h"
/// @brief 演算結果テーブルの基本クラス
//////////////////////////////////////////////////////////////////////
class CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr 親の ZddMgrImpl
  /// @param[in] name 名前(デバッグ用)
  CompTbl(ZddMgrImpl& mgr,
	  const char* name);

  /// @brief デストラクタ
  virtual
  ~CompTbl();


public:

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

  /// @brief load_limit を設定する．
  /// @param[in] load_limit 設定する値
  void
  load_limit(double load_limit);

  /// @brief 最大のテーブルサイズを設定する．
  /// @param[in] max_size 設定する値
  void
  max_size(ymuint64 max_size);

  /// @brief 次の要素を返す．
  CompTbl*
  next() const;


protected:

  /// @brief next_limitを更新する
  void
  update_next_limit();

  /// @brief テーブルを拡張すべき時には true を返す．
  bool
  check_tablesize() const;

  /// @brief ZddMgr からメモリを確保する．
  void*
  allocate(ymuint64 size);

  /// @brief ZddMgr にメモリを返す．
  void
  deallocate(void* ptr,
	     ymuint64 size);

  /// @brief ログ出力用のストリームを得る．
  ostream&
  logstream() const;

  /// @brief e の参照回数が0なら true を返す．
  static
  bool
  check_noref(ZddEdge e);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の ZddMgr
  ZddMgrImpl& mMgr;

  // 使用されているセルの数
  ymuint64 mUsedNum;

  // テーブルサイズ
  // セルの個数であって，バイト数ではないので注意
  ymuint64 mTableSize;

  // テーブルサイズ - 1 の値，ハッシュのマスクに用いる．
  ymuint64 mTableSize_1;

  // ほとんどデバッグ用の名前
  string mName;

  // テーブルの使用率がこの値を越えたらサイズの拡張を行なう．
  double mLoadLimit;

  // ただし，テーブルのサイズはこれ以上大きくしない．
  ymuint64 mMaxSize;

  // mUsedがこの値を越えたらテーブルを拡張する
  ymuint64 mNextLimit;

  // 同じ ZddMgr に属している次のテーブル
  CompTbl* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class CompTbl1 CompTbl.h "CompTbl.h"
/// @brief 1つの枝をキーとして結果の枝を格納するテーブル
//////////////////////////////////////////////////////////////////////
class CompTbl1 :
  public CompTbl
{

  // キーが1つのセル
  struct Cell
  {
    ZddEdge mKey1;
    ZddEdge mAns;
  };

public:

  /// @brief コンストラクタ
  /// @param[in] mgr 親の ZddMgrImpl
  /// @param[in] name 名前
  CompTbl1(ZddMgrImpl& mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~CompTbl1();


public:

  /// @brief id1をキーとして検索を行なう
  ZddEdge
  get(ZddEdge id1);

  /// @brief 結果を登録する
  void
  put(ZddEdge id1,
      ZddEdge ans);


private:

  /// @brief ハッシュ関数
  ymuint64
  hash_func(ZddEdge id1);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 設定する値
  bool
  resize(ymuint64 new_size);

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

  // キーが2つのセル
  struct Cell {
    ZddEdge mKey1;
    ZddEdge mKey2;
    ZddEdge mAns;
  };

public:

  /// @brief コンストラクタ
  /// @param[in] mgr 親の ZddMgrImpl
  /// @param[in] name 名前
  CompTbl2(ZddMgrImpl& mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~CompTbl2();


public:

  /// @brief id1, id2をキーとして検索を行なう
  ZddEdge
  get(ZddEdge id1,
      ZddEdge id2);

  /// @brief 結果を登録する
  void
  put(ZddEdge id1,
      ZddEdge id2,
      ZddEdge ans);


private:

  /// @brief ハッシュ関数
  ymuint64
  hash_func(ZddEdge id1,
	    ZddEdge id2);

  /// @brief テーブルサイズを変更する．
  /// @param[in] new_size 新しい値
  bool
  resize(ymuint64 new_size);

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のテーブル
  Cell* mTable;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を返す．
inline
CompTbl*
CompTbl::next() const
{
  return mNext;
}

// テーブルを拡張すべき時には true を返す．
inline
bool
CompTbl::check_tablesize() const
{
  return mUsedNum > mNextLimit && mTableSize < mMaxSize;
}

// e の参照回数が0なら true を返す．
inline
bool
CompTbl::check_noref(ZddEdge e)
{
  ZddNode* vp = e.get_node();
  return vp && vp->noref();
}

// ハッシュ関数
inline
ymuint64
CompTbl1::hash_func(ZddEdge id1)
{
  return ymuint64((id1.hash() * id1.hash()) >> 8) & mTableSize_1;
}

// id1をキーとして検索を行なう
inline
ZddEdge
CompTbl1::get(ZddEdge id1)
{
  Cell* tmp = mTable + hash_func(id1);
  if ( tmp->mKey1 != id1 ) {
    return ZddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
CompTbl1::put(ZddEdge id1,
	      ZddEdge ans)
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
CompTbl2::hash_func(ZddEdge id1,
		    ZddEdge id2)
{
  ymuint64 v1 = id1.hash();
  ymuint64 v2 = id2.hash();
  return ymuint64(v1 + v2 + v2 + (v1 >> 2) + (v2 >> 4)) & mTableSize_1;
  //    return size_t((id1 * (id2 + 2)) >> 2) & mTableSize_1;
}

// id1, id2をキーとして検索を行なう
inline
ZddEdge
CompTbl2::get(ZddEdge id1,
	      ZddEdge id2)
{
  Cell* tmp = mTable + hash_func(id1, id2);
  if ( tmp->mKey1 != id1 || tmp->mKey2 != id2 ) {
    return ZddEdge::make_error();
  }
  else {
    return tmp->mAns;
  }
}

// 結果を登録する
inline
void
CompTbl2::put(ZddEdge id1,
	      ZddEdge id2,
	      ZddEdge ans)
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

END_NAMESPACE_YM_ZDD

#endif // COMPTBL_H
