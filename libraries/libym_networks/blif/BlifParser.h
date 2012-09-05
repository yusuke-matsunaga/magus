#ifndef BLIFPARSER_H
#define BLIFPARSER_H

/// @file BlifParser.h
/// @brief BlifParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/blif_nsdef.h"
#include "ym_cell/cell_nsdef.h"
#include "BlifScanner.h"
#include "IdHash.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

class BlifHandler;

//////////////////////////////////////////////////////////////////////
/// @class BlifParser BlifParser.h <ym_blif/BlifParser.h>
/// @ingroup BlifGroup
/// @brief blif形式のファイルを読み込むパーサークラス
/// 適切なハンドラクラスを生成して add_handler() で登録して使う．
/// @sa BlifHandler MsgHandler
//////////////////////////////////////////////////////////////////////
class BlifParser
{
public:

  /// @brief コンストラクタ
  BlifParser();

  /// @brief デストラクタ
  ~BlifParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込みを行う．
  /// @param[in] filename ファイル名
  /// @param[in] cell_library セルライブラリ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename,
       const CellLibrary* cell_library);

  /// @brief イベントハンドラの登録
  /// @param[in] handler 登録するハンドラ
  /// @note handler はこのインスタンスが破壊される時に同時に破壊される．
  void
  add_handler(BlifHandler* handler);


public:
  //////////////////////////////////////////////////////////////////////
  // BlifHandler から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号から文字列を得る．
  const char*
  id2str(ymuint32 id);

  /// @brief ID番号からそれに関連した位置情報を得る．
  const FileRegion&
  id2loc(ymuint32 id);

  /// @brief ID番号からそれに関連した位置情報を得る．
  const FileRegion&
  id2def_loc(ymuint32 id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを一つ読み出す．
  /// @param[out] loc トークンの位置を格納する変数
  tToken
  get_token(FileRegion& loc);

  /// @brief トークンを戻す．
  void
  unget_token(tToken token,
	      const FileRegion& loc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  // NULL の場合もある．
  const CellLibrary* mCellLibrary;

  // 字句解析器
  BlifScanner mScanner;

  // 読み戻されたトークン
  tToken mUngetToken;

  // mUngetToken に対応する位置情報
  FileRegion mUngetTokenLoc;

  // イベントハンドラのリスト
  list<BlifHandler*> mHandlerList;

  // 識別子のハッシュ表
  IdHash mIdHash;

  // 位置情報の配列
  vector<FileRegion> mLocArray;

  // IdCellの配列
  vector<IdCell*> mNameArray;

  // ID 番号の配列
  vector<ymuint32> mIdArray;

  // キューブ数
  ymuint32 mNc;

  // パタンのバッファ
  StrBuff mCoverPat;

  // 出力の極性
  char mOpat;

  // 現在のセル
  const Cell* mCell;

  // 位置情報バッファ
  FileRegion mLoc1;

  // 文字列バッファ1
  StrBuff mName1;

  // 文字列バッファ2
  StrBuff mName2;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号から文字列を得る．
inline
const char*
BlifParser::id2str(ymuint32 id)
{
  return mIdHash.str(id);
}

// @brief ID番号からそれに関連した位置情報を得る．
inline
const FileRegion&
BlifParser::id2loc(ymuint32 id)
{
  return mIdHash.loc(id);
}

// @brief ID番号からそれに関連した位置情報を得る．
inline
const FileRegion&
BlifParser::id2def_loc(ymuint32 id)
{
  return mIdHash.def_loc(id);
}

// @brief トークンを戻す．
inline
void
BlifParser::unget_token(tToken token,
			const FileRegion& loc)
{
  mUngetToken = token;
  mUngetTokenLoc = loc;
}

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFPARSER_H
