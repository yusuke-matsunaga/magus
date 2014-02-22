#ifndef SPTMISC_H
#define SPTMISC_H

/// @file SptMisc.h
/// @brief その他の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/FileRegion.h"

#include "verilog/pt/PtMisc.h"
#include "verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// delay control/event control/repeat control を表すベースクラス
//////////////////////////////////////////////////////////////////////
class SptControl :
  public PtControl
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptControl(const FileRegion& file_region,
	     tPtCtrlType type,
	     const PtExpr* expr,
	     PtExprArray event_array);

  /// デストラクタ
  virtual
  ~SptControl();


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 型の取得
  virtual
  tPtCtrlType
  type() const;

  /// 遅延式の取得
  virtual
  const PtExpr*
  delay() const;

  /// @brief イベントリストのサイズの取得
  /// @retval イベントリストのサイズ event control/repeat control の場合
  /// @retval 0 上記以外
  virtual
  ymuint
  event_num() const;

  /// @brief イベントリストの要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  /// @note event control/repeat control の場合のみ意味を持つ
  virtual
  const PtExpr*
  event(ymuint pos) const;

  /// 繰り返し数の取得
  virtual
  const PtExpr*
  rep_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // クラスの型
  tPtCtrlType mType;

  // 遅延を表す式/繰り返し回数を表す式
  const PtExpr* mExpr;

  // イベントの配列
  PtExprArray mEventArray;

};


//////////////////////////////////////////////////////////////////////
/// ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class SptConnection :
  public PtConnection
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptConnection(const FileRegion& file_region,
		const PtExpr* expr,
		const char* name = NULL);

  /// デストラクタ
  virtual
  ~SptConnection();


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 名前の取得
  virtual
  const char*
  name() const;

  /// 式の取得
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // パラメータ名/ポート名
  const char* mName;

  // 接続を表す式
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// strength を表すクラス
//////////////////////////////////////////////////////////////////////
class SptStrength :
  public PtStrength
{
  friend class SptFactory;

private:

  /// コンストラクタ (0/1 の強度用)
  SptStrength(const FileRegion& file_region,
	      tVpiStrength drive0,
	      tVpiStrength drive1);

  /// コンストラクタ (電荷の強度用)
  SptStrength(const FileRegion& file_region,
	      tVpiStrength charge);

  /// デストラクタ
  virtual
  ~SptStrength();


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// drive strength0 の取得
  virtual
  tVpiStrength
  drive0() const;

  /// drive strength1 の取得
  virtual
  tVpiStrength
  drive1() const;

  /// charge strength の取得
  virtual
  tVpiStrength
  charge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  tVpiStrength mDrive0;
  tVpiStrength mDrive1;
  tVpiStrength mCharge;
};


//////////////////////////////////////////////////////////////////////
/// delay を表すクラス
//////////////////////////////////////////////////////////////////////
class SptDelay :
  public PtDelay
{
  friend class SptFactory;

private:

  /// 一つの値をとるコンストラクタ
  SptDelay(const FileRegion& file_region,
	   const PtExpr* value1);

  /// 二つの値をとるコンストラクタ
  SptDelay(const FileRegion& file_region,
	   const PtExpr* value1,
	   const PtExpr* value2);

  /// 三つの値をとるコンストラクタ
  SptDelay(const FileRegion& file_region,
	   const PtExpr* value1,
	   const PtExpr* value2,
	   const PtExpr* value3);

  /// デストラクタ
  virtual
  ~SptDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 値の取得
  virtual
  const PtExpr*
  value(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 値のリスト
  const PtExpr* mValue[3];

};


//////////////////////////////////////////////////////////////////////
/// 階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class SptNameBranch :
  public PtNameBranch
{
public:

  /// 名前のみのコンストラクタ
  SptNameBranch(const char* name);

  /// 名前とインデックスのコンストラクタ
  SptNameBranch(const char* name,
		int index);

  /// デストラクタ
  virtual
  ~SptNameBranch();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 名前の取得
  virtual
  const char*
  name() const;

  /// インデックスの有無のチェック
  virtual
  bool
  has_index() const;

  /// インデックスの取得
  virtual
  int
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // インデックス
  // ただし使うときは1ビット左にシフトして最下位ビットに1を立てておく．
  // インデックス0とインデックスなしを区別するため．
  ymuint32 mIndex;

};


//////////////////////////////////////////////////////////////////////
/// attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class SptAttrInst :
  public PtAttrInst
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptAttrInst(PtAttrSpecArray as_array);

  /// デストラクタ
  virtual
  ~SptAttrInst();


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  /// @return 要素数
  virtual
  ymuint
  attrspec_num() const;

  /// @brief 要素の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < attrspec_num() )
  virtual
  const PtAttrSpec*
  attrspec(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // attr spec の配列
  PtAttrSpecArray mAttrSpecArray;

};


//////////////////////////////////////////////////////////////////////
/// attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class SptAttrSpec :
  public PtAttrSpec
{
  friend class SptFactory;

private:

  /// コンストラクタ
  SptAttrSpec(const FileRegion& file_region,
	      const char* name,
	      const PtExpr* expr);

  /// デストラクタ
  virtual
  ~SptAttrSpec();


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  virtual
  FileRegion
  file_region() const;

  /// 名前の取得
  virtual
  const char*
  name() const;

  /// 式の取得
  virtual
  const PtExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 式
  const PtExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // SPTMISC_H
