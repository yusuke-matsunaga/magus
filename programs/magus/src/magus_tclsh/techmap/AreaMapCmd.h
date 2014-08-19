#ifndef TECHMAP_AREAMAPCMD_H
#define TECHMAP_AREAMAPCMD_H

/// @file AreamapCmd.h
/// @brief AreamapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class AreaMapCmd TechmapCmd "TechmapCmd.h"
/// @brief area map コマンド
//////////////////////////////////////////////////////////////////////
class AreaMapCmd :
  public TechmapCmd
{
public:

  /// @brief コンストラクタ
  AreaMapCmd(MagMgr* mgr,
	     CmnMgr& cmnmgr);

  /// @brief デストラクタ
  virtual
  ~AreaMapCmd();


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

  // verbose オプションの解析用オブジェクト
  TclPopt* mPoptVerbose;

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_AREAMAPCMD_H
