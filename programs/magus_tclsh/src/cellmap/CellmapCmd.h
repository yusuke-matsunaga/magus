#ifndef MAGUS_CELLMAP_CELLMAPCMD2_H
#define MAGUS_CELLMAP_CELLMAPCMD2_H

/// @file magus/cellmap/CellmapCmd.h
/// @brief CellmapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellmapCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <MagCmd.h>
#include "cellmap.h"
#include "SbjGraph.h"
#include "CellNetwork.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class CellmapData CellmapCmd.h "CellmapCmd.h"
/// @brief CellmapCmd に共通のデータ
//////////////////////////////////////////////////////////////////////
struct CellmapData
{

  // サブジェクトグラフ
  SbjGraph mSbjGraph;

  // LUTネットワーク
  CnGraph mCellNetwork;

};


//////////////////////////////////////////////////////////////////////
/// @class CellmapCmd CellmapCmd.h "CellmapCmd.h"
/// @brief LUTマッピングを行うコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class CellmapCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  CellmapCmd(NetMgr* mgr,
	     CellmapData* data);

  /// @brief デストラクタ
  virtual
  ~CellmapCmd();


protected:

  /// @brief SbjGraph を得る．
  SbjGraph&
  sbjgraph();

  /// @brief CellNetwork を得る．
  CnGraph&
  lutnetwork();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  CellmapData* mData;
  
};


//////////////////////////////////////////////////////////////////////
/// @class Conv2SbjCmd CellmapCmd "CellmapCmd.h"
/// @brief BNetwork から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////
class Conv2SbjCmd :
  public CellmapCmd
{
public:

  /// @brief コンストラクタ
  Conv2SbjCmd(NetMgr* mgr,
	      CellmapData* data);

  /// @brief デストラクタ
  virtual
  ~Conv2SbjCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  
};


//////////////////////////////////////////////////////////////////////
/// @class DumpSbjCmd CellmapCmd "CellmapCmd.h"
/// @brief SbjGraph の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////
class DumpSbjCmd :
  public CellmapCmd
{
public:

  /// @brief コンストラクタ
  DumpSbjCmd(NetMgr* mgr,
	     CellmapData* data);

  /// @brief デストラクタ
  virtual
  ~DumpSbjCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // blif オプションの解析用オブジェクト
  TclPopt* mPoptBlif;
  
};


//////////////////////////////////////////////////////////////////////
/// @class AreaMapCmd CellmapCmd "CellmapCmd.h"
/// @brief area map コマンド
//////////////////////////////////////////////////////////////////////
class AreaMapCmd :
  public CellmapCmd
{
public:

  /// @brief コンストラクタ
  AreaMapCmd(NetMgr* mgr,
	     CellmapData* data);

  /// @brief デストラクタ
  virtual
  ~AreaMapCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // method オプションの解析用オブジェクト
  TclPoptStr* mPoptMethod;

  // resub オプションの解析用オブジェクト
  TclPopt* mPoptResub;

  // verbose オプションの解析用オブジェクト
  TclPopt* mPoptVerbose;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DelayMapCmd CellmapCmd "CellmapCmd.h"
/// @brief delay map コマンド
//////////////////////////////////////////////////////////////////////
class DelayMapCmd :
  public CellmapCmd
{
public:

  /// @brief コンストラクタ
  DelayMapCmd(NetMgr* mgr,
	      CellmapData* data);

  /// @brief デストラクタ
  virtual
  ~DelayMapCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // method オプションの解析用オブジェクト
  TclPoptStr* mPoptMethod;

  // slack オプションの解析用オブジェクト
  TclPoptInt* mPoptSlack;

  // verbose オプションの解析用オブジェクト
  TclPopt* mPoptVerbose;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DumpCellCmd CellmapCmd "CellmapCmd.h"
/// @brief CellNetwork の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////
class DumpCellCmd :
  public CellmapCmd
{
public:

  /// @brief コンストラクタ
  DumpCellCmd(NetMgr* mgr,
	      CellmapData* data);

  /// @brief デストラクタ
  virtual
  ~DumpCellCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  
};


//////////////////////////////////////////////////////////////////////
/// @class Conv2BNetCmd CellmapCmd "CellmapCmd.h"
/// @brief CellNetwork を BNetwork に変換するコマンド
//////////////////////////////////////////////////////////////////////
class Conv2BNetCmd :
  public CellmapCmd
{
public:

  /// @brief コンストラクタ
  Conv2BNetCmd(NetMgr* mgr,
	       CellmapData* data);

  /// @brief デストラクタ
  virtual
  ~Conv2BNetCmd();


protected:
  
  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // dst_network オプションの解析用オブジェクト
  TclPoptStr* mPoptDstNetwork;
  
};

END_NAMESPACE_MAGUS_CELLMAP

#endif // MAGUS_CELLMAP_CELLMAPCMD_H
