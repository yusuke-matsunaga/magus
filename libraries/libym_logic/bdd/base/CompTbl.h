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

class BddMgrImpl;

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
  ~CompTbl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられるインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  string
  name() const;

  /// @brief 使用されているセル数を返す．
  ymuint64
  used_num() const;

  /// @brief テーブルサイズを返す．
  ymuint64
  table_size() const;

  /// @brief 最大のテーブルサイズを設定する．
  void
  max_size(ymuint64 max_size);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルサイズを設定する．
  /// @param[in] new_size 設定する新しいサイズ
  void
  set_table_size(ymuint64 new_size);

  /// @brief テーブルを拡張すべき時には true を返す．
  /// @param[in] num 使用中のセル数
  bool
  check_tablesize(ymuint64 num) const;

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

  // 親の BddMgr
  BddMgrImpl* mMgr;

  // ほとんどデバッグ用の名前
  string mName;

  // テーブルサイズ
  // セルの個数．バイト数ではないので注意
  ymuint64 mTableSize;

  // ただし，テーブルのサイズはこれ以上大きくしない．
  ymuint64 mMaxSize;

  // mUsedNumがこの値を越えたらテーブルを拡張する
  ymuint64 mNextLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前を返す．
inline
string
CompTbl::name() const
{
  return mName;
}

// @brief テーブルサイズを返す．
inline
ymuint64
CompTbl::table_size() const
{
  return mTableSize;
}

// テーブルを拡張すべき時には true を返す．
inline
bool
CompTbl::check_tablesize(ymuint64 num) const
{
  return num > mNextLimit && mTableSize < mMaxSize;
}

END_NAMESPACE_YM_BDD

#endif // COMPTBL_H
