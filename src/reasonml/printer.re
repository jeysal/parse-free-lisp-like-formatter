open Token;

let rec print = (~nestingLevel=0, tokens) =>
  switch tokens {
  | [] => ""
  | [current, ...rest] =>
    switch current {
    | LeftPar => "(" ++ print(rest, ~nestingLevel=nestingLevel + 1)
    | RightPar =>
      ")"
      ++ (nestingLevel == 1 ? "\n" : "")
      ++ print(rest, ~nestingLevel=nestingLevel - 1)
    | LineComment(text) => text ++ "\n" ++ print(rest, ~nestingLevel)
    | EmptyLine => print(rest, ~nestingLevel)
    | Operator(text)
    | Prefix(text)
    | NumLiteral(text)
    | BoolLiteral(text)
    | Keyword(text)
    | Identifier(text)
    | BlockComment(text) => text ++ print(rest, ~nestingLevel)
    }
  };
