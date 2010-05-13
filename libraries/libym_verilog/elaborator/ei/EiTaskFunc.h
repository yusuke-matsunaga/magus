#ifndef LIBYM_VERILOG_ELB_IMPL_EITASKFUNC_H
#define LIBYM_VERILOG_ELB_IMPL_EITASKFUNC_H

/// @file libym_verilog/elb_impl/ElbTaskFuncImpl.h
/// @brief ElbTaskFuncImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiTaskFunc.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbTaskFunc.h"
#include "EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiIODecl;

//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief ElbTask の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTask :
  public ElbTask
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  EiTask(const VlNamedObj* parent,
	 const PtItem* pt_item,
	 ymuint32 io_num,
	 EiIODecl* io_array);

  /// @brief デストラクタ
  virtual
  ~EiTask();
  

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
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;
  
  /// @brief 名前の取得
  virtual
  const char*
  name() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // VlTask の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief automatic 宣言されていたら true を返す．
  virtual
  bool
  automatic() const;
  
  /// @brief 入出力数を得る．
  virtual
  ymuint32
  io_num() const;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  const VlIODecl*
  io(ymuint32 pos) const;
  
  /// @brief 本体のステートメントを得る．
  virtual
  const VlStmt*
  stmt() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbTask の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の初期設定を行う．
  /// @param[in] pos 位置番号
  /// @param[in] pos 位置番号
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  virtual
  void
  init_iodecl(ymuint32 pos,
  	      ElbIOHead* head,
	      const PtIOItem* pt_item,
	      ElbDecl* decl);
  
  /// @brief 本体のステートメントをセットする．
  virtual
  void
  set_stmt(ElbStmt* stmt);

  /// @brief 入出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  ElbIODecl*
  _io(ymuint32 pos) const;
  
  /// @brief 本体の ElbStmt を得る．
  virtual
  ElbStmt*
  _stmt() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の Task 定義
  const PtItem* mPtItem;
  
  // 入出力数
  ymuint32 mIODeclNum;

  // 入出力のリスト
  EiIODecl* mIODeclList;
  
  // 本体のステートメント
  ElbStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiFunction EiTaskFunc.h "EiTaskFunc.h"
/// @brief ElbTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiFunction :
  public ElbFunction
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  EiFunction(const VlNamedObj* parent,
	     const PtItem* pt_item,
	     ymuint32 io_num,
	     EiIODecl* io_array);

  /// @brief デストラクタ
  virtual
  ~EiFunction();
  

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
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;
  
  /// @brief 名前の取得
  virtual
  const char*
  name() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // VlFunction の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief function type を返す．
  virtual
  tVpiFuncType
  func_type() const;
  
  /// @brief 出力のビット幅を返す．
  virtual
  ymuint32
  bit_size() const;
  
  /// @brief 符号付きの時 true を返す．
  virtual
  bool
  is_signed() const;
  
  /// @brief automatic 宣言されていたら true を返す．
  virtual
  bool
  automatic() const;
  
  /// @brief 範囲のMSBを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  left_range() const;
  
  /// @brief 範囲のLSBを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  right_range() const;
  
  /// @brief 入出力数を得る．
  virtual
  ymuint32
  io_num() const;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  const VlIODecl*
  io(ymuint32 pos) const;
  
  /// @brief 本体のステートメントを得る．
  virtual
  const VlStmt*
  stmt() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbFunction の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の初期設定を行う．
  /// @param[in] pos 位置番号
  /// @param[in] pos 位置番号
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木のIO宣言要素
  /// @param[in] decl 対応する宣言要素
  virtual
  void
  init_iodecl(ymuint32 pos,
  	      ElbIOHead* head,
	      const PtIOItem* pt_item,
	      ElbDecl* decl);
  
  /// @brief 出力変数をセットする．
  /// @param[in] ovar 出力変数
  /// @note 関数の場合のみ意味を持つ．
  virtual
  void
  set_ovar(ElbDecl* ovar);
  
  /// @brief 本体のステートメントをセットする．
  virtual
  void
  set_stmt(ElbStmt* stmt);
  
  /// @brief constant function の時に true を返す．
  virtual
  bool
  is_constant_function() const;

  /// @brief スカラー値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_scalar(const vector<ElbExpr*>& arg_list) const;

  /// @brief 論理値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  tVpiScalarVal
  eval_logic(const vector<ElbExpr*>& arg_list) const;
  
  /// @brief real 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  double
  eval_real(const vector<ElbExpr*>& arg_list) const;

  /// @brief bitvector 型の値を返す．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  virtual
  void
  eval_bitvector(const vector<ElbExpr*>& arg_list,
		 BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;

  /// @brief 入出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  ElbIODecl*
  _io(ymuint32 pos) const;
  
  /// @brief 本体の ElbStmt を得る．
  virtual
  ElbStmt*
  _stmt() const;
  
  
private:
  
  /// @brief 関数の値の評価を行う．
  /// @param[in] arg_list 引数のリスト
  /// @note constant function の場合のみ意味を持つ．
  void
  evaluate(const vector<ElbExpr*>& arg_list) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の Task 定義
  const PtItem* mPtItem;
  
  // 入出力数
  ymuint32 mIODeclNum;

  // 入出力のリスト
  EiIODecl* mIODeclList;
  
  // 出力変数
  ElbDecl* mOvar;
  
  // 本体のステートメント
  ElbStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiFunctionV EiaskFunc.h "EiTaskFunc.h"
/// @brief ベクタ型の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiFunctionV :
  public EiFunction
{
  friend class EiFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item パース木の定義
  /// @param[in] io_num IOの数
  /// @param[in] io_array IO の配列
  /// @param[in] left 範囲の MSB の式
  /// @param[in] right 範囲の LSB の式
  /// @param[in] left_val 範囲の MSB の値
  /// @param[in] right_val 範囲の LSB の値
  EiFunctionV(const VlNamedObj* parent,
	      const PtItem* pt_item,
	      ymuint32 io_num,
	      EiIODecl* io_array,
	      ElbExpr* left,
	      ElbExpr* right,
	      int left_val,
	      int right_val);
	      
  /// @brief デストラクタ
  virtual
  ~EiFunctionV();
  

public:
  //////////////////////////////////////////////////////////////////////
  // VlFunction の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 出力のビット幅を返す．
  virtual
  ymuint32
  bit_size() const;
  
  /// @brief 範囲のMSBを返す．
  virtual
  const VlExpr*
  left_range() const;
  
  /// @brief 範囲のLSBを返す．
  virtual
  const VlExpr*
  right_range() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 範囲
  EiRangeImpl mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_IMPL_EITASKFUNC_H
