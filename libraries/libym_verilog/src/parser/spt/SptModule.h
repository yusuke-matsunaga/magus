#ifndef SPTMODULE_H
#define SPTMODULE_H

/// @file SptModule.h
/// @brief SptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/FileRegion.h"

#include "verilog/pt/PtModule.h"
#include "PtiDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// module を表すノード
//////////////////////////////////////////////////////////////////////
class SptModule :
  public PtModule
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptModule(const FileRegion& file_region,
	    const char* name,
	    bool macro,
	    bool is_cell,
	    bool is_protected,
	    int time_unit,
	    int time_precision,
	    tVpiNetType net_type,
	    tVpiUnconnDrive unconn,
	    tVpiDefDelayMode delay,
	    int decay,
	    bool explicit_name,
	    bool portfaults,
	    bool suppress_faults,
	    const string& config,
	    const string& library,
	    const string& cell,
	    PtDeclHeadArray paramport_array,
	    PtiPortArray port_array,
	    PtIOHeadArray iodecl_array,
	    PtDeclHeadArray decl_array,
	    PtItemArray item_array);

  /// デストラクタ
  virtual
  ~SptModule();


public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 名前の取得
  virtual
  const char*
  name() const;

  /// macromodule 情報の取得
  virtual
  bool
  is_macromodule() const;

  /// cell 情報の取得
  virtual
  bool
  is_cell() const;

  /// protect 情報の取得
  virtual
  bool
  is_protected() const;

  /// time unit の取得
  virtual
  int
  time_unit() const;

  /// time precision の取得
  virtual
  int
  time_precision() const;

  /// default net type の取得
  virtual
  tVpiNetType
  nettype() const;

  /// unconnected drive の取得
  virtual
  tVpiUnconnDrive
  unconn_drive() const;

  /// default delay mode の取得
  virtual
  tVpiDefDelayMode
  delay_mode() const;

  /// default decay time の取得
  virtual
  int
  decay_time() const;

  /// portfaults 情報の取得
  virtual
  bool
  portfaults() const;

  /// suppress_faults 情報の取得
  virtual
  bool
  suppress_faults() const;

  /// config 情報の取得
  virtual
  const string&
  config() const;

  /// library 情報の取得
  virtual
  const string&
  library() const;

  /// cell 情報の取得
  virtual
  const string&
  cell() const;

  /// @brief パラメータポート宣言配列の取得
  virtual
  PtDeclHeadArray
  paramport_array() const;

  /// @brief ポート数の取得
  /// @return ポート数
  virtual
  ymuint
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  virtual
  const PtPort*
  port(ymuint pos) const;

  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

  /// @brief 入出力宣言の要素数の取得
  /// @note 個々のヘッダが持つ要素数の総和を計算する．
  virtual
  ymuint
  iodecl_num() const;

  /// @brief 宣言ヘッダ配列の取得
  virtual
  PtDeclHeadArray
  declhead_array() const;

  /// @brief item 配列の取得
  virtual
  PtItemArray
  item_array() const;

  /// 関数名から関数の検索
  virtual
  const PtItem*
  find_function(const char* name) const;

  /// top_module フラグを下ろす
  virtual
  void
  clear_topmodule() const;

  /// top module のチェック
  virtual
  bool
  is_topmodule() const;

  /// @brief in_use フラグの設定
  virtual
  void
  set_in_use() const;

  /// @brief in_use フラグの解除
  virtual
  void
  reset_in_use() const;

  /// @brief in_use フラグの取得
  virtual
  bool
  is_in_use() const;


private:

  // すべてのポートが名前を持っていることを記録する．
  void
  set_named_port();

  // すべてのポートが外部名を持っているときに true を返す．
  // { a, b } のような名無しポートがあると false となる．
  // true の時しか名前による結合は行えない．
  bool
  explicit_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 様々な情報をパックしたもの
  mutable
  ymuint32 mFlags;

  // decay time
  ymint32 mDefDecayTime;

  // config 情報
  string mConfig;

  // library 情報
  string mLibrary;

  // cell 情報
  string mCell;

  // パラメータポート宣言の配列
  PtDeclHeadArray mParamPortArray;

  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言の配列
  PtIOHeadArray mIOHeadArray;

  // 入出力宣言の要素数
  ymuint32 mIODeclNum;

  // 宣言リスト
  PtDeclHeadArray mDeclHeadArray;

  // 要素のリスト
  PtItemArray mItemArray;

#if __GNUC__ == 4 && __GNUC_MINOR__ >= 6
  typedef unordered_map<string, const PtItem*> StrItemMap;
#else
  typedef hash_map<string, const PtItem*> StrItemMap;
#endif

  // 関数定義の辞書
  StrItemMap mFuncDic;

};


//////////////////////////////////////////////////////////////////////
/// port を表すクラス
//////////////////////////////////////////////////////////////////////
class SptPort :
  public PtiPort
{
  friend class SptFactory;

private:

  /// @brief コンストラクタ
  SptPort(const FileRegion& file_region,
	  const PtExpr* portref,
	  PtExprArray portref_array,
	  const char* ext_name);

  /// @brief デストラクタ
  virtual
  ~SptPort();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// @brief 外向の名前の取得
  virtual
  const char*
  ext_name() const;

  /// @brief 内側のポート結線を表す式の取得
  virtual
  const PtExpr*
  portref() const;

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  ymuint
  portref_size() const;

  /// @brief 内部のポート結線の取得
  virtual
  const PtExpr*
  portref_elem(ymuint pos) const;

  /// @brief 内部のポート結線の報告の取得
  virtual
  tVlDirection
  portref_dir(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // PtiPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  /// @param[in] dir 方向
  virtual
  void
  _set_portref_dir(ymuint pos,
		   tVlDirection dir);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 外部向きの名前
  const char* mExtName;

  // portef
  const PtExpr* mPortRef;

  // portref の配列
  PtExprArray mPortRefArray;

  // portref の方向の配列
  tVlDirection* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTMODULE_H
