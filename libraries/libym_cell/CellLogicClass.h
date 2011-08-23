#ifndef CELLLOGICCLASS_H
#define CELLLOGICCLASS_H

/// @file CellLogicClass.h
/// @brief CellLogicClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellClassBase.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLogicClass CellLogicClass.h "CellLogicClass.h"
/// @brief 論理セルのクラス
//////////////////////////////////////////////////////////////////////
class CellLogicClass :
  public CellClassBase
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellLogicClass();

  /// @brief デストラクタ
  virtual
  ~CellLogicClass();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理セルの場合に true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief 論理セルの場合に出力ピン数を返す．
  virtual
  ymuint
  output_num() const;

  /// @brief 論理セルの場合に関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 非論理セルの場合の返り値は不定
  virtual
  const TvFunc&
  logic_function(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力数
  ymuint32 mNo;

  // 論理関数の配列
  TvFunc* mLogicArray;

};

END_NAMESPACE_YM_CELL

#endif // CELLLOGICCLASS_H
