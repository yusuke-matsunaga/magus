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

  /// @brief simple attribute を表す PtNode を生成する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  PtNode*
  new_ptsimple(ShString attr_name,
	       const FileRegion& attr_loc,
	       const PtValue* value);

  /// @brief complex attribute を表す PtNode を生成する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値のリスト
  PtNode*
  new_ptcomplex(ShString attr_name,
		const FileRegion& attr_loc,
		const vector<const PtValue*>& value_list);

  /// @brief group statement を表す PtNode を生成する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値のリスト
  PtNode*
  new_ptgroup(ShString attr_name,
	      const FileRegion& attr_loc,
	      const vector<const PtValue*>& value_list);

  /// @brief PtCell を生成する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  PtCell*
  new_ptcell(ShString attr_name,
	     const FileRegion& attr_loc,
	     const PtValue* value);

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

  /// @brief 演算子を表す PtValue を生成する．
  /// @param[in] type 型
  /// @param[in] opr1, opr2 オペランド
  PtValue*
  new_opr(tTokenType type,
	  PtValue* opr1,
	  PtValue* opr2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTMGR_H
