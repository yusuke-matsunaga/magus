#ifndef LIBYM_MISLIB_MISLIBSTR_H
#define LIBYM_MISLIB_MISLIBSTR_H

/// @file libym_mislib/MislibStr.h
/// @brief MislibStr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibStr MislibStr.h "MislibStr.h"
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibStr :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] str 共有された文字列のID
  MislibStr(const FileRegion& loc,
	    ShString str);

  /// @brief デストラクタ
  ~MislibStr();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 文字列を取り出す
  virtual
  ShString
  str() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  LogExpr
  to_expr(const hash_map<ShString, ymuint>& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列
  ShString mStr;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBSTR_H
