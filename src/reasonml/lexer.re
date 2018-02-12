open Token;

exception IllegalCharacter(string);

type consecutiveBreaks =
  | NoBreaks
  | SingleBreak
  | MultipleBreaks;

let rec tokenize =
        (~consecutiveBreaks=NoBreaks, ~current=?, code)
        : list(token) => {
  let len = String.length(code);
  if (len == 0) {
    [];
  } else {
    let char = code.[0];
    let rest = String.sub(code, 1, len - 1);
    switch (char, current) {
    /* whitespace & breaks */
    | (' ' | '\t', None) => tokenize(rest, ~consecutiveBreaks)
    | ('\n', None) =>
      switch consecutiveBreaks {
      | NoBreaks => tokenize(rest, ~consecutiveBreaks=SingleBreak)
      | SingleBreak => [
          EmptyLine,
          ...tokenize(rest, ~consecutiveBreaks=MultipleBreaks)
        ]
      | MultipleBreaks => tokenize(rest, ~consecutiveBreaks=MultipleBreaks)
      }
    /* parenthesis */
    | ('(', None) => [LeftPar, ...tokenize(rest)]
    | (')', None) => [RightPar, ...tokenize(rest)]
    /* operator */
    | (('=' | '+' | '-' | '*' | '/') as op, None) => [
        Operator(String.make(1, op)),
        ...tokenize(rest)
      ]
    /* prefix */
    | (('\'' | '&') as pre, None) => [
        Prefix(String.make(1, pre)),
        ...tokenize(rest)
      ]
    /* number literal */
    | ('0'..'9' as digit, Some(NumLiteral(num))) =>
      tokenize(rest, ~current=NumLiteral(num ++ String.make(1, digit)))
    | ('0'..'9', None) => tokenize(code, ~current=NumLiteral(""))
    | (_, Some(NumLiteral(_) as lit)) => [lit, ...tokenize(code)]
    /* identifier, keyword, boolean literal */
    | (('0'..'9' | 'a'..'z' | 'A'..'Z') as c, Some(Identifier(name))) =>
      tokenize(rest, ~current=Identifier(name ++ String.make(1, c)))
    | ('a'..'z' | 'A'..'Z', None) => tokenize(code, ~current=Identifier(""))
    | (_, Some(Identifier(id))) => [
        switch id {
        | ("true" | "false") as bool => BoolLiteral(bool)
        | ("quote" | "lambda" | "defun" | "let" | "if" | "and" | "or") as keyword =>
          Keyword(keyword)
        | id => Identifier(id)
        },
        ...tokenize(code)
      ]
    /* line comment */
    | ('\n', Some(LineComment(_) as comment)) => [comment, ...tokenize(code)]
    | (c, Some(LineComment(text))) =>
      tokenize(rest, ~current=LineComment(text ++ String.make(1, c)))
    | (';', None) => tokenize(code, ~current=LineComment(""))
    /* block comment initial detection */
    | ('#', None) => tokenize(rest, ~current=BlockComment("#"))
    | ('|', Some(BlockComment("#"))) =>
      tokenize(rest, ~current=BlockComment("#|"))
    | (c, Some(BlockComment("#"))) =>
      raise(
        IllegalCharacter(
          "Illegal character '" ++ String.make(1, c) ++ "' - expected '|'"
        )
      )
    /* block comment continuation and termination */
    | ('#', Some(BlockComment(text)))
        when text.[String.length(text) - 1] == '|' => [
        BlockComment(text ++ "#"),
        ...tokenize(rest)
      ]
    | (c, Some(BlockComment(text))) =>
      tokenize(rest, ~current=BlockComment(text ++ String.make(1, c)))
    /* illegal character */
    | (c, _) =>
      raise(
        IllegalCharacter("Illegal character '" ++ String.make(1, c) ++ "'")
      )
    };
  };
};
