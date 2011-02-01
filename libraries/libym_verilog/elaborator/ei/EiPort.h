#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPORT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPORT_H

/// @file libym_verilog/elaborator/ei/EiPort.h
/// @brief EiPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPort.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbPort.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPort EiPort.h "EiPort.h"
/// @brief ElbPort の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPort :
  public ElbPort
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiPort();

  /// @brief デストラクタ
  virtual
  ~EiPort();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の区別を得る．
  virtual
  tVpiDirection
  direction() const;

  /// @brief ビット幅を返す．
  virtual
  int
  bit_size() const;

  /// @brief 名前による接続を持つとき true を返す．
  virtual
  bool
  is_conn_by_name() const;

  /// @brief 明示的に名前がついているとき true を返す．
  virtual
  bool
  is_explicit_name() const;

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 親のモジュールを取出す
  virtual
  const VlModule*
  module() const;

  /// @brief ポートリストの何番目のポートかを表すインデックスを返す．
  virtual
  ymuint
  port_index() const;

  /// @brief 上位の接続先を返す．
  virtual
  const VlExpr*
  high_conn() const;

  /// @brief high_conn が左辺式の時の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  ymuint
  high_conn_elem_num() const;

  /// @brief high_conn が左辺式の時の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  high_conn_elem(ymuint pos) const;

  /// @brief 下位の接続先を返す．
  virtual
  const VlExpr*
  low_conn() const;

  /// @brief low_conn が左辺式の時の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  ymuint
  low_conn_elem_num() const;

  /// @brief low_conn が左辺式の時の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  low_conn_elem(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiPortの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  /// @param[in] module 親のモジュール
  /// @param[in] pt_port パース木のポート定義
  /// @param[in] index ポート番号
  /// @param[in] low_conn 下位の接続
  /// @param[in] dir 向き
  virtual
  void
  init(ElbModule* parent,
       const PtPort* pt_port,
       ymuint index,
       ElbLhs* low_conn,
       tVpiDirection dir);

  /// @brief high_conn を接続する．
  virtual
  void
  set_high_conn(ElbLhs* high_conn,
		bool conn_by_name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  ElbModule* mModule;

  // パース木のポート定義
  const PtPort* mPtPort;

  // ポート番号
  ymuint32 mIndex;

  // 上位の接続先
  ElbLhs* mHighConn;

  // 下位の接続先
  ElbLhs* mLowConn;

  // 向き
  tVpiDirection mDirection;

  // 名前による結合の時 true となるフラグ
  bool mConnByName;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPORT_H
