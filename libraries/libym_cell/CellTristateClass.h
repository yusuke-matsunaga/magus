#ifndef CELLTRISTATECLASS_H
#define CELLTRISTATECLASS_H

/// @file CellTristateClass.h
/// @brief CellTristateClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellClassBase.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellTristateClass CellTristateClass.h "CellTristateClass.h"
/// @brief 論理セルのクラス
//////////////////////////////////////////////////////////////////////
class CellTristateClass :
  public CellClassBase
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellTristateClass();

  /// @brief デストラクタ
  ~CellTristateClass();


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
  TvFunc
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

#endif // CELLTRISTATECLASS_H
