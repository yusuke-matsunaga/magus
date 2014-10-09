# coding=UTF-8

# 文脈自由文法を表すクラス
#
# - 文法記号は内部では0から始まる整数で表す．
#   ただし 0 番は実際には現れない末尾を示す記号($)
#   番号と文法記号の対応付けは m_TokenList で行う．
#   id 番目の文法記号は m_TokenList[id] でアクセス
#   できる．
#
# - 文法規則は (left, right) という2つ組で表す．
#   left は左辺の(非終端) 記号番号，right は
#   右辺の記号番号のタプル
#   文法規則も内部では0から始まる整数で表す．
#   m_RuleList に格納される．
#
# - LR(0) 項は (rule_id, pos) という2つ組で表す．
class Grammer :

    # コンストラクタ
    def __init__(self) :
        self.clear()

    # @brief 内容をクリアする．
    def clear(self) :
        self.m_DEBUG = False

        # 文法記号のリスト
        self.m_TokenList = []

        # 終端記号を表すフラグ
        self.m_TerminalList = []

        # 0 番目の '$' は終端文字
        ret = self.add_token('$')
        assert ret == 0

        # 文法規則のリスト
        self.m_RuleList = []

        # 開始記号
        self.m_StartNode = -1

        # 開始規則
        self.m_StartRule = -1

        # FIRST マップ
        self.m_FirstMap = []

        # FOLLOW マップ
        self.m_FollowMap = []

    # @brief 文法記号の追加
    # @param[in] token 追加するトークン文字列
    # @return トークン番号を返す．
    def add_token(self, token) :
        ret = len(self.m_TokenList)
        self.m_TokenList.append(token)
        self.m_TerminalList.append(True)
        return ret

    # @brief 文法規則の追加
    # @param[in] left 左辺のトークン番号
    # @param[in] right 右辺式を表すトークン番号のタプル
    # @return 文法番号を返す．
    def add_rule(self, left, right) :
        assert left < len(self.m_TokenList)
        for token in right :
            assert token < len(self.m_TokenList)

        self.m_TerminalList[left] = False
        ret = len(self.m_RuleList)
        self.m_RuleList.append( (left, right) )
        return ret

    # @brief 開始記号の設定
    # @param[in] start 開始記号のトークン番号
    #
    # この関数は clear() 後に一回しか呼べない．
    def set_start(self, start) :
        assert self.m_StartNode == -1
        ss_id = self.add_token('_START_')
        self.m_StartNode = ss_id
        self.m_StartRule = self.add_rule(ss_id, (start, ))

        # 左辺の文字数の最大値を求める．
        left_max = 0
        for rule in self.m_RuleList :
            (left, right) = rule
            tmp = self.id2token(left)
            tmp_len = len(tmp)
            if left_max < tmp_len :
                left_max = tmp_len

        self.m_LeftFormat = "%%-%ds ::=" % left_max

        self.calc_first()

    # @brief FIRST()/FOLLOW() を求める．
    def calc_first(self) :
        # m_FirstMap を [] で初期化する．
        self.m_FirstMap = []
        for token in self.m_TokenList :
            self.m_FirstMap.append([])

        # 終端記号の FIRST() は自分自身
        for token_id in range(0, len(self.m_TokenList)) :
            if self.m_TerminalList[token_id] :
                self.m_FirstMap[token_id].append(token_id)

        # 変更がなくなるまで以下のループを繰り返す．
        update = True
        while update :
            update = False
            for (left, right) in self.m_RuleList :
                all_epsilon = True
                flist = []
                for node_id in right :
                    has_epsilon = False
                    for token_id1 in self.m_FirstMap[node_id] :
                        if token_id1 == -1 :
                            has_epsilon = True
                        else :
                            add_to_tokenlist(token_id1, flist)
                    if not has_epsilon :
                        all_epsilon = False
                        break
                for token_id1 in flist :
                    if add_to_tokenlist(token_id1, self.m_FirstMap[left]) :
                        update = True
                if all_epsilon :
                    if add_to_tokenlist(-1, self.m_FirstMap[left]) :
                        update = True
        # ソートしておく
        for token_id in range(0, len(self.m_TokenList)) :
            self.m_FirstMap[token_id].sort()

        # self.m_FollowMap を [] で初期化する．
        self.m_FollowMap = []
        for token_id in self.m_TokenList :
            self.m_FollowMap.append([])

        self.m_FollowMap[self.m_StartNode].append(0)
        update = True
        while update :
            update = False
            for (left, right) in self.m_RuleList :
                n = len(right)
                for i in range(0, n - 1) :
                    for token_id in self.m_FirstMap[right[i + 1]] :
                        if token_id == -1 :
                            has_epsilon = True
                        else :
                            if add_to_tokenlist(token_id, self.m_FollowMap[right[i]]) :
                                update = True
                    if has_epsilon :
                        for token_id in self.m_FollowMap[left] :
                            if add_to_tokenlist(token_id, self.m_FollowMap[right[i]]) :
                                update = True
                for token_id in self.m_FollowMap[left] :
                    if add_to_tokenlist(token_id, self.m_FollowMap[right[n - 1]]) :
                        update = True
        # ソートしておく
        for token_id in range(0, len(self.m_TokenList)) :
            self.m_FollowMap[token_id].sort()

    # 記号列(トークン番号のリスト)に対する FIRST() を計算する．
    def first(self, token_list) :
        ans_list = []
        all_epsilon = True
        for token in token_list :
            has_epsilon = False
            for tmp_id in self.m_FirstMap[token] :
                if tmp_id == -1 :
                    has_epsilon = True
                else :
                    add_to_tokenlist(tmp_id, ans_list)
            if not has_epsilon :
                all_epsilon = False
                break
        if all_epsilon :
            ans_list.append(-1)
        return ans_list

    # @brief 内容を表示する
    def print_rules(self) :
        rule_id = 0
        for rule in self.m_RuleList :
            (left, right) = rule
            line = "Rule (%d): " % rule_id
            rule_id += 1
            line += self.m_LeftFormat % self.id2token(left)
            for id in right :
                line += " ";
                line += self.id2token(id)
            print line
        print ""

    # @brief トークンの情報とFIRST/FOLLOW を表示する．
    def print_tokens(self) :
        for token_id in range(1, len(self.m_TokenList)) :
            print '%s:' % self.id2token(token_id)
            print '  FIRST: ',
            for tmp in self.m_FirstMap[token_id] :
                if tmp == -1 :
                    print '<>',
                else :
                    print self.id2token(tmp),
            print ''
            print '  FOLLOW: ',
            for tmp in self.m_FollowMap[token_id] :
                if tmp == -1 :
                    print '<>',
                else :
                    print self.id2token(tmp),
            print ''
            print ''

    # @brief トークン番号からトークン文字列を得る．
    # @param[in] id トークン番号
    def id2token(self, id) :
        assert id < len(self.m_TokenList)
        return self.m_TokenList[id]

    # @brief 規則番号から規則を得る．
    # @param[in] id 規則番号
    def id2rule(self, id) :
        assert id < len(self.m_RuleList)
        return self.m_RuleList[id]

# @brief トークンリストに重複なく追加する．
def add_to_tokenlist(token, token_list) :
    if not token in token_list :
        token_list.append(token)
        return True
    else :
        return False

if __name__ == '__main__' :
    # テストプログラム
    g = Grammer()

    id = g.add_token('id')
    plus = g.add_token('+')
    times = g.add_token('*')
    lpar = g.add_token('(')
    rpar = g.add_token(')')
    expr = g.add_token('expr')
    term = g.add_token('term')
    factor = g.add_token('factor')

    g.add_rule(expr, (expr, plus, term))
    g.add_rule(expr, (term, )) # (term) ではタプルにならない

    g.add_rule(term, (term, times, factor))
    g.add_rule(term, (factor, )) # (factor) ではタプルにならない

    g.add_rule(factor, (lpar, expr, rpar))
    g.add_rule(factor, (id, )) # (id) ではタプルにならない

    g.set_start(expr)

    g.print_rules()

    g.print_tokens()
