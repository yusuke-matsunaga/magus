#ifndef SRC_PT_PTCONSTNODE_H
#define SRC_PT_PTCONSTNODE_H

/// @file src/pt/PtConstNode.h
/// @brief PtConstNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class PtConstNode PtConstNode.h "PtConstNode.h"
/// @brief 定数ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class PtConstNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] value 値
  PtConstNode(const FileRegion& file_region,
	      ymint32 value);

  /// @brief デストラクタ
  virtual
  ~PtConstNode();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief 定数値を返す．
  /// @note 定数型のノードの時に意味を持つ．
  virtual
  int
  cvalue() const;

  /// @brief 内容を表す文字列を返す．
  virtual
  string
  decompile() const;

  /// @brief 対応した AIG を作る．
  virtual
  void
  gen_aig(AigMgr& aigmgr,
	  const vector<Aig>& bvar_array,
	  ymuint bw,
	  vector<Aig>& out_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数値
  ymint32 mValue;

};

END_NAMESPACE_YM_BB

#endif // SRC_PT_PTCONSTNODE_H
