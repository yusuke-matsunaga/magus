#ifndef IMPHASH_H
#define IMPHASH_H

/// @file ImpHash.h
/// @brief ImpHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"
#include "YmUtils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpCell;

//////////////////////////////////////////////////////////////////////
/// @class ImpHash ImpHash.h "ImpHash.h"
/// @brief 含意情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpHash
{
public:

  /// @brief 空のコンストラクタ
  ImpHash();

  /// @brief コピーコンストラクタ
  ImpHash(const ImpHash& src);

  /// @brief デストラクタ
  ~ImpHash();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 該当する含意情報が含まれているか調べる．
  bool
  check(ymuint src_id,
	ymuint src_val,
	ymuint dst_id,
	ymuint dst_val) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 含意情報を追加する．
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値 ( 0 or 1 )
  /// @param[in] dst_id 含意先のノード番号
  /// @param[in] dst_val 含意先の値 ( 0 or 1 )
  void
  put(ymuint src_id,
      ymuint src_val,
      ymuint dst_id,
      ymuint dst_val);

  /// @brief 要素数のヒントを与える．
  void
  reserve(ymuint size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ImpCell を確保する．
  ImpCell*
  new_cell();

  /// @brief テーブルの領域を確保する．
  void
  alloc_table(ymuint size);

  /// @brief ハッシュ関数
  ymuint
  hash_func(ymuint id1,
	    ymuint id2) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ImpCell 用のアロケータ
  UnitAlloc mAlloc;

  // ハッシュ表に含まれる要素数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mHashLimit;

  // ハッシュ表
  ImpCell** mHashTable;

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPHASH_H
