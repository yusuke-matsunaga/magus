#ifndef YM_VERILOG_VERILOG_H
#define YM_VERILOG_VERILOG_H

/// @file ym_verilog/verilog.h
/// @brief libymverilog に共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: verilog.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

/// @namespace nsYm::nsVerilog
/// @brief Verilog パーサ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

/// @defgroup VlGroup Verilog パッケージ
///
/// Verilog-HDL(IEEE1364, IEEE1364-2001) フォーマットの
/// フロントエンド


#include "ymtools.h"
#include "vpi_user.h"

/// @brief ym_verilog 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_VERILOG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsVerilog)

/// @brief ym_verilog 用の名前空間の終了
#define END_NAMESPACE_YM_VERILOG \
END_NAMESPACE(nsVerilog) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class VlLineWatcher;
class VlMgr;
class VlScalarVal;
class VlTime;
class VlUdpVal;
class VlValue;
class VlValueType;


/// @defgroup VlCommon Verilog 用基本ユーティリティ
/// @ingroup VlGroup
/// @{

//////////////////////////////////////////////////////////////////////
// 列挙型の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @brief オブジェクトの型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiObjType {
  kVpiAlways          = vpiAlways,
  kVpiAssignStmt      = vpiAssignStmt,
  kVpiAssignment      = vpiAssignment,
  kVpiBegin           = vpiBegin,
  kVpiCase            = vpiCase,
  kVpiCaseItem        = vpiCaseItem,
  kVpiConstant        = vpiConstant,
  kVpiContAssign      = vpiContAssign,
  kVpiDeassign        = vpiDeassign,
  kVpiDefParam        = vpiDefParam,
  kVpiDelayControl    = vpiDelayControl,
  kVpiDisable         = vpiDisable,
  kVpiEventControl    = vpiEventControl,
  kVpiEventStmt       = vpiEventStmt,
  kVpiFor             = vpiFor,
  kVpiForce           = vpiForce,
  kVpiForever         = vpiForever,
  kVpiFork            = vpiFork,
  kVpiFuncCall        = vpiFuncCall,
  kVpiFunction        = vpiFunction,
  kVpiGate            = vpiGate,
  kVpiIf              = vpiIf,
  kVpiIfElse          = vpiIfElse,
  kVpiInitial         = vpiInitial,
  kVpiIntegerVar      = vpiIntegerVar,
  kVpiInterModPath    = vpiInterModPath,
  kVpiIterator        = vpiIterator,
  kVpiIODecl          = vpiIODecl,
  kVpiMemory          = vpiMemory,
  kVpiMemoryWord      = vpiMemoryWord,
  kVpiModPath         = vpiModPath,
  kVpiModule          = vpiModule,
  kVpiNamedBegin      = vpiNamedBegin,
  kVpiNamedEvent      = vpiNamedEvent,
  kVpiNamedFork       = vpiNamedFork,
  kVpiNet             = vpiNet,
  kVpiNetBit          = vpiNetBit,
  kVpiNullStmt        = vpiNullStmt,
  kVpiOperation       = vpiOperation,
  kVpiParamAssign     = vpiParamAssign,
  kVpiParameter       = vpiParameter,
  kVpiPartSelect      = vpiPartSelect,
  kVpiPathTerm        = vpiPathTerm,
  kVpiPort            = vpiPort,
  kVpiPortBit         = vpiPortBit,
  kVpiPrimTerm        = vpiPrimTerm,
  kVpiRealVar         = vpiRealVar,
  kVpiReg             = vpiReg,
  kVpiRegBit          = vpiRegBit,
  kVpiRelease         = vpiRelease,
  kVpiRepeat          = vpiRepeat,
  kVpiRepeatControl   = vpiRepeatControl,
  kVpiSchedEvent      = vpiSchedEvent,
  kVpiSpecParam       = vpiSpecParam,
  kVpiSwitch          = vpiSwitch,
  kVpiSysFuncCall     = vpiSysFuncCall,
  kVpiSysTaskCall     = vpiSysTaskCall,
  kVpiTableEntry      = vpiTableEntry,
  kVpiTask            = vpiTask,
  kVpiTaskCall        = vpiTaskCall,
  kVpiTchk            = vpiTchk,
  kVpiTchkTerm        = vpiTchkTerm,
  kVpiTimeVar         = vpiTimeVar,
  kVpiTimeQueue       = vpiTimeQueue,
  kVpiUdp             = vpiUdp,
  kVpiUdpDefn         = vpiUdpDefn,
  kVpiUserSystf       = vpiUserSystf,
  kVpiVarSelect       = vpiVarSelect,
  kVpiWait            = vpiWait,
  kVpiWhile           = vpiWhile,
  kVpiAttribute       = vpiAttribute,
  kVpiBitSelect       = vpiBitSelect,
  kVpiCallback        = vpiCallback,
  kVpiDelayTerm       = vpiDelayTerm,
  kVpiDelayDevice     = vpiDelayDevice,
  kVpiFrame           = vpiFrame,
  kVpiGateArray       = vpiGateArray,
  kVpiModuleArray     = vpiModuleArray,
  kVpiPrimitiveArray  = vpiPrimitiveArray,
  kVpiNetArray        = vpiNetArray,
  kVpiRange           = vpiRange,
  kVpiRegArray        = vpiRegArray,
  kVpiSwitchArray     = vpiSwitchArray,
  kVpiUdpArray        = vpiUdpArray,
  kVpiContAssignBit   = vpiContAssignBit,
  kVpiNamedEventArray = vpiNamedEventArray,
  kVpiScope           = vpiScope
};


