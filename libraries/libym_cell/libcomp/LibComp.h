#ifndef LIBCOMP_H
#define LIBCOMP_H

/// @file LibComp.h
/// @brief LibComp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "LdLogicMgr.h"
#include "LdFFMgr.h"
#include "LdPatMgr.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LibComp LibComp.h "LibComp.h"
/// @brief パタングラフの情報をダンプするためのクラス
//////////////////////////////////////////////////////////////////////
class LibComp
{
public:

  /// @brief コンストラクタ
  LibComp();

  /// @brief デストラクタ
  ~LibComp();


public:

  /// @brief グラフ構造全体の内容を表示する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] library 対象のセルライブラリ
  void
  display(ostream& s,
	  const CellLibrary& library);

  /// @relates LdFuncMgr
  /// @brief グラフ構造全体をダンプする．
  /// @param[in] s 出力先のストリーム
  /// @param[in] library 対象のセルライブラリ
  /// @note ダンプされた情報はそのまま PatMgr で読み込むことができる．
  void
  dump(ostream& s,
       const CellLibrary& library);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ライブラリの情報からパタンを生成する．
  /// @param[in] library 対象のセルライブラリ
  void
  gen_pat(const CellLibrary& library);

  /// @brief expr から生成されるパタンを登録する．
  void
  reg_expr(const LogExpr& expr);

  /// @brief 論理式から生成されるパタンを登録する．
  /// @param[in] pgfunc この式に対応する関数情報
  /// @param[in] expr パタンの元となる論理式
  void
  reg_pat(LdGroup* pgfunc,
	  const LogExpr& expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数情報を管理するオブジェクト
  LdLogicMgr mLdLogicMgr;

  // FFの情報を管理するオブジェクト
  LdFFMgr mLdFFMgr;

  // パタングラフを管理するオブジェクト
  LdPatMgr mLdPatMgr;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LIBCOMP_H
