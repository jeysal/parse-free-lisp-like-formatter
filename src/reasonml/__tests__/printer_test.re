open Jest;

open Expect;

open Token;

test("inserts a hard line break after a top-level expression", () =>
  expect(
    Printer.print([LeftPar, LeftPar, RightPar, RightPar, LeftPar, RightPar])
  )
  |> toEqual("(())\n()\n")
);

test("inserts a hard line break after a line comment", () =>
  expect(Printer.print([LineComment("; asdf"), LeftPar, RightPar]))
  |> toEqual("; asdf\n()\n")
);

test("inserts a space between tokens, but not inside of parens", () =>
  expect(
    Printer.print([
      LeftPar,
      Operator("="),
      Keyword("if"),
      Identifier("abc"),
      RightPar
    ])
  )
  |> toEqual("(= if abc)\n")
);
