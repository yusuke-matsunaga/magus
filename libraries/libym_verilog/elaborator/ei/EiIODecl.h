#ifndef LIBYM_VERILOG_ELB_IMPL_EIIODECL_H
#define LIBYM_VERILOG_ELB_IMPL_EIIODECL_H

/// @file libym_verilog/elaborator/EiIODecl.h
/// @brief EiIODecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiIODecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbIODecl.h"
#include "EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiIOHead  EiIODecl.h "EiIODecl.h"
/// @brief IO のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class EiIOHead :
  public ElbIOHead
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiIOHead(const PtIOHead* pt_header);
  
  /// @brief デストラクタ
  virtual
  ~EiIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tVpiDirection
  direction() const;
  
  /// @brief 親のモジュールの取得
  /// @note このクラスでは NULL を返す．
  virtual
  ElbModule*
  module() const;
  
  /// @brief 親の task の取得
  /// @note このクラスでは NULL を返す．
  virtual
  ElbTask*
  task() const;
  
  /// @brief 親の function の取得
  /// @note このクラスでは NULL を返す．
  virtual
  ElbFunction*
  function() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // パース木の IO 宣言ヘッダ
  const PtIOHead* mPtHead;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiModIOHead  EiIODecl.h "EiIODecl.h"
/// @brief module 用の IO のヘッダ
//////////////////////////////////////////////////////////////////////
class EiModIOHead :
  public EiIOHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiModIOHead(ElbModule* module,
	      const PtIOHead* pt_header);
  
  /// @brief デストラクタ
  virtual
  ~EiModIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールの取得
  ElbModule*
  module() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  ElbModule* mModule;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiTaskIOHead EiIODecl.h "EiIODecl.h"
/// @brief task 用の IO のヘッダ
//////////////////////////////////////////////////////////////////////
class EiTaskIOHead :
  public EiIOHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] task 親の task
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiTaskIOHead(ElbTask* task,
	       const PtIOHead* pt_header);
  
  /// @brief デストラクタ
  virtual
  ~EiTaskIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の task の取得
  ElbTask*
  task() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の task
  ElbTask* mTask;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiFuncIOHead EiIODecl.h "EiIODecl.h"
/// @brief function 用の IO のヘッダ
//////////////////////////////////////////////////////////////////////
class EiFuncIOHead :
  public EiIOHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] func 親の function
  /// @param[in] pt_header パース木のIO宣言ヘッダ
  EiFuncIOHead(ElbFunction* func,
	       const PtIOHead* pt_header);
  
  /// @brief デストラクタ
  virtual
  ~EiFuncIOHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiIOHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の function の取得
  ElbFunction*
  function() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の function
  ElbFunction* mFunction;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiIODecl EiIODecl.h "EiIODecl.h"
/// @brief IO 要素を表すクラス
//////////////////////////////////////////////////////////////////////
class EiIODecl :
  public ElbIODecl
{
  friend class EiFactory;
  
private:
  
  /// @brief コンストラクタ
  EiIODecl();
  
  /// @brief デストラクタ
  virtual
  ~EiIODecl();

  
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
  // VlIODecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief 方向を返す．
  virtual
  tVpiDirection
  direction() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  right_range() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief 対応する宣言要素を返す．
  virtual
  const VlDecl*
  decl() const;

  /// @brief 親のモジュールの取得
  virtual
  const VlModule*
  module() const;

  /// @brief 親の UDP の取得
  /// @note このクラスでは NULL を返す．
  virtual
  const VlUdpDefn*
  udp_defn() const;

  /// @brief 親の task の取得
  virtual
  const VlTask*
  task() const;

  /// @brief 親の function の取得
  virtual
  const VlFunction*
  function() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbIODecl の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 対応する ElbDecl を返す．
  virtual
  ElbDecl*
  _decl() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiIODecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  /// @param[in] 符号付き属性の補正値
  void
  init(ElbIOHead* head,
       const PtIOItem* pt_item,
       ElbDecl* decl);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbIOHead* mHead;

  // パース木の IO 宣言
  const PtIOItem* mPtItem;

  // 対応する宣言要素
  ElbDecl* mDecl;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_EIIODECL_H
