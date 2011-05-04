#ifndef MAGUS_CORE_READBLIF_H
#define MAGUS_CORE_READBLIF_H

/// @file src/core/ReadBlif.h
/// @brief ReadBlif のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIoCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "NetCmd.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bdn/BlifBdnConv.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ReadBlif ReadBlif.h "ReadBlif.h"
/// @brief blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlif :
  public NetCmd
{
public:

  /// @brief コンストラクタ
  ReadBlif(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadBlif();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Blif ファイルを読み込むためのパーサ
  BNetBlifReader mReader;

  // blif から Bdn に変換する関数
  BlifBdnConv mBlifBdnConv;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_CORE_READBLIF_H
