# Lexical Scanner
# http://www.ecs.syr.edu/faculty/fawcett/handouts/CSE687/Lectures/Project1-S2016.htm

This is a lexical scanner in C++ that consists of 2 two packages:
Tokenizer:
extracts words, called tokens, from a stream of characters. Token boundaries are white-space characters, transitions between alphanumeric and punctuator characters, and comment and string boundaries. Certain classes of punctuator characters belong to single character or two character tokens so they require special rules for extraction.

SemiExpressions:
groups tokens into sets, each of which contain all the information needed to analyze some grammatical construct without containing extra tokens that have to be saved for subsequent analyses. SemiExpressions are determined by special terminating characters: semicolon, open brace, closed brace, newline when preceeded on the same line with '#', and colon when preceded by one of the three tokens "public", "protected", or "private".


It collects the following token types:
alphanumeric tokens
punctuator tokens
special one3 and two4 character tokens with defaults that may be changed by calling setSpecialSingleChars(string ssc) and/or setSpecialCharPairs(string scp).
C style comments returned as a single token
C++ style comments returned as a single token
quoted strings

Special one character tokens: <, >, [, ], (, ), {, }, :, =, +, -, *, \n
Special two character tokens: <<, >>, ::, ++, --, ==, +=, -=, *=, /=
"abc" becomes the token abc and the outer quotes are discarded. "\"abc\"" becomes the token "abc" with the outer quotes discarded.
