#ifndef LIBYM_MISLIB_MISLIBMGR_H
#define LIBYM_MISLIB_MISLIBMGR_H

/// @file libym_mislib/MislibMgr.h
/// @brief MislibMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"


BEGIN_NAMESPACE_YM_MISLIB

class MislibMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class MislibMgr MislibMgr.h "ym_mislib/MislibMgr.h"
/// @brief MislibNode を管理するクラス
//////////////////////////////////////////////////////////////////////
class MislibMgr
{
  friend class MislibParser;

public:

  /// @brief コンストラクタ
  MislibMgr();

  /// @brief デストラクタ
  ~MislibMgr();


public:

  /// @brief 確保したオブジェクトをすべて削除する．
  void
  clear();

  /// @brief ゲートのリストを返す．
  const MislibNode*
  gate_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  MislibMgrImpl* mImpl;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBMGR_H