//////////////////////////////////////////////////////////////////////
/// @brief unconnected port drive の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiUnconnDrive {
  kVpiHighZ = vpiHighZ,
  kVpiPull1 = vpiPull1,
  kVpiPull0 = vpiPull0
};


//////////////////////////////////////////////////////////////////////
/// @brief default delay Mode の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiDefDelayMode {
  kVpiDelayModeNone    = vpiDelayModeNone,
  kVpiDelayModePath    = vpiDelayModePath,
  kVpiDelayModeDistrib = vpiDelayModeDistrib,
  kVpiDelayModeUnit    = vpiDelayModeUnit,
  kVpiDelayModeZero    = vpiDelayModeZero,
  kVpiDelayModeMTM     = vpiDelayModeMTM
};


//////////////////////////////////////////////////////////////////////
/// @brief direction の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiDirection {
  kVpiInput       = vpiInput,
  kVpiOutput      = vpiOutput,
  kVpiInout       = vpiInout,
  kVpiMixedIO     = vpiMixedIO,
  kVpiNoDirection = vpiNoDirection
};


//////////////////////////////////////////////////////////////////////
/// @brief net の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiNetType {
  kVpiWire     = vpiWire,
  kVpiWand     = vpiWand,
  kVpiWor      = vpiWor,
  kVpiTri      = vpiTri,
  kVpiTri0     = vpiTri0,
  kVpiTri1     = vpiTri1,
  kVpiTriReg   = vpiTriReg,
  kVpiTriAnd   = vpiTriAnd,
  kVpiTriOr    = vpiTriOr,
  kVpiSupply1  = vpiSupply1,
  kVpiSupply0  = vpiSupply0,
  kVpiNone     = vpiNone
};


//////////////////////////////////////////////////////////////////////
/// @brief primitive の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiPrimType {
  kVpiAndPrim       = vpiAndPrim,
  kVpiNandPrim      = vpiNandPrim,
  kVpiNorPrim       = vpiNorPrim,
  kVpiOrPrim        = vpiOrPrim,
  kVpiXorPrim       = vpiXorPrim,
  kVpiXnorPrim      = vpiXnorPrim,
  kVpiBufPrim       = vpiBufPrim,
  kVpiNotPrim       = vpiNotPrim,
  kVpiBufif0Prim    = vpiBufif0Prim,
  kVpiBufif1Prim    = vpiBufif1Prim,
  kVpiNotif0Prim    = vpiNotif0Prim,
  kVpiNotif1Prim    = vpiNotif1Prim,
  kVpiNmosPrim      = vpiNmosPrim,
  kVpiPmosPrim      = vpiPmosPrim,
  kVpiCmosPrim      = vpiCmosPrim,
  kVpiRnmosPrim     = vpiRnmosPrim,
  kVpiRpmosPrim     = vpiRpmosPrim,
  kVpiRcmosPrim     = vpiRcmosPrim,
  kVpiRtranPrim     = vpiRtranPrim,
  kVpiRtranif0Prim  = vpiRtranif0Prim,
  kVpiRtranif1Prim  = vpiRtranif1Prim,
  kVpiTranPrim      = vpiTranPrim,
  kVpiTranif0Prim   = vpiTranif0Prim,
  kVpiTranif1Prim   = vpiTranif1Prim,
  kVpiPullupPrim    = vpiPullupPrim,
  kVpiPulldownPrim  = vpiPulldownPrim,
  kVpiSeqPrim       = vpiSeqPrim,
  kVpiCombPrim      = vpiCombPrim
};


//////////////////////////////////////////////////////////////////////
/// @brief module path/data path の極性
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiPolarity {
  kVpiPositive = vpiPositive,
  kVpiNegative = vpiNegative,
  kVpiUnknown  = vpiUnknown
};


