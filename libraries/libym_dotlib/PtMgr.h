#ifndef LIBYM_DOTLIB_PTMGR_H
#define LIBYM_DOTLIB_PTMGR_H

/// @file libym_dotlib/PtMgr.h
/// @brief PtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_int.h"
#include "ym_utils/ShString.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtMgr PtMgr.h "PtMgr.h"
/// @brief PtNode を管理するためのクラス
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
  /// @note 以前，生成されたオブジェクトは破壊される．
  void
  clear();

  /// @brief 整数値を表す PtNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtNodeImpl*
  new_int(int value,
	  const FileRegion& loc);

  /// @brief 実数値を表す PtNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtNodeImpl*
  new_float(double value,
	    const FileRegion& loc);

  /// @brief 文字列シンボルを表す PtNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtNodeImpl*
  new_string(ShString value,
	     const FileRegion& loc);

  /// @brief + 演算子を表す PtNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtNodeImpl*
  new_plus(const DotlibNode* opr1,
	   const DotlibNode* opr2);

  /// @brief - 演算子を表す PtNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtNodeImpl*
  new_minus(const DotlibNode* opr1,
	    const DotlibNode* opr2);

  /// @brief * 演算子を表す PtNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtNodeImpl*
  new_mult(const DotlibNode* opr1,
	   const DotlibNode* opr2);

  /// @brief / 演算子を表す PtNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  PtNodeImpl*
  new_div(const DotlibNode* opr1,
	  const DotlibNode* opr2);

  /// @brief リストを表す PtNode を生成する．
  /// @note 空のリストが返される．
  PtNodeImpl*
  new_list();

  /// @brief グループを表す PtNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtNodeImpl*
  new_group(const DotlibNode* value,
	    const FileRegion& loc);

  /// @brief PtAttr を生成する．
  PtNodeImpl*
  new_attr(const ShString& attr_name,
	   const DotlibNode* value,
	   const FileRegion& loc);

  /// @brief 使用メモリ量の一覧を出力する．
  /// @param[in] s 出力先のストリーム
  void
  show_stats(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 生成したノードをリンクにつなぐ．
  void
  add_node(PtNodeImpl* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // ここで確保したオブジェクトのリストの先頭
  // PtNodeImpl::mClearLink をつかってリンクとリストを構成する．
  PtNodeImpl* mTop;

  // 個々の要素の使用数
  ymuint32 mIntNum;
  ymuint32 mFloatNum;
  ymuint32 mStrNum;
  ymuint32 mOprNum;
  ymuint32 mListNum;
  ymuint32 mGroupNum;
  ymuint32 mAttrNum;
};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_PTMGR_H
