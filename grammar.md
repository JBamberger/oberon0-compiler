# Grammar

```bnf
Grammar:

ident = letter {letter | digit}.

integer = digit {digit}.

selector = {"." ident | "[" expression "]"}.

number = integer.

factor = ident selector | number | "(" expression ")" | "~" factor.

term = factor {("*" | "DIV" | "MOD" | "&") factor}.

SimpleExpression = ["+"|"-"] term {("+"|"-" | "OR") term}.

expression = SimpleExpression [("=" | "#" | "<" | "<=" | ">" | ">=") SimpleExpression].

assignment = ident selector ":=" expression.

ActualParameters = "(" [expression {"," expression}] ")" .

ProcedureCall = ident selector [ActualParameters].

IfStatement = "IF" expression "THEN" StatementSequence
              {"ELSIF" expression "THEN" StatementSequence}
              ["ELSE" StatementSequence]
              "END".

WhileStatement = "WHILE" expression "DO" StatementSequence "END".

statement = [assignment | ProcedureCall | IfStatement | WhileStatement].

StatementSequence = statement {";" statement}.

IdentList = ident {"," ident}.

ArrayType = "ARRAY" expression "OF" type.

FieldList = [IdentList ":" type].

RecordType = "RECORD" FieldList {";" FieldList} "END".

type = ident | ArrayType | RecordType.

FPSection = ["VAR"] IdentList ":" type.

FormalParameters = "(" [FPSection {";" FPSection}] ")".

ProcedureHeading = "PROCEDURE" ident [FormalParameters].

ProcedureBody = declarations ["BEGIN" StatementSequence] "END" ident.

ProcedureDeclaration = ProcedureHeading ";" ProcedureBody.

declarations = ["CONST" {ident "=" expression ";"}]
               ["TYPE" {ident "=" type ";"}]
               ["VAR" {IdentList ":" type ";"}]
               {ProcedureDeclaration ";"}.

module = "MODULE" ident ";"
         declarations ["BEGIN" StatementSequence]
         "END" ident "." .
```
