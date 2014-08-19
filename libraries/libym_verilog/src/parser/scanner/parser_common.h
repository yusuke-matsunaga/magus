#ifndef PARSER_COMMON_H
#define PARSER_COMMON_H

/// @file parser_common.h
/// @brief YACC の生成したヘッダファイルをインクルードする為のファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/pt/PtP.h"
#include "PtiFwd.h"
#include "PtrList.h"
#include "Parser.h"


BEGIN_NAMESPACE_YM_VERILOG

class PuHierName;

#include "verilog_grammer.hh"

END_NAMESPACE_YM_VERILOG

#endif // PARSER_COMMON_H
