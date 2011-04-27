#ifndef LIBYM_CELL_DOTLIB_PTMGR_H
#define LIBYM_CELL_DOTLIB_PTMGR_H

/// @file libym_cell/dotlib/PtMgr.h
/// @brief PtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtMgr PtMgr.h "PtMgr.h"
/// @brief PtNode, PtValue を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class PtMgr
{
public:

  /// @brief コンストラクタ
  PtMgr();

  /// @brief デストラクタ
  ~PtMgr();


public:

  /// @brief 初期化する．
  /// @note 以前，生成されたオブジェクトは(デストラクタ抜きで)破壊される．
  void
  init();

#if 0
  /// @brief simple attribute を表す PtNode を生成する．
  /// @param[in] value 値
  PtNode*
  new_ptsimple(const PtValue* value);

  /// @brief complex attribute を表す PtNode を生成する．
  /// @param[in] value_list 値のリスト
  PtNode*
  new_ptcomplex(const vector<const PtValue*>& value_list);

  /// @brief group statement を表す PtNode を生成する．
  /// @param[in] value_list 値のリスト
  PtNode*
  new_ptgroup(const vector<const PtValue*>& value_list);
#endif

  /// @brief PtLibrary を生成する．
  /// @param[in] name ライブラリ名
  PtLibrary*
  new_ptlibrary(const ShString& name);

  /// @brief PtCell を生成する．
  /// @param[in] name セル名
  PtCell*
  new_ptcell(const ShString& name);

  /// @brief PtLeakagePower を生成する．
  PtLeakagePower*
  new_ptleakage_power();

  /// @brief PtBus を生成する．
  /// @param[in] name バス名
  PtBus*
  new_ptbus(const ShString& name);

  /// @brief PtBundle を生成する．
  /// @param[in] name バンドル名
  PtBundle*
  new_ptbundle(const ShString& name);

  /// @brief PtPin を生成する．
  /// @param[in] name ピン名
  PtPin*
  new_ptpin(const ShString& name);

  /// @brief PtTiming を生成する．
  PtTiming*
  new_pttiming();

  /// @brief PtTable を生成する．
  /// @param[in] name テーブルのテンプレート名
  PtTable*
  new_pttable(const ShString& name);

  /// @brief 整数値を表す PtValue を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtValue*
  new_int(int value,
	  const FileRegion& loc);

  /// @brief 実数値を表す PtValue を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtValue*
  new_float(double value,
	    const FileRegion& loc);

  /// @brief 文字列シンボルを表す PtValue を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtValue*
  new_string(ShString value,
	     const FileRegion& loc);

  /// @brief + 演算子を表す PtValue を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtValue*
  new_plus(PtValue* opr1,
	   PtValue* opr2);

  /// @brief - 演算子を表す PtValue を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtValue*
  new_minus(PtValue* opr1,
	    PtValue* opr2);

  /// @brief * 演算子を表す PtValue を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtValue*
  new_mult(PtValue* opr1,
	   PtValue* opr2);

  /// @brief / 演算子を表す PtValue を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtValue*
  new_div(PtValue* opr1,
	  PtValue* opr2);

  /// @brief リストを表す PtValue を生成する．
  /// @param[in] top 先頭の要素
  PtValue*
  new_list(PtValue* top);

  /// @brief 使用メモリ量の一覧を出力する．
  /// @param[in] s 出力先のストリーム
  void
  show_stats(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 個々の要素の使用数
  ymuint32 mSimpleNum;
  ymuint32 mComplexNum;
  ymuint32 mGroupNum;
  ymuint32 mCellNum;
  ymuint32 mLeakagePowerNum;
  ymuint32 mBusNum;
  ymuint32 mBundleNum;
  ymuint32 mPinNum;
  ymuint32 mTimingNum;
  ymuint32 mTableNum;
  ymuint32 mIntNum;
  ymuint32 mFloatNum;
  ymuint32 mStrNum;
  ymuint32 mOprNum;
  ymuint32 mListNum;
};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTMGR_H
