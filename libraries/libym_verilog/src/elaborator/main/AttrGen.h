#ifndef ATTRGEN_H
#define ATTRGEN_H

/// @file AttrGen.h
/// @brief AttrGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"
#include "YmVerilog/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AttrGen AttrGen.h "AttrGen.h"
/// @brief ElbAttr を生成するクラス
//////////////////////////////////////////////////////////////////////
class AttrGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  AttrGen(Elaborator& elab,
	  ElbMgr& elb_mgr,
	  ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~AttrGen();


public:
  //////////////////////////////////////////////////////////////////////
  // AttrGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtAttr から ElbAttr を生成する．
  /// @param[in] pt_attr_array 属性を表すパース木
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] obj 属性を設定する対象のオブジェクト
  void
  instantiate_attribute(PtAttrInstArray pt_attr_array,
			bool def,
			const VlObj* obj);

};

END_NAMESPACE_YM_VERILOG

#endif // ATTRGEN_H
