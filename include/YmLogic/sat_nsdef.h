#ifndef YMYMLOGIC_SAT_NSDEF_H
#define YMYMLOGIC_SAT_NSDEF_H

/// @file YmLogic/sat_nsdef.h
/// @brief SAT Solver 用の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup SatGroup SAT ソルバ
///
/// 中身はフルスクラッチで API もオリジナルだが，実装のアイデアの多くは
/// MiniSat-1.14 を参考にしている．ちなみに MiniSat そのものを ymsat の
/// API で使うこともできる．


#include "YmTools.h"


/// @brief sat 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SAT \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSat)

/// @brief sat 用の名前空間の終了
#define END_NAMESPACE_YM_SAT \
END_NAMESPACE(nsSat) \
END_NAMESPACE_YM

/// @namespace nsYm::nsSat
/// @brief SAT ソルバ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_SAT

class SatSolver;
class SatStats;
class SatMsgHandler;
class SatMsgHandlerImpl1;

class DimacsParser;
class DimacsHandler;
class DimacsVerifier;
class SatDimacsHandler;

END_NAMESPACE_YM_SAT

BEGIN_NAMESPACE_YM

using nsSat::SatSolver;
using nsSat::SatStats;
using nsSat::SatMsgHandler;
using nsSat::SatMsgHandlerImpl1;

using nsSat::DimacsParser;
using nsSat::DimacsHandler;
using nsSat::DimacsVerifier;
using nsSat::SatDimacsHandler;

END_NAMESPACE_YM

#endif // YMYMLOGIC_SAT_NSDEF_H
