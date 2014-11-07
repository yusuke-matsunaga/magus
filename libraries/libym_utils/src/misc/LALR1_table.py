# coding=UTF-8

from grammer import *

# LALR(1)構文解析表を作るモジュール
#
# - LALR(1) 項は (rule_id, pos, token) という3つ組で表す．

class LALR1_table :
    # コンストラクタ
    def __init__(self) :
        self._Debug = False
        self.clear()


    # 内容をクリアする．
    def clear(self) :

        # 状態リスト
        # 状態とは項の集合
        self._StateList = []

        # ACTION マップ
        self._ActionMap = []


    # @brief LALR(1)構文解析表を作る．
    def make_table(self, grammer) :
        self.clear()

        # LR0正準集を求める．
        self._StateList = LR0_state_list(grammer)

        # 先読みの生成/伝搬を調べる．
        generated_token = []
        propagated_token = []
        dummy = grammer._DummyToken
        for state in self._StateList :
            for (rule_id, pos) in state :
                if rule_id != grammer._StartRule and pos == 0 :
                    # 非カーネル項は除外する．
                    continue
                exp_set = LR1_closure(grammer, [(rule_id, pos, dummy)])
                for (rule_id1, pos1, token1) in exp_set :
                    (left, right) = grammer.id2rule(rule_id1)
                    if pos1 == len(right) :
                        continue
                    next_token = right[pos1]
                    state2 = LR0_next_state(grammer, state, next_token)
                    if token1 == dummy :
                        # 伝搬された先読み
                        for (rule_id2, pos2) in state2 :
                            if rule_id2 == rule_id1 and pos2 == pos1 + 1:
                                propagated_token.append( (state, rule_id, pos, state2, rule_id2, pos2) )
                    else :
                        # 生成された先読み
                        for (rule_id2, pos2) in state2 :
                            if rule_id2 == rule_id1 and pos2 == pos1 + 1:
                                generated_token.append( (state2, rule_id2, pos2, token1) )

        start_state = LR0_closure(grammer, [(grammer._StartRule, 0)])
        generated_token.append( (start_state, grammer._StartRule, 0, 0) )

        if self._Debug :
            print "Generated Tokens"
            for (state, rule_id, pos, token) in generated_token :
                state_id = self.state2id(state)
                print " State#%d: Rule%d, %d: %s" % (state_id, rule_id, pos, grammer._TokenList[token])
            print ""
            print "Propagated Tokens"
            for (state1, rule_id1, pos1, state2, rule_id2, pos2) in propagated_token :
                state_id1 = self.state2id(state1)
                state_id2 = self.state2id(state2)
                print " State#%d: Rule%d, %d -> State#%d: Rule%d, %d" % (state_id1, rule_id1, pos1, state_id2, rule_id2, pos2)
            print ""

        # 先読みの伝搬を行う．
        la_token_list = []
        new_token_list = generated_token
        while len(new_token_list) > 0 :
            cur_token_list = new_token_list
            new_token_list = []
            for (state, rule_id, pos, token) in cur_token_list :
                if (state, rule_id, pos, token) in la_token_list :
                    continue
                # term_id の先読みに token を追加
                la_token_list.append( (state, rule_id, pos, token) )
                # (state, rule_id, pos, token) の伝搬を調べる．
                for (state1, rule_id1, pos1, state2, rule_id2, pos2) in propagated_token :
                    if state1 == state and rule_id1 == rule_id and pos1 == pos :
                        new_token_list.append( (state2, rule_id2, pos2, token) )

        # LALR(1) 正準集を作る．
        # 実際には項番号をキーにした先読みトークンリストを作る．

        if self._Debug :
            print "Look Ahead Tokens"
            for (state, rule_id, pos, token) in la_token_list :
                state_id = self.state2id(state)
                print " State#%d: Rule%d, %d: %s" % (state_id, rule_id, pos, grammer._TokenList[token])
            print ""

        # 動作表を作る．
        for state in self._StateList :
            state_id = self.state2id(state)
            self._ActionMap.append({})
            for token_id in range(0, len(grammer._TokenList)) :
                if token_id == grammer._DummyToken :
                    continue
                if grammer._TerminalList[token_id] :
                    for (rule_id, pos) in state :
                        (left, right) = grammer.id2rule(rule_id)
                        if len(right) > pos and right[pos] == token_id :
                            tmp_state = LR0_next_state(grammer, state, token_id)
                            next_id = self.state2id(tmp_state)
                            assert next_id != -1
                            self.set_action(state_id, token_id, 'shift', next_id, rule_id, pos)
                else :
                    tmp_state = LR0_next_state(grammer, state, token_id)
                    if len(tmp_state) > 0 :
                        next_id = self.state2id(tmp_state)
                        self.set_action(state_id, token_id, 'goto', next_id, rule_id, pos)
            for (rule_id, pos) in state :
                (left, right) = grammer.id2rule(rule_id)
                if len(right) == pos :
                    if left == grammer._StartNode :
                        self.set_action(state_id, 0, 'accept', 0, rule_id, pos)
                    else :
                        for (state1, rule_id1, pos1, token1) in la_token_list :
                            state1_id = self.state2id(state1)
                            if state1_id == state_id and rule_id1 == rule_id and pos1 == pos :
                                self.set_action(state_id, token1, 'reduce', rule_id, rule_id, pos)
            # conflict チェック
            for key in self._ActionMap[state_id].keys() :
                action_list = self._ActionMap[state_id][key]
                n = len(action_list)
                if n > 1 :
                    for i in range(0, n - 1) :
                        (action1, action_id1, rule_id1, pos1) = action_list[i]
                        for j in range(i + 1, n) :
                            (action2, action_id2, rule_id2, pos2) = action_list[j]
                            if action1 == "shift" :
                                if action2 == "shift" :
                                    print "shift/shift conflict"
                                if action2 == "reduce" :
                                    print "shift/reduce conflict"
                            if action1 == "reduce" :
                                if action2 == "shift" :
                                    print "shift/reduce conflict"
                                if action2 == "reduce" :
                                    print "reduce/reduce conflict"

    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    def set_action(self, state_id, token_id, action, action_id, rule_id, pos) :
        if self._ActionMap[state_id].has_key(token_id) :
            if not (action, action_id) in self._ActionMap[state_id][token_id] :
                self._ActionMap[state_id][token_id].append( (action, action_id, rule_id, pos) )
        else :
            self._ActionMap[state_id][token_id] = [(action, action_id, rule_id, pos)]

    # @brief LALR(1)状態リストを表示する．
    def print_states(self, grammer) :
        for state in self._StateList :
            state_id = self.state2id(state)
            print 'State#%d:' % state_id
            print ''
            LR0_print_terms(grammer, state)
            print ""
            for key in self._ActionMap[state_id].keys() :
                token = grammer.id2token(key)
                for (action, action_id, rule_id, pos) in self._ActionMap[state_id][key] :
                    print '  %s: %s State#%d , Rule#%d, %d' % (token, action, action_id, rule_id, pos)
            print ''

    # @brief 状態番号を返す．
    # @param[in] state 状態(項の集合)
    def state2id(self, state) :
        state.sort()
        state_id = 0
        for state1 in self._StateList :
            if state1 == state :
                return state_id
            state_id += 1
        else :
            return -1


if __name__ == '__main__' :
    # テストプログラム
    g = Grammer()

    use_1 = False
    use_2 = True

    if use_1 and use_2 :
        use_2 = False

    if use_1 :
        S = g.add_token('S')
        L = g.add_token('L')
        R = g.add_token('R')
        id = g.add_token('id')
        eq = g.add_token('=')
        star = g.add_token('*')

        g.add_rule(S, (L, eq, R))
        g.add_rule(S, (R, ))
        g.add_rule(L, (star, R))
        g.add_rule(L, (id, ))
        g.add_rule(R, (L, ))

        g.set_start(S)

    if use_2:
        expr = g.add_token("expr")
        id = g.add_token("id")
        plus = g.add_token("+")
        times = g.add_token("*")
        lpar = g.add_token("(")
        rpar = g.add_token(")")

        g.add_rule(expr, (id, ))
        g.add_rule(expr, (expr, plus, expr))
        g.add_rule(expr, (expr, times, expr))
        g.add_rule(expr, (lpar, expr, rpar))

        g.set_start(expr)

    g.print_rules()

    g.print_tokens()

    lr1_tab = LALR1_table()

    lr1_tab.make_table(g)

    lr1_tab.print_states(g)
