open Token;

let maxLineLength = 42;

let rec print = (~nestingLevel=0, tokens) =>
  switch tokens {
  | [] => ""
  | [current, ...rest] =>
    /* current token */
    let currentCode =
      switch current {
      | LeftPar => "("
      | RightPar => ")"
      | EmptyLine => ""
      | Prefix(text)
      | Operator(text)
      | NumLiteral(text)
      | BoolLiteral(text)
      | Keyword(text)
      | Identifier(text)
      | LineComment(text)
      | BlockComment(text) => text
      };
    /* spacing or breaks */
    let whitespaceAfter =
      switch (current, rest, nestingLevel) {
      /* hard break after */
      | (LineComment(_), _, _)
      | (RightPar, _, 1) => "\n"
      /* no space after */
      | (LeftPar, _, _)
      | (Prefix(_), _, _)
      | (_, [RightPar, ..._], _) => ""
      /* otherwise insert space */
      | _ => " "
      };
    /* recursively print remaining tokens */
    let newNestingLevel =
      switch current {
      | LeftPar => nestingLevel + 1
      | RightPar => nestingLevel - 1
      | _ => nestingLevel
      };
    let restCode = print(rest, ~nestingLevel=newNestingLevel);
    /* assemble code from pieces */
    currentCode ++ whitespaceAfter ++ restCode;
  };
