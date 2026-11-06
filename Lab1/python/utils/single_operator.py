'''
识别单字符运算符

RELOP   =
MINUS   -
PLUS    +
TIMES   *
OVER    /
LPAREN  (
RPAREN  )
SEMI    ;

函数返回'类别，符号，下一个位置'
'''

single_operator = {
    '=': 'RELOP',
    '-': 'MINUS',
    '+': 'PLUS',
    '*': 'TIMES',
    '/': 'OVER',
    '(': 'LPAREN',
    ')': 'RPAREN',
    ';': 'SEMI',
}

def recognize_single_operator(src, pos):
    if not 0 <= pos < len(src):
        return None

    now = src[pos]
    token_type = single_operator.get(now)
    if token_type is None:
        return None

    return token_type, now, pos + 1

