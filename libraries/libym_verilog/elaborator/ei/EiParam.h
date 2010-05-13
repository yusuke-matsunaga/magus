#ifndef LIBYM_VERILOG_ELB_IMPL_EIPARAM_H
#define LIBYM_VERILOG_ELB_IMPL_EIPARAM_H

/// @file libym_verilog/elb_impl/EiParam.h
/// @brief EiParam のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParam.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "EiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParam EiParam.h "EiParam.h"
/// @brief parameter を表す ElbDecl の実装クラス
//////////////////////////////////////////////////////////////////////
class EiParam :
  public EiDecl
{
  friend class EiFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  EiParam(ElbDeclHead* head,
	  const PtNamedBase* pt_item,
	  ElbExpr* init);

  /// @brief デストラクタ
  virtual
  ~EiParam();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;
  
  /// @brief 定数値を持つ型のときに true を返す．
  virtual
  bool
  is_consttype() const;
  
  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const VlExpr*
  init_value() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 初期値の設定
  /// @param[in] expr 初期値
  virtual
  void
  set_init(ElbExpr* expr);
  
  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  get_scalar() const;
    
  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(tVpiScalarVal val);
  
  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;
  
  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val);
  
  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		tVpiScalarVal val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  ElbExpr* mInit;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiLocalParam EiParam.h "EiParam.h"
/// @brief localparam を表す ElbDecl の実装クラス
//////////////////////////////////////////////////////////////////////
class EiLocalParam :
  public EiParam
{
  friend class EiFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] init 初期値
  EiLocalParam(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ElbExpr* init);

  /// @brief デストラクタ
  virtual
  ~EiLocalParam();
  

public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief localparam のときに true 返す．
  /// @note このクラスでは true を返す．
  virtual
  bool
  is_local_param() const;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_IMPL_EIPARAM_H
