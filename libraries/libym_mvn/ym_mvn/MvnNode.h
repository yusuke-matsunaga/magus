#ifndef YM_MVN_MVNNODE_H
#define YM_MVN_MVNNODE_H

/// @file ym_mvn/MvnNode.h
/// @brief MvnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_mvn/MvnInputPin.h"
#include "ym_mvn/MvnOutputPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnNode MvnNode.h "ym_mvn/MvnNode.h"
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
    /// (クロック，データ入力，非同期セット(コントロールとデータ))
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
    kConst

  };


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
  ymuint
  input_num() const;

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  const MvnInputPin*
  input(ymuint pos) const;

  /// @brief 出力ピン数を得る．
  ymuint
  output_num() const;

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  const MvnOutputPin*
  output(ymuint pos) const;

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


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  MvnNode(MvnModule* parent,
	 ymuint ni,
	 ymuint no);

  /// @brief デストラクタ
  virtual
  ~MvnNode();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  MvnInputPin*
  _input(ymuint pos);

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  MvnOutputPin*
  _output(ymuint pos);

  /// @brief 入力ピンのビット幅を設定する．
  /// @param[in] pos 対象のピン番号
  /// @param[in] bit_width ビット幅
  void
  set_ipin_bit_width(ymuint pos,
		     ymuint bit_width);

  /// @brief 出力ピンのビット幅を設定する．
  /// @param[in] pos 対象のピン番号
  /// @param[in] bit_width ビット幅
  void
  set_opin_bit_width(ymuint pos,
		     ymuint bit_width);


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

  // 入力ピン数
  ymuint32 mNi;

  // 入力ピンの配列
  MvnInputPin* mInputPins;

  // 出力ピン数
  ymuint32 mNo;

  // 出力ピンの配列
  MvnOutputPin* mOutputPins;

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

// @brief 入力ピン数を得る．
inline
ymuint
MvnNode::input_num() const
{
  return mNi;
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
const MvnInputPin*
MvnNode::input(ymuint pos) const
{
  return &mInputPins[pos];
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
MvnInputPin*
MvnNode::_input(ymuint pos)
{
  return &mInputPins[pos];
}

// @brief 出力ピン数を得る．
inline
ymuint
MvnNode::output_num() const
{
  return mNo;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
const MvnOutputPin*
MvnNode::output(ymuint pos) const
{
  return &mOutputPins[pos];
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
MvnOutputPin*
MvnNode::_output(ymuint pos)
{
  return &mOutputPins[pos];
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNNODE_H
