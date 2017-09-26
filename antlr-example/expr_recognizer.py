'''Expression recognizer.
Handles expression described in expr.g4.
Grammar file should be compiled by antlr4 with option '-Dlanguage=Python3' before executing this.
Module 'antlr4' is required.
'''
import antlr4
import exprLexer
import exprParser

if __name__ == '__main__':
    PARSER = exprParser.exprParser(antlr4.CommonTokenStream(exprLexer.exprLexer(
        antlr4.InputStream(input('Please enter a expression: ')))))
    print(PARSER.expr().toStringTree(PARSER.ruleNames, PARSER))
