"""
LL(1) 预测分析表（表达式文法）

文法：
    E  -> T E'
    E' -> + T E' | - T E' | ε
    T  -> F T'
    T' -> * F T' | / F T' | ε
    F  -> ( E ) | id | num

"""

NON_TERMINAL = {'E', "E'", 'T', "T'", 'F'}

parsing_table = {
    "E": {
        "LPAREN": ["T", "E'"],
        "ID": ["T", "E'"],
        "NUM": ["T", "E'"],
    },

    "E'": {
        "PLUS": ["PLUS", "T", "E'"],
        "MINUS": ["MINUS", "T", "E'"],
        "RPAREN": ['epsilon'],
        'EOF': ['epsilon'],
    },

    "T": {
        "LPAREN": ["F", "T'"],
        "ID": ["F", "T'"],
        "NUM": ["F", "T'"],
    },

    "T'": {
        "PLUS": ['epsilon'],
        "MINUS": ['epsilon'],
        "TIMES": ["TIMES", "F", "T'"],
        "OVER": ["OVER", "F", "T'"],
        "RPAREN": ['epsilon'],
        'EOF': ['epsilon'],
    },

    "F": {
        "LPAREN": ["LPAREN", "E", "RPAREN"],
        "ID": ["ID"],
        "NUM": ["NUM"],
    },
}