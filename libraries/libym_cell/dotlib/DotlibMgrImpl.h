#ifndef LIBYM_DOTLIB_DOTLIBMGRIMPL_H
#define LIBYM_DOTLIB_DOTLIBMGRIMPL_H

/// @file libym_dotlib/DotlibMgrImpl.h
/// @brief DotlibMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_int.h"
#include "ym_utils/ShString.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibMgrImpl DotlibMgrImpl.h "DotlibMgrImpl.h"
/// @brief DotlibMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class DotlibMgrImpl
{
public:

  /// @brief コンストラクタ
  DotlibMgrImpl();

  /// @brief デストラクタ
  ~DotlibMgrImpl();


public:

  /// @brief 初期化する．
  /// @note 以前，生成されたオブジェクトは破壊される．
  void
  clear();

  /// @brief 整数値を表す DotlibNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibNodeImpl*
  new_int(int value,
	  const FileRegion& loc);

  /// @brief 実数値を表す DotlibNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibNodeImpl*
  new_float(double value,
	    const FileRegion& loc);

  /// @brief 文字列シンボルを表す DotlibNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibNodeImpl*
  new_string(ShString value,
	     const FileRegion& loc);

  /// @brief + 演算子を表す DotlibNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  DotlibNodeImpl*
  new_plus(const DotlibNode* opr1,
	   const DotlibNode* opr2);

  /// @brief - 演算子を表す DotlibNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  DotlibNodeImpl*
  new_minus(const DotlibNode* opr1,
	    const DotlibNode* opr2);

  /// @brief * 演算子を表す DotlibNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  DotlibNodeImpl*
  new_mult(const DotlibNode* opr1,
	   const DotlibNode* opr2);

  /// @brief / 演算子を表す DotlibNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  DotlibNodeImpl*
  new_div(const DotlibNode* opr1,
	  const DotlibNode* opr2);

  /// @brief NOT 演算子を表す DotlibNode を生成する．
  /// @param[in] opr オペランド
  /// @param[in] loc ファイル上の位置
  DotlibNodeImpl*
  new_not(const DotlibNode* opr,
	  const FileRegion& loc);

  /// @brief AND 演算子を表す DotlibNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  DotlibNodeImpl*
  new_and(const DotlibNode* opr1,
	  const DotlibNode* opr2);

  /// @brief OR 演算子を表す DotlibNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  DotlibNodeImpl*
  new_or(const DotlibNode* opr1,
	 const DotlibNode* opr2);

  /// @brief XOR 演算子を表す DotlibNode を生成する．
  /// @param[in] opr1, opr2 オペランド
  DotlibNodeImpl*
  new_xor(const DotlibNode* opr1,
	  const DotlibNode* opr2);

  /// @brief リストを表す DotlibNode を生成する．
  /// @note 空のリストが返される．
  DotlibNodeImpl*
  new_list();

  /// @brief グループを表す DotlibNode を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibNodeImpl*
  new_group(const DotlibNode* value,
	    const FileRegion& loc);

  /// @brief DotlibAttr を生成する．
  DotlibNodeImpl*
  new_attr(const ShString& attr_name,
	   const DotlibNode* value,
	   const FileRegion& loc);

  /// @brief 根のノードを設定する．
  void
  set_root_node(DotlibNodeImpl* root);

  /// @brief 根のノードを返す．
  const DotlibNode*
  root_node() const;

  /// @brief 使用メモリ量の一覧を出力する．
  /// @param[in] s 出力先のストリーム
  void
  show_stats(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 根のノード
  DotlibNodeImpl* mRoot;

  // 個々の要素の使用数
  ymuint32 mIntNum;
  ymuint32 mFloatNum;
  ymuint32 mStrNum;
  ymuint32 mOprNum;
  ymuint32 mNotNum;
  ymuint32 mListNum;
  ymuint32 mGroupNum;
  ymuint32 mAttrNum;
};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_DOTLIBMGRIMPL_H
