
/// @file YmUtils.sip
/// @brief YmUtils モジュールの sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

%Module YmUtils

%Include GcSolver.sip
%Include McSolver.sip
%Include GenBase.sip
%Include CombiGen.sip
%Include PermGen.sip
%Include RandGen.sip
%Include FileInfo.sip
%Include FileLoc.sip
%Include FileRegion.sip
%Include IDO.sip
%Include ODO.sip
%Include CodecType.sip
%Include FileIDO.sip
%Include FileODO.sip
%Include MFSet.sip
%Include BtgMatch.sip
%Include USTime.sip
%Include StopWatch.sip
%Include MStopWatch.sip
%Include MsgType.sip
%Include MsgMgr.sip

%ModuleHeaderCode
#include "utils/CodecType.h"
using nsYm::kCodecThrough;
using nsYm::kCodecZ;
using nsYm::kCodecGzip;
using nsYm::kCodecBzip2;
using nsYm::kCodecLzma;

#include "utils/msg_type.h"
using nsYm::tMsgType;
using nsYm::kMsgError;
using nsYm::kMsgWarning;
using nsYm::kMsgFailure;
using nsYm::kMsgInfo;
using nsYm::kMsgDebug;
%End
