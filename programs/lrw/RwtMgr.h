#ifndef RWTMGR_H
#define RWTMGR_H

/// @file RwtMgr.h
/// @brief RwtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class RwtNode;

//////////////////////////////////////////////////////////////////////
/// @class RwtMgr RwtMgr.h "RwtMgr.h"
/// @brief RwtNode をセットアップするためのクラス
//////////////////////////////////////////////////////////////////////
class RwtMgr
{
public:

  /// @brief コンストラクタ
  RwtMgr();

  /// @brief デストラクタ
  ~RwtMgr();


public:

  /// @brief 初期化する．
  void
  init();

  /// @brief 関数に対するノードを返す．
  RwtNode*
  find_node(ymuint16 func) const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  RwtNode*
  node(ymuint pos) const;

  /// @brief 内容をダンプする．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを設定する．
  void
  set_node(ymuint id,
	   ymuint16 func,
	   bool xor_flag,
	   RwtNode* child0,
	   RwtNode* child1,
	   bool inv0,
	   bool inv1,
	   ymuint level,
	   ymuint volume);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // ノードの配列
  RwtNode* mNodeArray;

  // 関数ベクタをキーにしてノードを納めた連想配列
  unordered_map<ymuint16, RwtNode*> mFuncHash;

};

END_NAMESPACE_YM

#endif // RWTMGR_H
