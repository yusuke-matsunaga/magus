#ifndef ELBFWD_H
#define ELBFWD_H

/// @file ElbFwd.h
/// @brief ElbObj の派生クラスの前方宣言ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

// in ElbScopeHandle.h
class ElbScopeHandle;

// in ElbModule.h
class ElbModule;
class ElbModuleArray;

// in ElbPort.h
class ElbPort;

// in ElbIODecl.h
class ElbIOHead;
class ElbIODecl;

// in ElbDecl.h
class ElbDeclHead;
class ElbDecl;
class ElbDeclArray;

// in ElbParameter.h
class ElbParamHead;
class ElbParameter;

// in ElbParamAssign.h
class ElbParamAssign;
class ElbDefParam;

// in ElbContAssign.h
class ElbCaHead;
class ElbContAssign;

// in ElbTaskFunc.h
class ElbTaskFunc;

// in ElbPrimitive.h
class ElbPrimHead;
class ElbPrimitive;
class ElbPrimArray;
class ElbPrimTerm;

// in ElbProcess.h
class ElbProcess;

// in ElbScope.h
class ElbScope;

// in ElbStmt.h
class ElbStmt;
class ElbCaseItem;

// in ElbControl.h
class ElbControl;

// in ElbExpr.h
class ElbExpr;

// in ElbDelay.h
class ElbDelay;

// in ElbRange.h
class ElbRange;
class ElbRangeSrc;

// in ElbUdp.h
class ElbUdpDefn;
class ElbTableEntry;

// in ElbUserSystf.h
class ElbUserSystf;

// in ElbGfRoot.h
class ElbGfRoot;

// in ElbGenvar.h
class ElbGenvar;

// in ElbAttribute.h
class ElbAttribute;
class ElbAttrList;
class ElbAttrIter;

END_NAMESPACE_YM_VERILOG

#endif // ELBFWD_H
