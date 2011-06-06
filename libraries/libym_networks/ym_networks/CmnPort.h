#ifndef YM_NETWORKS_CMNPORT_H
#define YM_NETWORKS_CMNPORT_H

/// @file ym_networks/CmnPort.h
/// @brief CmnPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn_nsdef.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnPort CmnPort.h "ym_networks/CmnPort.h"
/// @brief ポートを表すクラス
//////////////////////////////////////////////////////////////////////
class CmnPort
{
  friend class CmnMgrImpl;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] bw ビット数
  /// @param[in] bit_array 対応する入出力ノードの配列
  CmnPort(const string& name,
	  ymuint bw);

  /// @brief デストラクタ
  ~CmnPort();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID 番号を得る．
  ymuint
  id() const;

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief ビット数を得る．
  ymuint
  bit_width() const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 出力ポートの場合には NULL を返す．
  const CmnNode*
  input(ymuint pos) const;

  /// @brief 出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 入力ポートの場合には NULL を返す．
  const CmnNode*
  output(ymuint pos) const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 出力ポートの場合には NULL を返す．
  CmnNode*
  _input(ymuint pos);

  /// @brief 出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 入力ポートの場合には NULL を返す．
  CmnNode*
  _output(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 名前
  string mName;

  // ビット数
  ymuint32 mBitWidth;

  // 入力ノードの配列
  CmnNode** mInputArray;

  // 出力ノードの配列
  CmnNode** mOutputArray;

};


//////////////////////////////////////////////////////////////////////
// クラス CmnPort
//////////////////////////////////////////////////////////////////////

// @brief 名前を得る．
inline
string
CmnPort::name() const
{
  return mName;
}

// @brief ビット数を得る．
inline
ymuint
CmnPort::bit_width() const
{
  return mBitWidth;
}

// @brief 入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
// @note 出力ポートの場合には NULL を返す．
const CmnNode*
CmnPort::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
// @note 入力ポートの場合には NULL を返す．
const CmnNode*
CmnPort::output(ymuint pos) const
{
  return mOutputArray[pos];
}

// @brief 入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
// @note 出力ポートの場合には NULL を返す．
CmnNode*
CmnPort::_input(ymuint pos)
{
  return mInputArray[pos];
}

// @brief 出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
// @note 入力ポートの場合には NULL を返す．
CmnNode*
CmnPort::_output(ymuint pos)
{
  return mOutputArray[pos];
}

END_NAMESPACE_YM_CMN

#endif // YM_NETWORKS_CMNPORT_H
