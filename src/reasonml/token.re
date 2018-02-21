type token =
  | LeftPar
  | RightPar
  | Operator(string)
  | Prefix(string)
  | NumLiteral(string)
  | BoolLiteral(string)
  | Keyword(string)
  | Identifier(string)
  | LineComment(string)
  | BlockComment(string)
  | EmptyLine;

type consecutiveBreaks =
  | NoBreaks
  | SingleBreak
  | MultipleBreaks;
