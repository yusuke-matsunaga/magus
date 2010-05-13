#ifndef YM_VERILOG_VL_VLFWD_H
#define YM_VERILOG_VL_VLFWD_H

/// @file ym_verilog/vl/VlFwd.h
/// @brief VlObj の派生クラスの前方宣言ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlFwd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>


BEGIN_NAMESPACE_YM_VERILOG

// in VlObj.h
class VlObj;

// in VlNamedObj.h
class VlNamedObj;

// in VlScope.h
class VlScope;

// in VlDecl.h
class VlDecl;

// in VlModule.h
class VlModule;
class VlModuleArray;

// in VlPort.h
class VlPort;

// in VlIODecl.h
class VlIODecl;

// in VlParamAssign.h
class VlParamAssign;
class VlDefParam;

// in VlContAssign.h
class VlContAssign;

// in VlTaskFunc.h
class VlFunction;
class VlTask;

// in VlPrimitive.h
class VlPrimitive;
class VlPrimArray;
class VlPrimTerm;

// in VlProcess.h
class VlProcess;

// in VlStmt.h
class VlStmt;
class VlCaseItem;

// in VlControl.h
class VlControl;

// in VlExpr.h
class VlExpr;

// in VlDelay.h
class VlDelay;

// in VlRange.h
class VlRange;

// in VlUdp.h
class VlUdpDefn;
class VlTableEntry;

// in VlUserSystf.h
class VlUserSystf;

// in VlAttribute.h
class VlAttribute;
class VlAttrList;

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLFWD_H
