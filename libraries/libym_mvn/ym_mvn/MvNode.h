#ifndef YM_MVN_MVNODE_H
#define YM_MVN_MVNODE_H

/// @file ym_mvn/MvNode.h
/// @brief MvNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvNode MvNode.h "ym_mvn/MvNode.h"
/// @brief MvNetwork のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvNode :
  public DlElem
{
  friend class MvMgr;

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

    /// @brief through ( 1入力, 1出力 )
    kThrough,

    /// @brief logical not ( 1入力, 1出力 )
    kNot,
    /// @brief logical and ( 2入力, 1出力 )
    kAnd,
    /// @brief logical or ( 2入力, 1出力 )
    kOr,
    /// @brief logical xor ( 2入力, 1出力 )
    kXor,

    /// @brief reduction and ( 1入力, 1出力 )
    kRAnd,
    /// @brief reduction or ( 1入力, 1出力 )
    kROr,
    /// @brief reduction xor ( 1入力, 1出力 )
    kRXor,

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

    /// @brief bit select ( 2入力, 1出力 )
    kBitSelect,

    /// @brief part select ( 3入力, 1出力 )
    kPartSelect,
    
    /// @brief module instance ( n入力, m出力 )
    kInst,
    
  };


public:

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 親のモジュールを得る．
  MvModule*
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
  const MvInputPin*
  input(ymuint pos) const = 0;

  /// @brief 出力ピン数を得る．
  virtual
  ymuint
  output_num() const = 0;

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  const MvOutputPin*
  output(ymuint pos) const = 0;

  /// @brief 入力ノード/出力ノードの場合に位置を返す．
  /// @note type() が kInput と kOutput の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  pos() const;

  /// @brief 下位モジュールを得る．
  /// @note type() が kInst の時のみ意味を持つ．
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const MvModule*
  module() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvNode() { }

  /// @brief デストラクタ
  virtual
  ~MvNode() { }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  MvInputPin*
  input(ymuint pos) = 0;

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  MvOutputPin*
  output(ymuint pos) =0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 親のモジュール
  MvModule* mParent;

#if 0
  // ノードの種類
  tType mType;
  
  // 入力ピン数
  ymuint32 mInputNum;

  // 入力ピンの実体の配列
  MvInputPin* mInputArray;

  // 出力ピン
  ymuint32 mOutputNum;

  // 出力ピンの実体の配列
  MvOutputPin* mOutputArray;
#endif

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
MvNode::id() const
{
  return mId;
}

// @brief 親のモジュールを得る．
inline
MvModule*
MvNode::parent() const
{
  return mParent;
}

#if 0
// @brief ノードの種類を得る．
inline
tType
MvNode::type() const
{
  return mType;
}

// @brief 入力ピン数を得る．
inline
ymuint
MvNode::input_num() const
{
  return mInputNum;
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
const MvInputPin*
MvNode::input(ymuint pos) const
{
  return &mInputArray[pos];
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
MvInputPin*
MvNode::input(ymuint pos)
{
  return &mInputArray[pos];
}

// @brief 出力ピン数を得る．
inline
ymuint
MvNode::output_num() const
{
  return mOutputNum;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
const MvOutputPin*
MvNode::output(ymuint pos) const
{
  return &mOutputArray[pos];
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
MvOutputPin*
MvNode::output(ymuint pos)
{
  return &mOutputArray[pos];
}
#endif

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNODE_H
