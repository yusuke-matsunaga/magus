#ifndef INCLUDE_PTNODE_H
#define INCLUDE_PTNODE_H

/// @file include/PtNode.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "bb_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_logic/AigMgr.h"
#include "ym_logic/Aig.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class PtNode PtNode.h "PtNode.h"
/// @brief パース木の要素を表すクラス
//////////////////////////////////////////////////////////////////////
class PtNode
{
public:

  /// @brief ノードの型
  enum tType {
    /// @brief ID
    kId,
    /// @brief 定数
    kConst,
    /// @brief 2の補数
    kNegOp,
    /// @brief 単項マイナス
    kUminusOp,
    /// @brief 加算
    kAddOp,
    /// @brief 減算
    kSubOp,
    /// @brief 乗算
    kMulOp,
    /// @brief 除算
    kDivOp,
    /// @brief 剰余算
    kModOp,
    /// @brief 左シフト
    kSllOp,
    /// @brief 右シフト
    kSrlOp,
    /// @brief ビットワイズ AND
    kAndOp,
    /// @brief ビットワイズ OR
    kOrOp,
    /// @brief ビットワイズ XOR
    kXorOp
  };


public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  PtNode(const FileRegion& file_region);

  /// @param[in]
  /// @brief デストラクタ
  virtual
  ~PtNode();


public:

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const;

  /// @brief 型を返す．
  virtual
  tType
  type() const = 0;

  /// @brief ID 番号を返す．
  /// @note ID型のノードの時に意味を持つ．
  virtual
  ymuint
  id() const;

  /// @brief 定数値を返す．
  /// @note 定数型のノードの時に意味を持つ．
  virtual
  int
  cvalue() const;

  /// @brief オペランド数を返す．
  /// @note 演算型のノードの時に意味を持つ．
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note 演算型のノードの時に意味を持つ．
  virtual
  PtNode*
  operand(ymuint pos) const;

  /// @brief 内容を表す文字列を返す．
  virtual
  string
  decompile() const = 0;

  /// @brief 対応した AIG を作る．
  virtual
  void
  gen_aig(AigMgr& aigmgr,
	  const vector<Aig>& bvar_array,
	  ymuint bw,
	  vector<Aig>& out_array) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置を返す．
inline
FileRegion
PtNode::file_region() const
{
  return mFileRegion;
}

END_NAMESPACE_YM_BB

#endif // INCLUDE_PTNODE_H
