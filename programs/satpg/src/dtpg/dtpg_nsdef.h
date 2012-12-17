#ifndef DTPG_NSDEF_H
#define DTPG_NSDEF_H

/// @file dtpg_nsdef.h
/// @brief Dtpg 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dtpgsat_nsdef.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_DTPG \
BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE(nsDtpg)

#define END_NAMESPACE_YM_SATPG_DTPG \
END_NAMESPACE(nsDtpg) \
END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_DTPG

class DtpgSat;
class DtpgNetwork;
class DtpgNode;
class DtpgFFR;
class DtpgFault;
class DtpgOperator;

END_NAMESPACE_YM_SATPG_DTPG

BEGIN_NAMESPACE_YM_SATPG

using nsDtpg::DtpgSat;

END_NAMESPACE_YM_SATPG

#endif // DTPG_NSDEF_H
