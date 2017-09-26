grammar expr;

Identifier: [a-zA-Z_] [a-zA-Z_0-9]*;
Number: [1-9] [0-9]*;
Plus: '+';
Multiply: '*';
WhiteSpace: [ \t\n\r]+ -> skip;

expr: expr Multiply expr
    | expr Plus expr
    | '(' expr ')'
    | Identifier
    | Number;
