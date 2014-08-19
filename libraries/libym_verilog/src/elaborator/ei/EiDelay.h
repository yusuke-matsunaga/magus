#ifndef EIDELAY_H
#define EIDELAY_H

/// @file EiDelay.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDelay.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDelay EiDelay.h "EiDelay.h"
/// @brief ElbDelay の実装クラス
//////////////////////////////////////////////////////////////////////
class EiDelay :
  public ElbDelay
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] elem_num 要素数
  /// @param[in] expr_list 式の配列
  EiDelay(const PtBase* pt_obj,
	  ymuint elem_num,
	  ElbExpr** expr_list);

  /// @brief デストラクタ
  virtual
  ~EiDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDelay の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const VlExpr*
  expr(ymuint pos) const;

  /// @brief Verilog-HDL の文字列を得る．
  string
  decompile() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtBase* mPtObj;

  // 要素数
  ymuint32 mElemNum;

  // 要素の配列
  ElbExpr** mElemArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDELAY_H
