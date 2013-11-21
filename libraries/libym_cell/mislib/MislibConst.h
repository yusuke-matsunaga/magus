#ifndef LIBYM_MISLIB_MISLIBCONST_H
#define LIBYM_MISLIB_MISLIBCONST_H

/// @file libym_mislib/MislibConst.h
/// @brief MislibConst0, MislibConst1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibConst0 MislibConst.h "MislibConst.h"
/// @brief 定数0を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibConst0 :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibConst0(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibConst0();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

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

};


//////////////////////////////////////////////////////////////////////
/// @class MislibConst1 MislibConst.h "MislibConst.h"
/// @brief 定数1を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibConst1 :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibConst1(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibConst1();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

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

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBCONST_H
