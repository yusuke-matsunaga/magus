#ifndef VERILOGWRITERIMPL_H
#define VERILOGWRITERIMPL_H

/// @file VerilogWriterImpl.h
/// @brief VerilogWriterImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/cmn.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class VerilogWriterimpl VerilogWriterImpl.h
/// @brief CmnVerilogWriter の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class VerilogWriterImpl
{
public:

  /// @brief コンストラクタ
  VerilogWriterImpl();

  /// @brief デストラクタ
  ~VerilogWriterImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CmnMgr の内容を Verilog-HDL 形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr 対象のネットワーク
  void
  dump(ostream& s,
       const CmnMgr& mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード名を設定する．
  void
  set_node_name(const CmnNode* node,
		const string& name);

  /// @brief ノード名を返す．
  string
  node_name(const CmnNode* node) const;

  /// @brief DFF/ラッチ の入力のノード名を返す．
  string
  dff_node_name(const CmnNode* node) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードID をキーとして名前を格納する配列
  vector<string> mNameArray;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // VERILOGWRITERIMPL_H
