#ifndef LIBYM_MVN_VERILOG_CFGMGR_H
#define LIBYM_MVN_VERILOG_CFGMGR_H

/// @file libym_mvn/verilog/CfgMgr.h
/// @brief CfgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_MVN

class CfgNode;

//////////////////////////////////////////////////////////////////////
/// @class CfgMgr CfgMgr.h "CfgMgr.h"
/// @brief CFG を表すクラス
//////////////////////////////////////////////////////////////////////
class CfgMgr
{
public:

  /// @brief コンストラクタ
  CfgMgr();

  /// @brief デストラクタ
  ~CfgMgr();


public:

  /// @brief ノード(ステートメント)を追加する．
  pair<CfgNode*, CfgNode*>
  add_stmt(const VlStmt* stmt);

  /// @brief 先頭のノードを取り出す．
  CfgNode*
  top() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを生成する．
  /// @param[in] stmt ノードに対応するステートメント
  CfgNode*
  new_node(const VlStmt* stmt);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CfgNode のメモリ確保用のオブジェクト
  UnitAlloc mAlloc;

  // 先頭のノード
  CfgNode* mTop;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_VERILOG_CFGMGR_H
