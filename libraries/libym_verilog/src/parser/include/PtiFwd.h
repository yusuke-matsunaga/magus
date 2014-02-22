#ifndef PTIFWD_H
#define PTIFWD_H

/// @file PtiFwd.h
/// @brief Pti クラスの名前の宣言のみおこなうヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/verilog.h"
#include "verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in PtiFactory.h
class PtiFactory;

// in PtiDecl.h
class PtiPort;
class PtiPortRef;
class PtiIOHead;
class PtiDeclHead;

typedef PtArray<PtiPort> PtiPortArray;

END_NAMESPACE_YM_VERILOG

#endif // PTIFWD_H
