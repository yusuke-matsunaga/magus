#ifndef MAGUS_LUTMAP_AREAMAP2CMD_H
#define MAGUS_LUTMAP_AREAMAP2CMD_H

/// @file magus/lutmap/AreaMapCmd.h
/// @brief AreaMap2Cmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class AreaMap2Cmd LutmapCmd "LutmapCmd.h"
/// @brief area map コマンド
//////////////////////////////////////////////////////////////////////
class AreaMap2Cmd :
  public LutmapCmd
{
public:

  /// @brief コンストラクタ
  AreaMap2Cmd(MagMgr* mgr,
	      LutmapData* data);

  /// @brief デストラクタ
  virtual
  ~AreaMap2Cmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // method オプションの解析用オブジェクト
  TclPoptStr* mPoptMethod;

  // resub オプションの解析用オブジェクト
  TclPopt* mPoptResub;

  // count オプションの解析用オブジェクト
  TclPoptInt* mPoptCount;

  // verbose オプションの解析用オブジェクト
  TclPopt* mPoptVerbose;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LUTMAP_AREAMAP2CMD_H
