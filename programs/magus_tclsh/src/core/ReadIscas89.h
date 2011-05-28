#ifndef MAGUS_CORE_READISCAS89_H
#define MAGUS_CORE_READISCAS89_H

/// @file src/core/ReadIscas89.h
/// @brief ReadIscas89 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIoCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "NetCmd.h"

#include "ym_networks/BNetIscas89Reader.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ReadIscas89 BNetIoCmd.h "BNetIoCmd.h"
/// @brief iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89 :
  public NetCmd
{
public:

  /// @brief コンストラクタ
  ReadIscas89(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadIscas89();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // iscas89 ファイルを読み込むためのパーサ
  BNetIscas89Reader mReader;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_CORE_READISCAS89_H
