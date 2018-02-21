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

test("inserts an empty line if such a token occurs", () =>
  expect(Printer.print([LeftPar, EmptyLine, RightPar]))
  |> toEqual("(\n\n  )\n")
);

test("inserts only one empty line even if it follows a hard line break", () =>
  expect(Printer.print([LeftPar, RightPar, EmptyLine, LeftPar, RightPar]))
  |> toEqual("()\n\n()\n")
);

test("indents after an empty line", () =>
  expect(Printer.print([LeftPar, EmptyLine, Identifier("x"), RightPar]))
  |> toEqual("(\n\n  x)\n")
);

test(
  "inserts a soft line break if the next token does not fit on the same line",
  () => {
  let identifier = String.make(Printer.maxLineLength - 2, 'x');
  expect(
    Printer.print([LeftPar, Identifier(identifier), Identifier("x"), RightPar])
  )
  |> toEqual("(" ++ identifier ++ "\n  x)\n");
});

test(
  "does not insert a soft line break if the next token just barely fits on the same line",
  () => {
    let identifier = String.make(Printer.maxLineLength - 4, 'x');
    expect(
      Printer.print([
        LeftPar,
        Identifier(identifier),
        Identifier("x"),
        RightPar
      ])
    )
    |> toEqual("(" ++ identifier ++ " x)\n");
  }
);

test(
  "does not calculate line length based on a space that was replaced by a line break",
  () => {
    let identifier = String.make(Printer.maxLineLength - 3, 'x');
    expect(
      Printer.print([
        LeftPar,
        Identifier("xx"),
        Identifier(identifier),
        RightPar
      ])
    )
    |> toEqual("(xx\n  " ++ identifier ++ ")\n"); /* rightPar should not be on new line */
  }
);

test(
  "uses space beyond the line length limit for further tokens once the limit has been violated",
  () => {
    let tooLargeIdentifier = String.make(Printer.maxLineLength - 1, 'x');
    let halfLineIdentifier = String.make(Printer.maxLineLength / 2 - 1, 'x');
    expect(
      Printer.print([
        LeftPar,
        Identifier("x"),
        Identifier(tooLargeIdentifier),
        Identifier(halfLineIdentifier),
        Identifier(halfLineIdentifier),
        RightPar
      ])
    )
    |> toEqual(
         "(x\n  "
         ++ tooLargeIdentifier
         ++ "\n  "
         ++ halfLineIdentifier
         ++ " "
         ++ halfLineIdentifier
         ++ "\n)\n"
       );
  }
);

test("does not apply the extended line length limit after a hard break", () => {
  let tooLargeIdentifier = String.make(Printer.maxLineLength - 1, 'x');
  let exactlyFittingIdentifier = String.make(Printer.maxLineLength - 2, 'x');
  expect(
    Printer.print([
      LeftPar,
      Identifier("x"),
      Identifier(tooLargeIdentifier),
      RightPar,
      LeftPar,
      Identifier("x"),
      Identifier(exactlyFittingIdentifier),
      RightPar
    ])
  )
  |> toEqual(
       "(x\n  "
       ++ tooLargeIdentifier
       ++ "\n)\n(x\n  "
       ++ exactlyFittingIdentifier
       ++ "\n)\n"
     );
});

test("indents multiple levels deep", () => {
  let identifier = String.make(Printer.maxLineLength - 3, 'x');
  expect(
    Printer.print([
      LeftPar,
      Identifier(identifier),
      LeftPar,
      Identifier("x"),
      RightPar,
      RightPar
    ])
  )
  |> toEqual("(" ++ identifier ++ " (\n    x))\n");
});
