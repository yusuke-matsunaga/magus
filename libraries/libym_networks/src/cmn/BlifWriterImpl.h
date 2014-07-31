#ifndef BLIFWRITERIMPL_H
#define BLIFWRITERIMPL_H

/// @file BlifWriterImpl.h
/// @brief BlifWriterImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/cmn.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class BlifWriterimpl BlifWriterImpl.h
/// @brief CmnBlifWriter の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class BlifWriterImpl
{
public:

  /// @brief コンストラクタ
  BlifWriterImpl();

  /// @brief デストラクタ
  ~BlifWriterImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CmnMgr の内容を net-blif 形式で出力する．
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

#endif // BLIFWRITERIMPL_H