//////////////////////////////////////////////////////////////////////
/// @brief module path の edge type
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiEdge {
  kVpiNoEdge  = vpiNoEdge,
  kVpiEdge01  = vpiEdge01,
  kVpiEdge10  = vpiEdge10,
  kVpiEdge0x  = vpiEdge0x,
  kVpiEdgex1  = vpiEdgex1,
  kVpiEdge1x  = vpiEdge1x,
  kVpiEdgex0  = vpiEdgex0,
  kVpiPosedge = vpiPosedge, // = (kVpiEdgex1 | kVpiEdge01 | kVpiEdge0x)
  kVpiNegedge = vpiNegedge, // = (kVpiEdgex0 | kVpiEdge10 | kVpiEdge1x)
  kVpiAnyEdge = vpiAnyEdge  // = (kVpiPosedge | kVpiNegedge)
};


//////////////////////////////////////////////////////////////////////
/// @brief path delay connection の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiPathType {
  kVpiPathFull     = vpiPathFull,
  kVpiPathParallel = vpiPathParallel
};


//////////////////////////////////////////////////////////////////////
/// @brief timing check の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiTchkType {
  kVpiSetup      = vpiSetup,
  kVpiHold       = vpiHold,
  kVpiPeriod     = vpiPeriod,
  kVpiWidth      = vpiWidth,
  kVpiSkew       = vpiSkew,
  kVpiRecovery   = vpiRecovery,
  kVpiNoChange   = vpiNoChange,
  kVpiSetupHold  = vpiSetupHold,
  kVpiFullskew   = vpiFullskew,
  kVpiRecrem     = vpiRecrem,
  kVpiRemoval    = vpiRemoval,
  kVpiTimeskew   = vpiTimeskew
};


//////////////////////////////////////////////////////////////////////
/// @brief operation の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiOpType {
  kVpiMinusOp        = vpiMinusOp,
  kVpiPlusOp         = vpiPlusOp,
  kVpiNotOp          = vpiNotOp,
  kVpiBitNegOp       = vpiBitNegOp,
  kVpiUnaryAndOp     = vpiUnaryAndOp,
  kVpiUnaryNandOp    = vpiUnaryNandOp,
  kVpiUnaryOrOp      = vpiUnaryOrOp,
  kVpiUnaryNorOp     = vpiUnaryNorOp,
  kVpiUnaryXorOp     = vpiUnaryXorOp,
  kVpiUnaryXNorOp    = vpiUnaryXNorOp,
  kVpiSubOp          = vpiSubOp,
  kVpiDivOp          = vpiDivOp,
  kVpiModOp          = vpiModOp,
  kVpiEqOp           = vpiEqOp,
  kVpiNeqOp          = vpiNeqOp,
  kVpiCaseEqOp       = vpiCaseEqOp,
  kVpiCaseNeqOp      = vpiCaseNeqOp,
  kVpiGtOp           = vpiGtOp,
  kVpiGeOp           = vpiGeOp,
  kVpiLtOp           = vpiLtOp,
  kVpiLeOp           = vpiLeOp,
  kVpiLShiftOp       = vpiLShiftOp,
  kVpiRShiftOp       = vpiRShiftOp,
  kVpiAddOp          = vpiAddOp,
  kVpiMultOp         = vpiMultOp,
  kVpiLogAndOp       = vpiLogAndOp,
  kVpiLogOrOp        = vpiLogOrOp,
  kVpiBitAndOp       = vpiBitAndOp,
  kVpiBitOrOp        = vpiBitOrOp,
  kVpiBitXorOp       = vpiBitXorOp,
  kVpiBitXNorOp      = vpiBitXNorOp,
  kVpiConditionOp    = vpiConditionOp,
  kVpiConcatOp       = vpiConcatOp,
  kVpiMultiConcatOp  = vpiMultiConcatOp,
  kVpiEventOrOp      = vpiEventOrOp,
  kVpiNullOp         = vpiNullOp,
  kVpiListOp         = vpiListOp,
  kVpiMinTypMaxOp    = vpiMinTypMaxOp,
  kVpiPosedgeOp      = vpiPosedgeOp,
  kVpiNegedgeOp      = vpiNegedgeOp,
  kVpiArithLShiftOp  = vpiArithLShiftOp,
  kVpiArithRShiftOp  = vpiArithRShiftOp,
  kVpiPowerOp        = vpiPowerOp
};


//////////////////////////////////////////////////////////////////////
/// @brief constant の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし kVpiSignedXXXXX はない．
//////////////////////////////////////////////////////////////////////
enum tVpiConstType {
  kVpiDecConst          = vpiDecConst,
  kVpiRealConst         = vpiRealConst,
  kVpiBinaryConst       = vpiBinaryConst,
  kVpiOctConst          = vpiOctConst,
  kVpiHexConst          = vpiHexConst,
  kVpiStringConst       = vpiStringConst,
  kVpiIntConst          = vpiIntConst,
  kVpiSignedDecConst    = kVpiDecConst    | 8,
  kVpiSignedBinaryConst = kVpiBinaryConst | 8,
  kVpiSignedOctConst    = kVpiOctConst    | 8,
  kVpiSignedHexConst    = kVpiHexConst    | 8
};


