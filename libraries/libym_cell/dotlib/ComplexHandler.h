#ifndef LIBYM_CELL_DOTLIB_COMPLEXHANDLER_H
#define LIBYM_CELL_DOTLIB_COMPLEXHANDLER_H

/// @file libym_cell/dotlib/ComplexHandler.h
/// @brief ComplexHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class ComplexHandler DotlibHandler.h "DotlibHandler.h"
/// @brief complex attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class ComplexHandler :
  public DotlibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  ComplexHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~ComplexHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const ShString& attr_name,
	    const FileRegion& attr_loc);


protected:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値のリスト
  /// @note デフォルトの実装ではなにもしないで true を返す．
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    PtNode* value);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_COMPLEXHANDLER_H
