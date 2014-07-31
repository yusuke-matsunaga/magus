#ifndef IMPREC_H
#define IMPREC_H

/// @file ImpRec.h
/// @brief ImpRec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetwork.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpNode;

//////////////////////////////////////////////////////////////////////
/// @class ImpRec ImpRec.h "ImpRec.h"
/// @brief 含意結果を記録するための基底クラス
//////////////////////////////////////////////////////////////////////
class ImpRec
{
public:

  /// @brief デストラクタ
  virtual
  ~ImpRec() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 含意元の情報をセットする．
  /// @param[in] node 含意元のノード
  /// @param[in] val 値
  void
  set_src(ImpNode* node,
	  ymuint val);

  /// @brief 含意結果を記録する．
  /// @param[in] dst_node 含意先のノード
  /// @param[in[ dst_val 含意先の値
  void
  record(ImpNode* dst_node,
	 ymuint dst_val);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 含意結果を記録する仮想関数
  /// @param[in] src_node 含意元のノード
  /// @param[in] src_val 含意元の値
  /// @param[in] dst_node 含意先のノード
  /// @param[in[ dst_val 含意先の値
  virtual
  void
  record(ImpNode* src_node,
	 ymuint src_val,
	 ImpNode* dst_node,
	 ymuint dst_val) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意元のノード
  ImpNode* mSrcNode;

  // 含意元の値
  ymuint8 mSrcVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 含意元の情報をセットする．
// @param[in] node 含意元のノード
// @param[in] val 値
inline
void
ImpRec::set_src(ImpNode* node,
		ymuint val)
{
  mSrcNode = node;
  mSrcVal = val;
}

// @brief 含意結果を記録する．
// @param[in] dst_node 含意先のノード
// @param[in[ dst_val 含意先の値
inline
void
ImpRec::record(ImpNode* dst_node,
	       ymuint dst_val)
{
  record(mSrcNode, mSrcVal, dst_node, dst_val);
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPREC_H
