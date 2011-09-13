#ifndef LIBCOMP_H
#define LIBCOMP_H

/// @file LibComp.h
/// @brief LibComp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "LcLogicMgr.h"
#include "LcFFMgr.h"
#include "LcLatchMgr.h"
#include "LcPatMgr.h"

#include "ym_utils/BinIO.h"


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

  /// @brief セルのグループ化，クラス化を行う．
  void
  compile(const CellLibrary& library);

  /// @brief グラフ構造全体の内容を表示する．
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s);

  /// @brief グラフ構造全体をダンプする．
  /// @param[in] bos 出力先のストリーム
  /// @note ダンプされた情報はそのまま PatMgr で読み込むことができる．
  void
  dump(BinO& bos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief expr から生成されるパタンを登録する．
  void
  reg_expr(const LogExpr& expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数情報を管理するオブジェクト
  LcLogicMgr mLogicMgr;

  // FFの情報を管理するオブジェクト
  LcFFMgr mFFMgr;

  // ラッチの情報を管理するオブジェクト
  LcLatchMgr mLatchMgr;

  // パタングラフを管理するオブジェクト
  LcPatMgr mPatMgr;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LIBCOMP_H
