#ifndef LIBYM_VERILOG_PARSER_INCLUDE_PTIDECL_H
#define LIBYM_VERILOG_PARSER_INCLUDE_PTIDECL_H

/// @file libym_verilog/parser/include/PtiDecl.h
/// @brief 宣言要素関係の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtiDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtPort.h"
#include "ym_verilog/pt/PtDecl.h"
#include "PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtiPort PtiDecle.h "PtiDecl.h"
/// @brief port を表すクラス
//////////////////////////////////////////////////////////////////////
class PtiPort :
  public PtPort
{
public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  /// @param[in] dir 方向
  virtual
  void
  _set_portref_dir(ymuint pos,
		   tVpiDirection dir) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtiIOHead PtiDecl.h "PtiDecl.h"
/// @brief IO宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class PtiIOHead :
  public PtIOHead
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtiIOHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素の配列
  virtual
  void
  set_elem(PtIOItemArray elem_array) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtiDeclHead PtiDecl.h "PtiDecl.h"
/// @brief 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class PtiDeclHead :
  public PtDeclHead
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtiDeclHead の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素リストの設定
  /// @param[in] elem_array 要素の配列
  virtual
  void
  set_elem(PtDeclItemArray elem_array) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_INCLUDE_PTIDECL_H
