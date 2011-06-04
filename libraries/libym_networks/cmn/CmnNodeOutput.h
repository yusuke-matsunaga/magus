#ifndef LIBYM_NETWORKS_CMN_CMNNODEOUTPUT_H
#define LIBYM_NETWORKS_CMN_CMNNODEOUTPUT_H

/// @file libym_networks/cmn/CmnNodeOutput.h
/// @brief CmnNodeOutput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnNode.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnNodeOutput CmnNodeOutput.h "CmnNodeOutput.h"
/// @brief 出力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeOutput :
  public CmnNode
{
  friend class CmnMgrImpl;

protected:

  /// @brief コンストラクタ
  CmnNodeOutput();

  /// @brief デストラクタ
  virtual
  ~CmnNodeOutput();


public:

  /// @brief タイプを得る．
  virtual
  tType
  type() const;

  /// @brief 出力ノードの時に true を返す．
  virtual
  bool
  is_output() const;

  /// @brief ファンインのノードを得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  virtual
  const CmnNode*
  output_fanin() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンインのノード
  const CmnNode* mFanin;

};

END_NAMESPACE_YM_CMN


#endif // LIBYM_NETWORKS_CMN_CMNNODEOUTPUT_H
