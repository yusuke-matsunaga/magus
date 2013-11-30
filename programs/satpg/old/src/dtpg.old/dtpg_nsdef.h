#ifndef DTPG_NSDEF_H
#define DTPG_NSDEF_H

/// @file dtpg_nsdef.h
/// @brief Dtpg 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_DTPG \
BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE(nsDtpgOld)

#define END_NAMESPACE_YM_SATPG_DTPG \
END_NAMESPACE(nsDtpgOld) \
END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_DTPG

class DtpgSat;
class DtpgNetwork;
class DtpgNode;
class DtpgPrimitive;
class DtpgFFR;
class DtpgFault;

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPG_NSDEF_H
