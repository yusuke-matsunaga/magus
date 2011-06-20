#ifndef LIBYM_MISLIB_MISLIBNUM_H
#define LIBYM_MISLIB_MISLIBNUM_H

/// @file libym_mislib/MislibNum.h
/// @brief MislibNum のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl1.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibNum MislibNum.h "MislibNum.h"
/// @brief 数値を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibNum :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] num 数値
  MislibNum(const FileRegion& loc,
	    double num);

  /// @brief デストラクタ
  ~MislibNum();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 数値を取り出す
  virtual
  double
  num() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 数値
  double mNum;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBNUM_H
