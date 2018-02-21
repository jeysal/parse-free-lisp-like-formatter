open Token;

let maxLineLength = 42;

let rec print = (~consecutiveBreaks=NoBreaks, ~nestingLevel=0, tokens) =>
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
    let (whitespaceAfter, newConsecutiveBreaks) =
      switch (current, rest, nestingLevel) {
      /* break after */
      | (LineComment(_), _, _)
      | (RightPar, _, 1) => ("\n", SingleBreak)
      | (EmptyLine, _, _) => (
          switch consecutiveBreaks {
          | NoBreaks => "\n\n"
          | SingleBreak => "\n"
          | MultipleBreaks => ""
          },
          MultipleBreaks
        )
      /* no space after */
      | (LeftPar, _, _)
      | (Prefix(_), _, _)
      | (_, [RightPar, ..._], _) => ("", NoBreaks)
      /* otherwise insert space */
      | _ => (" ", NoBreaks)
      };
    /* recursively print remaining tokens */
    let newNestingLevel =
      switch current {
      | LeftPar => nestingLevel + 1
      | RightPar => nestingLevel - 1
      | _ => nestingLevel
      };
    let restCode =
      print(
        rest,
        ~nestingLevel=newNestingLevel,
        ~consecutiveBreaks=newConsecutiveBreaks
      );
    /* assemble code from pieces */
    currentCode ++ whitespaceAfter ++ restCode;
  };
