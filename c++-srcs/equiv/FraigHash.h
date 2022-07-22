﻿#ifndef FRAIGHASH_H
#define FRAIGHASH_H

/// @file FraigHash.h
/// @brief FraigHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "fraig_nsdef.h"


BEGIN_NAMESPACE_FRAIG

class FraigNode;

//////////////////////////////////////////////////////////////////////
/// @class FraigHash FraigHash.h "FraigHash.h"
/// @brief FraigNode のハッシュ表を実装するためのクラス
//////////////////////////////////////////////////////////////////////
class FraigHash
{
public:

  /// @brief コンストラクタ
  FraigHash();

  /// @brief デストラクタ
  ~FraigHash();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  ///
  /// テーブルサイズはそのまま
  void
  clear();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size サイズ
  void
  alloc_table(int req_size);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  int mNum;

  // mHashTable のサイズ
  int mHashSize;

  // ハッシュ表を拡大する目安
  int mNextLimit;

  // ハッシュ表
  FraigNode** mTable;

};

END_NAMESPACE_FRAIG

#endif // FRAIGHASH_H
