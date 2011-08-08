#ifndef YM_NETWORKS_MVNNODE_H
#define YM_NETWORKS_MVNNODE_H

/// @file ym_networks/MvnNode.h
/// @brief MvnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn.h"
#include "ym_networks/MvnInputPin.h"
#include "ym_networks/MvnOutputPin.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnNode MvnNode.h "ym_networks/MvnNode.h"
/// @brief MvNetwork のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnNode
{
  friend class MvnMgr;

public:
  //////////////////////////////////////////////////////////////////////
  /// @brief 型定義
  /// @note 論理演算とビットワイズ論理演算の区別はない．
  //////////////////////////////////////////////////////////////////////
  enum tType {
    /// @brief 入力 ( 0入力, 1出力 )
    kInput,
    /// @brief 出力 ( 1入力, 0出力 )
    kOutput,
    /// @brief 入出力 ( 1入力, 1出力 )
    kInout,

    /// @brief フリップフロップ
    /// (クロック，データ入力，非同期セット入力，データ出力)
    /// 非同期セットは複数ある場合もある．
    kDff,

    /// @brief ラッチ
    /// (データ入力，ラッチイネーブル，データ出力)
    kLatch,

    /// @brief through ( 1入力, 1出力 )
    kThrough,

    /// @brief logical not ( 1入力, 1出力 )
    kNot,
    /// @brief logical and ( n入力, 1出力 )
    kAnd,
    /// @brief logical or ( n入力, 1出力 )
    kOr,
    /// @brief logical xor ( n入力, 1出力 )
    kXor,

    /// @brief reduction and ( 1入力, 1出力 )
    kRand,
    /// @brief reduction or ( 1入力, 1出力 )
    kRor,
    /// @brief reduction xor ( 1入力, 1出力 )
    kRxor,

    /// @brief equal ( 2入力, 1出力 )
    kEq,
    /// @brief less than ( 2入力, 1出力 )
    kLt,
    /// @brief case-equal ( 2入力, 1出力 )
    kCaseEq,

    /// @brief shift left logical ( 2入力, 1出力 )
    kSll,
    /// @brief shift right logical ( 2入力, 1出力 )
    kSrl,
    /// @brief shift left arithmetic ( 2入力, 1出力 )
    kSla,
    /// @brief shift right arithmetic ( 2入力, 1出力 )
    kSra,

    /// @brief cmpl ( 1入力, 1出力 )
    /// 2の補数
    kCmpl,
    /// @brief add ( 2入力, 1出力 )
    kAdd,
    /// @brief sub ( 2入力, 1出力 )
    kSub,
    /// @brief multiply ( 2入力, 1出力 )
    kMult,
    /// @brief division ( 2入力, 1出力 )
    kDiv,
    /// @brief modulo ( 2入力, 1出力 )
    kMod,
    /// @brief power ( 2入力, 1出力 )
    kPow,

    /// @brief condition ( 3入力, 1出力 )
    kIte,

    /// @brief concatenate ( n入力, 1出力 )
    kConcat,

    /// @brief constant bit-select ( 1入力, 1出力 )
    kConstBitSelect,

    /// @brief constant part-select ( 1入力, 1出力 )
    kConstPartSelect,

    /// @brief bit-select ( 2入力, 1出力 )
    kBitSelect,

    /// @brief part-select ( 3入力, 1出力 )
    kPartSelect,

    /// @brief combinational UDP ( n入力, 1出力 )
    kCombUdp,

    /// @brief sequential UDP ( n入力, 1出力 )
    kSeqUdp,

    /// @brief constant ( 0入力, 1出力 )
    kConst,

    /// @brief cell ( n入力，n出力 )
    kCell

  };


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  MvnNode(MvnModule* parent);

  /// @brief デストラクタ
  virtual
  ~MvnNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 親のモジュールを得る．
  const MvnModule*
  parent() const;

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const = 0;

  /// @brief 入力ピン数を得る．
  virtual
  ymuint
  input_num() const = 0;

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  const MvnInputPin*
  input(ymuint pos) const = 0;

  /// @brief 出力ピン数を得る．
  virtual
  ymuint
  output_num() const = 0;

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  const MvnOutputPin*
  output(ymuint pos) const = 0;

  /// @brief クロック信号の極性を得る．
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  clock_pol() const;

  /// @brief 非同期セット信号の極性を得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  control_pol(ymuint pos) const;

  /// @brief 非同期セットの値を表す定数ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const MvnNode*
  control_val(ymuint pos) const;

  /// @brief ビット位置を得る．
  /// @note type() が kConstBitSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  bitpos() const;

  /// @brief 範囲指定の MSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  lsb() const;

  /// @brief 定数値を得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kConst の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  const_value(vector<ymuint32>& val) const;

  /// @brief Xマスクを得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kCaseEq の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  xmask(vector<ymuint32>& val) const;

  /// @brief セルを得る．
  /// @note type() が kCell の時のみ意味をモツ．
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const Cell*
  cell() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  MvnInputPin*
  _input(ymuint pos) = 0;

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  MvnOutputPin*
  _output(ymuint pos) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 親のモジュール
  MvnModule* mParent;

  // mNodeList からの削除で用いる反復子
  list<MvnNode*>::iterator mSelfRef;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
MvnNode::id() const
{
  return mId;
}

// @brief 親のモジュールを得る．
inline
const MvnModule*
MvnNode::parent() const
{
  return mParent;
}

END_NAMESPACE_YM_NETWORKS_MVN

#endif // YM_NETWORKS_MVNNODE_H
