#ifndef LIBYM_CELL_MISLIB_MISLIBMGR_H
#define LIBYM_CELL_MISLIB_MISLIBMGR_H

/// @file libym_cell/mislib/MislibMgr.h
/// @brief MislibMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ShString.h"

#include "MislibPt.h"
#include "MislibLex.h"


BEGIN_NAMESPACE_YM_CELL_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibMgr MislibMgr.h "MislibMgr.h"
/// @brief MislibPt から CellLibrary を生成するクラス
//////////////////////////////////////////////////////////////////////
class MislibMgr
{
public:

  /// @brief コンストラクタ
  MislibMgr();

  /// @brief デストラクタ
  ~MislibMgr();


public:

  /// @brief MislibPt から CellLibrary を生成する．
  /// @param[in] lib_name ライブラリ名
  /// @param[in] root パース木のルート
  /// @return 生成したライブラリを返す．
  /// @note 生成が失敗したら NULL を返す．
  const CellLibrary*
  gen_library(const string& lib_name,
	      const MislibPt* root);

  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセージマネージャの取得
inline
MsgMgr&
MislibMgr::msg_mgr()
{
  return mMsgMgr;
}

END_NAMESPACE_YM_CELL_MISLIB

#endif // LIBYM_CELL_MISLIB_MISLIBMGR_H
