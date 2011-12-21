#ifndef COMPTBL_H
#define COMPTBL_H

/// @file CompTbl.h
/// @brief CompTbl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CompTbl CompTbl.h "CompTbl.h"
/// @brief 演算結果テーブルの基本クラス
/// 実際にはこれの継承クラスを定義する必要がある
//////////////////////////////////////////////////////////////////////
class CompTbl
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name デバッグ用
  CompTbl(BddMgrImpl* mgr,
	  const char* name);

  /// @brief デストラクタ
  virtual
  ~CompTbl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられるインターフェイス
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

  /// @brief load_limit を設定する．
  void
  load_limit(double load_limit);

  /// @brief 最大のテーブルサイズを設定する．
  void
  max_size(ymuint64 max_size);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief next_limitを更新する
  void
  update_next_limit();

  /// @brief テーブルを拡張すべき時には true を返す．
  bool
  check_tablesize() const;

  // BddMgr からメモリを確保する．
  void*
  allocate(ymuint64 size);

  // BddMgr にメモリを返す．
  void
  deallocate(void* ptr,
	     ymuint64 size);

  // ログ出力用のストリームを得る．
  ostream&
  logstream() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 使用されているセルの数
  ymuint64 mUsedNum;

  // テーブルサイズ
  // セルの個数．バイト数ではないので注意
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

  // 親の BddMgr
  BddMgrClassic* mMgr;

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

// e の参照回数が0なら true を返す．
inline
bool
CompTbl::check_noref(BddEdge e)
{
  BddNode* vp = e.get_node();
  return vp && vp->noref();
}

END_NAMESPACE_YM_BDD

#endif // COMPTBL_H
