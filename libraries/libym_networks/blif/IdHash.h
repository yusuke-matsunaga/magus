#ifndef IDHASH_H
#define IDHASH_H

/// @file IdHash.h
/// @brief IdHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/blif_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

class IdCell;

//////////////////////////////////////////////////////////////////////
/// @class IdHash IdHash.h "IdHash.h"
/// @brief 識別子を管理するハッシュ表
/// 同一の文字列に対しては同じ ID 番号を返す．
/// 文字列領域はこのクラスが管理する．
/// また，この文字列を定義している場所と参照している場所の位置情報も
/// 持つ．
//////////////////////////////////////////////////////////////////////
class IdHash
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  IdHash();

  /// @brief デストラクタ
  ~IdHash();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 識別子に対応するセルを探す．
  /// @param[in] str 文字列
  /// @param[in] create 存在しないときに新規生成するなら true
  /// @return 対応する IdCell を返す．
  IdCell*
  find(const char* str,
       bool create);

  /// @brief 登録されている要素数を返す．(= ID番号の最大値 + 1)
  ymuint32
  num() const;

  /// @brief ID 番号に対応する IdCell を得る．
  IdCell*
  cell(ymuint32 id) const;

  /// @brief ID 番号から文字列を得る．
  const char*
  str(ymuint32 id) const;

  /// @brief ID番号から位置情報を得る．
  const FileRegion&
  loc(ymuint32 id) const;

  /// @brief ID番号からその識別子を定義している位置情報を返す．
  const FileRegion&
  def_loc(ymuint32 id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表を拡大する．
  void
  alloc_table(ymuint32 new_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IdCell用のアロケータ
  SimpleAlloc mAlloc;

  // テーブルサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  IdCell** mTable;

  // IDをキーにして IdCell を格納する配列
  vector<IdCell*> mCellArray;

  // ハッシュ表を拡張する目安
  ymuint32 mNextLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 登録されている要素数を返す．(= ID番号の最大値 + 1)
inline
ymuint32
IdHash::num() const
{
  return mCellArray.size();
}

// @brief ID 番号に対応する IdCell を得る．
inline
IdCell*
IdHash::cell(ymuint32 id) const
{
  return mCellArray[id];
}

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // IDHASH_H