//////////////////////////////////////////////////////////////////////
/// @brief case の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiCaseType {
  kVpiCaseExact = vpiCaseExact,
  kVpiCaseX     = vpiCaseX,
  kVpiCaseZ     = vpiCaseZ
};


//////////////////////////////////////////////////////////////////////
/// @brief function の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし kVpiRealtimeFunc はない．
//////////////////////////////////////////////////////////////////////
enum tVpiFuncType {
  kVpiIntFunc         = vpiIntFunc,
  kVpiRealFunc        = vpiRealFunc,
  kVpiTimeFunc        = vpiTimeFunc,
  kVpiSizedFunc       = vpiSizedFunc,
  kVpiSizedSignedFunc = vpiSizedSignedFunc,
  kVpiRealtimeFunc    = 6,
  kVpiSysFuncInt      = vpiSysFuncInt,
  kVpiSysFuncReal     = vpiSysFuncReal,
  kVpiSysFuncTime     = vpiSysFuncTime,
  kVpiSysSizedFunc    = vpiSysFuncSized
};


//////////////////////////////////////////////////////////////////////
/// @brief delay の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiDelayType {
  kVpiModPathDelay      = vpiModPathDelay,
  kVpiInterModPathDelay = vpiInterModPathDelay,
  kVpiMIPDelay          = vpiMIPDelay
};


//////////////////////////////////////////////////////////////////////
/// @brief strength の値
//////////////////////////////////////////////////////////////////////
enum tVpiStrength {
  kVpiNoStrength   = 0,
  kVpiSupplyDrive  = 1,
  kVpiStrongDrive  = 2,
  kVpiPullDrive    = 3,
  kVpiWeakDrive    = 4,
  kVpiLargeCharge  = 5,
  kVpiMediumCharge = 6,
  kVpiSmallCharge  = 7,
  kVpiHiZ          = 8
};


//////////////////////////////////////////////////////////////////////
/// @brief 変数のデータ型
//////////////////////////////////////////////////////////////////////
enum tVpiVarType {
  kVpiVarNone     = 0,
  kVpiVarInteger  = 1,
  kVpiVarReal     = 2,
  kVpiVarTime     = 3,
  kVpiVarRealtime = 4
};


//////////////////////////////////////////////////////////////////////
/// @brief 範囲指定のモード
//////////////////////////////////////////////////////////////////////
enum tVpiRangeMode {
  /// @brief 範囲指定なし
  kVpiNoRange    = 0,
  /// @brief 固定
  kVpiConstRange = 1, // [ a : b ]
  /// @brief 可変(プラス)
  kVpiPlusRange  = 2, // [ a +: b ]
  /// @brief 可変(マイナス)
  kVpiMinusRange = 3  // [ a -: b ]
};


//////////////////////////////////////////////////////////////////////
/// @brief IO宣言で用いられる補助的な型
//////////////////////////////////////////////////////////////////////
enum tVpiAuxType {
  /// @brief 型無し
  kVpiAuxNone = 0,
  /// @brief net 型
  kVpiAuxNet  = 1,
  /// @brief reg 型
  kVpiAuxReg  = 2,
  /// @brief 変数型
  kVpiAuxVar  = 3
};


//////////////////////////////////////////////////////////////////////
/// @brief vectored/scalared 指定の値
//////////////////////////////////////////////////////////////////////
enum tVpiVsType {
  /// @brief 指定なし
  kVpiVsNone   = 0,
  /// @brief ベクター型
  kVpiVectored = 1,
  /// @brief スカラー型
  kVpiScalared = 2
};


//////////////////////////////////////////////////////////////////////
/// @brief specify block item の種類
//////////////////////////////////////////////////////////////////////
enum tVpiSpecItemType {
  kVpiPulsestyleOnEvent  = 0,
  kVpiPulsestyleOnDetect = 1,
  kVpiShowcancelled      = 2,
  kVpiNoshowcancelled    = 3
};


//////////////////////////////////////////////////////////////////////
/// @brief specify block path の種類
//////////////////////////////////////////////////////////////////////
enum tVpiSpecPathType {
  kVpiSpecPathNull   = 0,
  kVpiSpecPathIf     = 1,
  kVpiSpecPathIfnone = 2
};

/// @}

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_YM

using nsVerilog::BitVector;
using nsVerilog::VlLineWatcher;
using nsVerilog::VlMgr;
using nsVerilog::VlScalarVal;
using nsVerilog::VlTime;
using nsVerilog::VlUdpVal;
using nsVerilog::VlValueType;
using nsVerilog::VlValue;

END_NAMESPACE_YM

#endif // YM_VERILOG_VERILOG_H
