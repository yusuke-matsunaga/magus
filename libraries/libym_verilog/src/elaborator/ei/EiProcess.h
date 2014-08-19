#ifndef EIPROCESS_H
#define EIPROCESS_H

/// @file EiProcess.h
/// @brief EiProcess のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbProcess.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiProcess EiProcess.h "EiProcess.h"
/// @brief initial/always 文を表すクラス
//////////////////////////////////////////////////////////////////////
class EiProcess :
  public ElbProcess
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の要素定義
  EiProcess(const VlNamedObj* parent,
	    const PtItem* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiProcess();


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
  // VlProcess の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 本体のステートメントの取得
  virtual
  const VlStmt*
  stmt() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbProcess の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントをセットする．
  /// @param[in] stmt 本体のステートメント
  void
  set_stmt(ElbStmt* stmt);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の要素定義
  const PtItem* mPtItem;

  // 本体のステートメント
  ElbStmt* mStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPROCESS_H
