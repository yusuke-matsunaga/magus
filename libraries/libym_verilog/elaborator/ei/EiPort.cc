
/// @file libym_verilog/elb_impl/EiPort.cc
/// @brief EiPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPort.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiPort.h"
#include "ElbModule.h"
#include "ElbExpr.h"
#include "ElbLhs.h"

#include "ym_verilog/pt/PtPort.h"



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPort::EiPort()
{
}

// @brief デストラクタ
EiPort::~EiPort()
{
}

// @brief 初期設定を行う．
// @param[in] module 親のモジュール
// @param[in] pt_port パース木のポート定義
// @param[in] index ポート番号
// @param[in] low_conn 下位の接続
// @param[in] dir 向き
void
EiPort::init(ElbModule* module,
	     const PtPort* pt_port,
	     ymuint index,
	     ElbLhs* low_conn,
	     tVpiDirection dir)
{
  mModule = module;
  mPtPort = pt_port;
  mIndex = index;
  mHighConn = NULL;
  mLowConn = low_conn;
  mDirection = dir;
  mConnByName = false;
}

// @brief high_conn を接続する．
void
EiPort::set_high_conn(ElbLhs* high_conn,
		      bool conn_by_name)
{
  mHighConn = high_conn;
  mConnByName = conn_by_name;
}

// @brief 型の取得
tVpiObjType
EiPort::type() const
{
  return kVpiPort;
}

// @brief ファイル位置の取得
FileRegion
EiPort::file_region() const
{
  return mPtPort->file_region();
}

// @brief 入出力の区別を得る．
tVpiDirection
EiPort::direction() const
{
  return mDirection;
}

// @brief ビット幅を返す．
int
EiPort::bit_size() const
{
  return mLowConn->_expr()->bit_size();
}

// @brief 名前による接続を持つとき true を返す．
bool
EiPort::is_conn_by_name() const
{
  return mConnByName;
}

// @brief 明示的に名前がついているとき true を返す．
bool
EiPort::is_explicit_name() const
{
  return mPtPort->ext_name() != NULL;
}

// @brief 名前を返す．
const char*
EiPort::name() const
{
  return mPtPort->ext_name();
}

// @brief 親のモジュールを取出す
const VlModule*
EiPort::module() const
{
  return mModule;
}

// @brief ポートリストの何番目のポートかを表すインデックスを返す．
ymuint
EiPort::port_index() const
{
  return mIndex;
}

// @brief 上位の接続先を返す．
const VlExpr*
EiPort::high_conn() const
{
  if ( mHighConn != NULL ) {
    return mHighConn->_expr();
  }
  return NULL;
}

// @brief high_conn が左辺式の時の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
ymuint
EiPort::high_conn_elem_num() const
{
  if ( mHighConn != NULL ) {
    return mHighConn->elem_num();
  }
  return 0;
}

// @brief high_conn が左辺式の時の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
const VlExpr*
EiPort::high_conn_elem(ymuint pos) const
{
  if ( mHighConn != NULL ) {
    return mHighConn->elem(pos);
  }
  return NULL;
}

// @brief 下位の接続先を返す．
const VlExpr*
EiPort::low_conn() const
{
  if ( mLowConn != NULL ) {
    return mLowConn->_expr();
  }
  return NULL;
}

// @brief low_conn が左辺式の時の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
ymuint
EiPort::low_conn_elem_num() const
{
  if ( mLowConn != NULL ) {
    return mLowConn->elem_num();
  }
  return 0;
}

// @brief low_conn が左辺式の時の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
const VlExpr*
EiPort::low_conn_elem(ymuint pos) const
{
  if ( mLowConn != NULL ) {
    return mLowConn->elem(pos);
  }
  return NULL;
}

END_NAMESPACE_YM_VERILOG
