#ifndef CIPINHASH_H
#define CIPINHASH_H

/// @file CiPinHash.h
/// @brief CiPinHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

class CiCell;
class CiPin;

//////////////////////////////////////////////////////////////////////
/// @class CiPinHash CiPinHash.h "CiPinHash.h"
/// @brief ピン名のハッシュ表
//////////////////////////////////////////////////////////////////////
class CiPinHash
{
public:

  /// @brief コンストラクタ
  CiPinHash();

  /// @brief デストラクタ
  ~CiPinHash();


public:

  /// @brief ピンを追加する．
  /// @param[in] pin 追加するピン
  void
  add(CiPin* pin);

  /// @brief ピンを取り出す．
  /// @param[in] cell セル
  /// @param[in] name 名前
  /// @return cell の name というピンを返す．
  /// @note なければ NULL を返す．
  CiPin*
  get(const CiCell* cell,
      ShString name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルの領域を確保する．
  /// @param[in] req_size 要求するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テーブルサイズ
  ymuint32 mSize;

  // ハッシュ表
  CiPin** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_CELL

#endif // CIPINHASH_H
