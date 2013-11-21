#ifndef YM_LOGIC_DIMACSPARSER_H
#define YM_LOGIC_DIMACSPARSER_H

/// @file ym_logic/DimacsParser.h
/// @brief DimacsParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/sat_nsdef.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_SAT

// クラスの前方宣言
class DimacsParserImpl;

//////////////////////////////////////////////////////////////////////
/// @class DimacsHandler DimacsParser.h "ym_logic/DimacsParser.h"
/// @ingroup SatGroup
/// @brief DIMACS パーサーのイベントハンドラの基底クラス
/// @sa DimacsParser
//////////////////////////////////////////////////////////////////////
class DimacsHandler
{
protected:

  /// @brief コンストラクタ
  DimacsHandler();

  /// @brief デストラクタ
  virtual
  ~DimacsHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは必要に応じて下記の仮想関数を上書きすること
  // デフォルトのハンドラはなにもしない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init();

  /// @brief p 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] nv 変数の数
  /// @param[in] nc 節の数
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_p(const FileRegion& loc,
	 ymuint nv,
	 ymuint nc);

  /// @brief clause 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] lits リテラルの配列．最後の0は含まない
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_clause(const FileRegion& loc,
	      const vector<int>& lits);

  /// @brief 終了処理
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();

};


//////////////////////////////////////////////////////////////////////
/// @class DimacsParser DimacsParser.h "ym_logic/DimacsParser.h"
/// @ingroup SatGroup
/// @brief DIMACS形式の CNFファイルを読み込むパーサークラス
/// @sa DimacsHandler
//////////////////////////////////////////////////////////////////////
class DimacsParser
{
public:

  /// @brief コンストラクタ
  DimacsParser();

  /// @brief デストラクタ
  ~DimacsParser();


public:

  /// @brief 読み込みを行う．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  void
  add_handler(DimacsHandler* handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  DimacsParserImpl* mImpl;

};

END_NAMESPACE_YM_SAT

#endif // YM_LOGIC_DIMACSPARSER_H
