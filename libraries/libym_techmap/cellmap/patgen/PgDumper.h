#ifndef LIBYM_TECHMAP_PATGEN_PGDUMPER_H
#define LIBYM_TECHMAP_PATGEN_PGDUMPER_H

/// @file libym_techmap/patgen/PgDumper.h
/// @brief PgDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "PgFuncMgr.h"
#include "PatGen.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP_PATGEN

//////////////////////////////////////////////////////////////////////
/// @class PgDumper PgDumper.h "PgDumper.h"
/// @brief パタングラフの情報をダンプするためのクラス
//////////////////////////////////////////////////////////////////////
class PgDumper
{
public:

  /// @brief コンストラクタ
  PgDumper();

  /// @brief デストラクタ
  ~PgDumper();


public:

  /// @brief ライブラリの情報からパタンを生成する．
  /// @param[in] library 対象のセルライブラリ
  void
  gen_pat(const CellLibrary& library);

  /// @brief グラフ構造全体の内容を表示する．
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s);

  /// @relates PgFuncMgr
  /// @brief グラフ構造全体をダンプする．
  /// @param[in] s 出力先のストリーム
  /// @note ダンプされた情報はそのまま PatMgr で読み込むことができる．
  void
  dump(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief expr から生成されるパタンを登録する．
  void
  reg_expr(const LogExpr& expr);

  /// @brief 論理式から生成されるパタンを登録する．
  /// @param[in] pgfunc この式に対応する関数情報
  /// @param[in] expr パタンの元となる論理式
  void
  reg_pat(PgFunc* pgfunc,
	  const LogExpr& expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数情報を管理するオブジェクト
  PgFuncMgr mPgfMgr;

  // パタングラフを管理するオブジェクト
  PatGen mPatGen;

};

END_NAMESPACE_YM_CELLMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGDUMPER_H
