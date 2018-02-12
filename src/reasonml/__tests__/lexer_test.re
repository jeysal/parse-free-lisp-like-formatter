open Jest;

open Expect;

open Token;

test("recognizes all regular token types", () =>
  expect(Lexer.tokenize({|
    (lambda id1 &id2
      (= true 123))
  |}))
  |> toEqual([
       LeftPar,
       Keyword("lambda"),
       Identifier("id1"),
       Prefix("&"),
       Identifier("id2"),
       LeftPar,
       Operator("="),
       BoolLiteral("true"),
       NumLiteral("123"),
       RightPar,
       RightPar
     ])
);

test("recognizes line comments", () =>
  expect(Lexer.tokenize({|
    (a ;comment
    b)
  |}))
  |> toEqual([
       LeftPar,
       Identifier("a"),
       LineComment(";comment"),
       Identifier("b"),
       RightPar
     ])
);

test("recognizes block comments", () =>
  expect(Lexer.tokenize({|
    (a #|com
    ment|# b)
  |}))
  |> toEqual([
       LeftPar,
       Identifier("a"),
       BlockComment("#|com\n    ment|#"),
       Identifier("b"),
       RightPar
     ])
);

test("throws if a '#' is not followed by a '|'", () =>
  expect(() =>
    Lexer.tokenize({|
    (a # b)
  |})
  ) |> toThrow
);

test("generates tokens for empty lines", () =>
  expect(Lexer.tokenize({|
      (a

      b)
  |}))
  |> toEqual([LeftPar, Identifier("a"), EmptyLine, Identifier("b"), RightPar])
);

test("treats a line as empty even if it contains whitespace", () =>
  expect(Lexer.tokenize("(a\n\t\nb)"))
  |> toEqual([LeftPar, Identifier("a"), EmptyLine, Identifier("b"), RightPar])
);

test("throws if the code includes an illegal char", () =>
  expect(() =>
    Lexer.tokenize("(a ~ b)")
  ) |> toThrow
);
