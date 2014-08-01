#ifndef YMSH_YMSHCMD_H
#define YMSH_YMSHCMD_H

/// @file Ymsh/YmshCmd.h
/// @brief YmshCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Ymsh/ymsh_nsdef.h"
#include "YmUtils/StrBuff.h"


BEGIN_NAMESPACE_YM_YMSH

class YmshImpl;

//////////////////////////////////////////////////////////////////////
/// @class YmshCmd YmshCmd.h "Ymsh/YmshCmd.h"
/// @brief Ymsh のコマンドの基底クラス
/// 実際にはこのクラスを継承したクラスを実装する必要がある．
//////////////////////////////////////////////////////////////////////
class YmshCmd
{
  friend class YmshImpl;
  friend class YmshCmdTable;

public:

  /// @brief コンストラクタ
  /// @param[in] name コマンド名
  YmshCmd(const char* name);

  /// @brief デストラクタ
  virtual
  ~YmshCmd();


public:
  //////////////////////////////////////////////////////////////////////
  // 処理を行うメイン関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンドの実行を行う．
  /// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
  /// @return 実行後の状態を表すコードを返す．
  virtual
  int
  exec(const vector<string>& argv) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に公開している関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンド名を返す．
  const char*
  name() const;

  /// @brief 親のインタープリタを返す．
  YmshImpl*
  ymsh() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  StrBuff mName;

  // 親のインタープリタ
  YmshImpl* mYmsh;

  // ハッシュ表のためのリンク
  YmshCmd* mLink;

};

END_NAMESPACE_YM_YMSH

#endif // YMSH_YMSHCMD_H
