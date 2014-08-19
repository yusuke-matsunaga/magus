#ifndef SRC_PT_PTIDNODE_H
#define SRC_PT_PTIDNODE_H

/// @file src/pt/PtIdNode.h
/// @brief PtIdNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class PtIdNode PtIdNode.h "PtIdNode.h"
/// @brief ID 番号ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class PtIdNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] id ID番号
  PtIdNode(const FileRegion& file_region,
	   ymuint id);

  /// @param[in]
  /// @brief デストラクタ
  virtual
  ~PtIdNode();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief ID 番号を返す．
  /// @note ID型のノードの時に意味を持つ．
  virtual
  ymuint
  id() const;

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

  // ID 番号
  ymuint32 mId;

};

END_NAMESPACE_YM_BB

#endif // SRC_PT_PTIDNODE_H
