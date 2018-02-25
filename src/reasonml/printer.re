open Token;

let maxLineLength = 42;

let indentSize = 2;

let indent = nestingLevel => String.make(nestingLevel * indentSize, ' ');

let tokenText = token =>
  switch token {
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

type whitespaceInsertion = {
  whitespace: string,
  newLineLength: int,
  newConsecutiveBreaks: consecutiveBreaks
};

/* whitespace before token */
let whitespaceBefore =
    (
      ~currentToken,
      ~currentCodeLength,
      ~previousToken,
      ~consecutiveBreaks,
      ~lineLength,
      ~lineLengthLimit,
      ~nestingLevel
    ) => {
  let whitespace =
    switch (previousToken, currentToken, consecutiveBreaks) {
    /* previous allows no space after */
    | (None | Some(LeftPar | Prefix(_)), _, _)
    /* current allows no space before */
    | (_, RightPar | EmptyLine, _)
    /* at beginning of line */
    | (_, _, SingleBreak | MultipleBreaks) => ""
    | _ => " "
    };
  let printLength = lineLength + String.length(whitespace) + currentCodeLength;
  if (printLength > lineLengthLimit && consecutiveBreaks == NoBreaks) {
    /* insert soft line break */
    let indentation = indent(nestingLevel);
    {
      whitespace: "\n" ++ indentation,
      newLineLength: String.length(indentation),
      newConsecutiveBreaks: SingleBreak
    };
  } else {
    {
      /* insert space (or nothing) */
      whitespace,
      newLineLength: lineLength + String.length(whitespace),
      newConsecutiveBreaks: consecutiveBreaks
    };
  };
};

/* whitespace after token */
let whitespaceAfter =
    (~currentToken, ~lineLength, ~nestingLevel, ~consecutiveBreaks) =>
  switch (currentToken, nestingLevel) {
  | (LineComment(_), _)
  | (RightPar, 0) => {
      whitespace: "\n",
      newLineLength: 0,
      newConsecutiveBreaks: SingleBreak
    }
  | (EmptyLine, _) => {
      whitespace:
        (
          switch consecutiveBreaks {
          | NoBreaks => "\n\n"
          | SingleBreak => "\n"
          | MultipleBreaks => ""
          }
        )
        ++ indent(nestingLevel),
      newLineLength: 0,
      newConsecutiveBreaks: MultipleBreaks
    }
  | _ => {
      whitespace: "",
      newLineLength: lineLength,
      newConsecutiveBreaks: NoBreaks
    }
  };

/* main printer function */
let rec print =
        (
          ~consecutiveBreaks=NoBreaks,
          ~nestingLevel=0,
          ~lineLength=0,
          ~transientLineLengthLimit=maxLineLength,
          ~previousToken=?,
          tokens
        ) =>
  switch tokens {
  | [] => ""
  | [currentToken, ...rest] =>
    let currentCode = tokenText(currentToken);
    /* nesting level */
    let nestingLevelBeforeToken =
      currentToken == RightPar ? nestingLevel - 1 : nestingLevel;
    let nestingLevelAfterToken =
      currentToken == LeftPar ? nestingLevel + 1 : nestingLevelBeforeToken;
    /* before token, whitespace */
    let {
      whitespace: leadingWhitespace,
      newLineLength: lineLengthBeforeToken,
      newConsecutiveBreaks: consecutiveBreaksBeforeToken
    } =
      whitespaceBefore(
        ~currentToken,
        ~currentCodeLength=String.length(currentCode),
        ~previousToken,
        ~consecutiveBreaks,
        ~lineLength,
        ~lineLengthLimit=transientLineLengthLimit,
        ~nestingLevel=nestingLevelBeforeToken
      );
    /* after token */
    let lineLengthAfterToken =
      lineLengthBeforeToken + String.length(currentCode);
    let transientLineLengthLimitAfterToken =
      Js.Math.max_int(transientLineLengthLimit, lineLengthAfterToken);
    let consecutiveBreaksAfterToken = consecutiveBreaksBeforeToken;
    /* whitespace */
    let {whitespace: trailingWhitespace, newLineLength, newConsecutiveBreaks} =
      whitespaceAfter(
        ~currentToken,
        ~nestingLevel=nestingLevelAfterToken,
        ~lineLength=lineLengthAfterToken,
        ~consecutiveBreaks=consecutiveBreaksAfterToken
      );
    /* reset transient line length limit after a hard break */
    let newTransientLineLengthLimit =
      newConsecutiveBreaks == NoBreaks ?
        transientLineLengthLimitAfterToken : maxLineLength;
    /* recursively print remaining tokens */
    let restCode =
      print(
        rest,
        ~lineLength=newLineLength,
        ~transientLineLengthLimit=newTransientLineLengthLimit,
        ~previousToken=currentToken,
        ~nestingLevel=nestingLevelAfterToken,
        ~consecutiveBreaks=newConsecutiveBreaks
      );
    /* assemble code from pieces */
    leadingWhitespace ++ currentCode ++ trailingWhitespace ++ restCode;
  };
