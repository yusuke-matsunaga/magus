#ifndef VMSTMTNODE_H
#define VMSTMTNODE_H

/// @file libym_verilog/tests/vlview/VmStmtNode.h
/// @brief VmStmtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmStmtNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmStmtNodeList VmStmtNode.h
/// @brief ステートメントリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmStmtListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] stmt_array ステートメントの配列
  VmStmtListNode(const QString& label,
		 const vector<const VlStmt*>& stmt_array);

  /// @brief デストラクタ
  virtual
  ~VmStmtListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // ステートメントの配列
  vector<const VlStmt*> mStmtArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmStmtNode VmStmtNode.h
/// @brief ステートメントを表すノード
//////////////////////////////////////////////////////////////////////
class VmStmtNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label
  /// @param[in] stmt ステートメント
  VmStmtNode(const QString& label,
	     const VlStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~VmStmtNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // ステートメント
  const VlStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @Class VmArgListNode VmStmtNode.h
/// @brief 引数のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmArgListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt 親の enable 文
  VmArgListNode(const VlStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~VmArgListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親の enable 文
  const VlStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmCaseItemListNode VmStmtNode.h
/// @brief caseitem のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmCaseItemListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt 親の case 文
  VmCaseItemListNode(const VlStmt* stmt);

  /// @brief デストラクタ
  virtual
  ~VmCaseItemListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親の case 文
  const VlStmt* mStmt;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmCaseItemNode VmStmtNode.h
/// @brief caseitem を表すノード
//////////////////////////////////////////////////////////////////////
class VmCaseItemNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] caseitem caseitem 文
  VmCaseItemNode(const VlCaseItem* caseitem);

  /// @brief デストラクタ
  virtual
  ~VmCaseItemNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // caseitem 文を表すオブジェクト
  const VlCaseItem* mCaseItem;
  
};


//////////////////////////////////////////////////////////////////////
/// @Class VmControlNode VmStmtNode.h
/// @brief control を表すノード
//////////////////////////////////////////////////////////////////////
class VmControlNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] control コントロールを表すオブジェクト
  VmControlNode(const VlControl* control);

  /// @brief デストラクタ
  virtual
  ~VmControlNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // コントロール
  const VlControl* mControl;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMSTMTNODE_H
