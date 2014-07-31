#ifndef UDPGEN_H
#define UDPGEN_H

/// @file UdpGen.h
/// @brief UdpGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"
#include "YmVerilog/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class UdpGen UdpGen.h "UdpGen.h"
/// @brief ElbUdpDefn を生成するクラス
//////////////////////////////////////////////////////////////////////
class UdpGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  UdpGen(Elaborator& elab,
	 ElbMgr& elb_mgr,
	 ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~UdpGen();


public:
  //////////////////////////////////////////////////////////////////////
  // UdpGen の関数
  //////////////////////////////////////////////////////////////////////
  /// @brief UDP定義を生成する．
  /// @param[in] pt_udp パース木の UDP 定義
  void
  instantiate_udp(const PtUdp* pt_udp);

};

END_NAMESPACE_YM_VERILOG

#endif // ELBUDPGEN_H
